
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LINKTYPE_EN10MB			1

#define PCAP_PKT_MAXSIZE		8192

#define ETH_HLEN				14

#define ETH_P_IP				0x0800
#define ETH_P_8021Q				0x8100

#define VLAN_ETH_HLEN			18

#define VLAN_ETH_ALEN			6

struct pcap_file_header
{
	uint8_t magic[4];
	uint16_t version_major;
	uint16_t version_minor;
	int32_t thiszone;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t linktype;
};

struct pcap_pkthdr
{
	struct timeval ts;
	uint32_t caplen;
	uint32_t len;
};

struct vlan_eth_hdr
{
	uint8_t dest[VLAN_ETH_ALEN];
	uint8_t src[VLAN_ETH_ALEN];
	uint16_t proto;
	uint16_t tci;
	uint16_t encap_proto;
};

static int str2mac(const char *s, unsigned char mac_addr[VLAN_ETH_ALEN]);

static void usage(const char *progname);

int main(int argc, char **argv)
{
	int c, proto, head_skip, n, npacket, npacket_written, nbyte_written, link_hdrlen, skip;
	int test_src_mac_addr, test_dest_mac_addr;
	unsigned char src_mac_addr[VLAN_ETH_ALEN], dest_mac_addr[VLAN_ETH_ALEN];
	char *end;
	FILE *in, *out;
	struct pcap_file_header pcap_file_header;
	struct pcap_pkthdr pcap_pkthdr;
	unsigned char buf[PCAP_PKT_MAXSIZE];
	struct vlan_eth_hdr *vlan_eth_hdr;

	proto = -1;
	head_skip = 0;
	test_src_mac_addr = 0;
	test_dest_mac_addr = 0;

	while ((c = getopt(argc, argv, "p:h:s:d:")) != -1)
	{
		switch (c)
		{
			case 'p':
				proto = strtol(optarg, &end, 0);
				if (*end != '\0')
					usage(argv[0]);
			break;

			case 'h':
				head_skip = strtol(optarg, &end, 0);
				if (*end != '\0')
					usage(argv[0]);

				if (head_skip < 0)
					head_skip = 0;
			break;

			case 's':
				if (strlen(optarg) != (2 * VLAN_ETH_ALEN))
					usage(argv[0]);

				if (str2mac(optarg, src_mac_addr) != 0)
					usage(argv[0]);

				test_src_mac_addr = 1;
			break;

			case 'd':
				if (strlen(optarg) != (2 * VLAN_ETH_ALEN))
					usage(argv[0]);

				if (str2mac(optarg, dest_mac_addr) != 0)
					usage(argv[0]);

				test_dest_mac_addr = 1;
			break;

			case '?':
			default:
				fprintf(stderr, "unrecognized option: %c\n", c);
				usage(argv[0]);
		}
	}

	if (argc != (optind + 2))
		usage(argv[0]);

	in = fopen(argv[optind], "r");
	if (in == NULL)
	{
		fprintf(stderr, "couldn't open input file '%s'\n", argv[optind]);
		exit(1);
	}

	out = fopen(argv[optind + 1], "w");
	if (out == NULL)
	{
		fprintf(stderr, "couldn't open output file '%s'\n", argv[optind + 1]);
		exit(1);
	}

	n = fread(&pcap_file_header, 1, sizeof(struct pcap_file_header), in);
	if (n != sizeof(struct pcap_file_header))
	{
		fprintf(stderr, "couldn't read pcap file header\n");
		exit(1);
	}

	if ((pcap_file_header.magic[0] != 0xD4) ||
		(pcap_file_header.magic[1] != 0xC3) ||
		(pcap_file_header.magic[2] != 0xB2) ||
		(pcap_file_header.magic[3] != 0xA1))
	{
		fprintf(stderr, "input file is not a pcap file\n");
		exit(1);
	}

	if (pcap_file_header.linktype != LINKTYPE_EN10MB)
	{
		fprintf(stderr, "wrong pcap file linktype %d\n", pcap_file_header.linktype);
		exit(1);
	}

	npacket = 0;
	npacket_written = 0;
	nbyte_written = 0;

	for (;;)
	{
		npacket++;

		n = fread(&pcap_pkthdr, 1, sizeof(struct pcap_pkthdr), in);
		if (n == 0)
		{
			break;
		}
		else if (n != sizeof(struct pcap_pkthdr))
		{
			fprintf(stderr, "couldn't read pcap packet header #%d\n", npacket);
			exit(1);
		}

		n = fread(buf, 1, pcap_pkthdr.caplen, in);
		if (n != pcap_pkthdr.caplen)
		{
			fprintf(stderr, "couldn't read pcap packet #%d\n", npacket);
			exit(1);
		}

		vlan_eth_hdr = (struct vlan_eth_hdr *) buf;
		if (vlan_eth_hdr->proto == htons(ETH_P_8021Q))
		{
			if ((proto >= 0) && (proto <= 0xFFFF) &&
				(vlan_eth_hdr->encap_proto != htons(proto)))
				continue;

			link_hdrlen = VLAN_ETH_HLEN;
		}
		else
		{
			if ((proto >= 0) && (proto <= 0xFFFF) &&
				(vlan_eth_hdr->proto != htons(proto)))
				continue;

			link_hdrlen = ETH_HLEN;
		}

		if ((test_src_mac_addr != 0) &&
			(memcmp(src_mac_addr, vlan_eth_hdr->src, VLAN_ETH_ALEN) != 0))
			continue;

		if ((test_dest_mac_addr != 0) &&
			(memcmp(dest_mac_addr, vlan_eth_hdr->dest, VLAN_ETH_ALEN) != 0))
			continue;

		skip = link_hdrlen + head_skip;
		if (skip >= pcap_pkthdr.caplen)
			continue;

		n = fwrite(buf + skip, 1, pcap_pkthdr.caplen - skip, out);
		if (n != (pcap_pkthdr.caplen - skip))
		{
			fprintf(stderr, "couldn't write pcap packet #%d\n", npacket);
			exit(1);
		}

		npacket_written++;
		nbyte_written += (pcap_pkthdr.caplen - skip);
	}

	printf("total packets processed #%d\n", npacket);
	printf("total packets written #%d\n", npacket_written);
	printf("total bytes written #%d (0x%x)\n", nbyte_written, nbyte_written);

	fclose(in);
	fclose(out);

	exit(0);
}

static int str2mac(const char *s, unsigned char mac_addr[VLAN_ETH_ALEN])
{
	int i;
	unsigned char a, b;

	if (strlen(s) != (2 * VLAN_ETH_ALEN))
		return -1;

	for (i = 0; i < VLAN_ETH_ALEN; i++)
	{
		a = s[2 * i];
		b = s[2 * i + 1];

		if (!isxdigit(a) || !isxdigit(b))
			return -1;

		if (isalpha(a))
			a = toupper(a) - 'A' + 10;
		else
			a = a - '0';

		if (isalpha(b))
			b = toupper(b) - 'A' + 10;
		else
			b = b - '0';

		mac_addr[i] = (a << 4) | b;
	}

	return 0;
}

static void usage(const char *progname)
{
	fprintf(stderr, "usage: %s [-p <protocol>] [-h <head skip>] [-s <src mac address>] "
		"[-d <dst mac address>] <input filename> <output filename>\n", progname);
	exit(1);
}
