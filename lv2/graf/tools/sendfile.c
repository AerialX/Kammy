
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
#include <unistd.h>
#include <libnet.h>
#include <pcap.h>

#define ETH_ALEN			6

#define ETH_DATA_LEN		1500

#define VLAN_HLEN			4

#define VLAN_ETH_ALEN		6
#define VLAN_ETH_HLEN		18
#define VLAN_ETH_DATA_LEN	1500
#define VLAN_ETH_FRAME_LEN	1518

#define VLAN_VID_MASK		0x0FFF

#define PKT_MAGIC			0x0FACE0FF
#define PKT_HDR_SIZE		16
#define PKT_FLAG_LAST		0x00000001

struct vlan_hdr
{
	u_int16_t tci;
	u_int16_t encap_proto;
};

struct vlan_eth_hdr
{
	u_int8_t dest[VLAN_ETH_ALEN];
	u_int8_t src[VLAN_ETH_ALEN];
	u_int16_t proto;
	u_int16_t tci;
	u_int16_t encap_proto;
};

struct pkt_hdr
{
	u_int32_t magic;
	u_int32_t offset;
	u_int32_t size;
	u_int32_t flags;
};

static int str2mac(const char *s, unsigned char mac_addr[ETH_ALEN]);

static void usage(const char *progname);

int main(int argc, char **argv)
{
	int c, n, i, proto, packet_size, pause_us, retransmit, file_size, num_packets, npacket;
	char *end;
	libnet_t *ln_ctx;
	char ln_errbuf[LIBNET_ERRBUF_SIZE];
	struct libnet_ether_addr *ln_hwaddr;
	libnet_ptag_t ln_ptag;
	pcap_t *pcap_ctx;
	char pcap_errbuf[PCAP_ERRBUF_SIZE], pcap_fp_str[64];
	struct bpf_program pcap_fp;
	struct pcap_pkthdr pcap_hdr;
	FILE *fp;
	unsigned char buf[ETH_DATA_LEN], dest_mac_addr[ETH_ALEN];
	struct pkt_hdr *pkt_hdr;
	struct vlan_eth_hdr *vlan_eth_hdr;

	proto = 0xCAFE;
	packet_size = ETH_DATA_LEN - PKT_HDR_SIZE;
	pause_us = 1000;
	retransmit = 3;

	while ((c = getopt(argc, argv, "p:s:w:r:")) != -1)
	{
		switch (c)
		{
			case 'p':
				proto = strtol(optarg, &end, 0);
				if ((*end != '\0'))
					usage(argv[0]);
			break;

			case 's':
				packet_size = strtol(optarg, &end, 0);
				if ((*end != '\0'))
					usage(argv[0]);

				if ((packet_size <= 0) || (packet_size > (ETH_DATA_LEN - PKT_HDR_SIZE)))
					packet_size = ETH_DATA_LEN - PKT_HDR_SIZE;
			break;

			case 'w':
				pause_us = strtol(optarg, &end, 0);
				if ((*end != '\0'))
					usage(argv[0]);

				if (pause_us <= 0)
					pause_us = 1;
			break;

			case 'r':
				retransmit = strtol(optarg, &end, 0);
				if ((*end != '\0'))
					usage(argv[0]);

				if (retransmit < 0)
					retransmit = 0;
			break;

			case '?':
			default:
				fprintf(stderr, "unrecognized option: %c\n", c);
				usage(argv[0]);
		}
	}

	if (argc != (optind + 3))
		usage(argv[0]);

	if (strlen(argv[optind]) <= 0)
		usage(argv[0]);

	ln_ctx = libnet_init(LIBNET_LINK, argv[optind], ln_errbuf);
	if (ln_ctx == NULL)
	{
		fprintf (stderr, "couldn't initialize libnet context: %s\n", ln_errbuf);
		exit (1);
	}

	if (str2mac(argv[optind + 1], dest_mac_addr) != 0)
		usage(argv[0]);

	pcap_ctx = pcap_open_live(argv[optind], BUFSIZ, 1, 1000, pcap_errbuf);
	if (pcap_ctx == NULL)
	{
		fprintf (stderr, "couldn't initialize pcap context: %s\n", pcap_errbuf);
		exit (1);
	}

	sprintf(pcap_fp_str, "vlan and ether proto 0x%04x and ether src %02x:%02x:%02x:%02x:%02x:%02x",
		proto, dest_mac_addr[0], dest_mac_addr[1], dest_mac_addr[2], dest_mac_addr[3],
		dest_mac_addr[4], dest_mac_addr[5]);

	printf("pcap filter: %s\n", pcap_fp_str);

	if (pcap_compile(pcap_ctx, &pcap_fp, pcap_fp_str, 0, PCAP_NETMASK_UNKNOWN) == -1)
	{
		fprintf (stderr, "couldn't compile pcap filter: %s\n", pcap_geterr(pcap_ctx));
		exit (1);
	}

	if (pcap_setfilter(pcap_ctx, &pcap_fp) == -1)
	{
		fprintf (stderr, "couldn't set pcap filter: %s\n", pcap_geterr(pcap_ctx));
		exit (1);
	}

	fp = fopen(argv[optind + 2], "r");
	if (fp == NULL)
	{
		fprintf(stderr, "couldn't open file '%s'\n", argv[optind + 2]);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);

	file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	printf("file size #%d\n", file_size);

	if (file_size == 0)
	{
		printf("file is empty, terminating\n");
		exit(0);
	}

	ln_hwaddr = libnet_get_hwaddr(ln_ctx);

	ln_ptag = libnet_build_ethernet(dest_mac_addr, (u_int8_t *) ln_hwaddr,
		proto, NULL, 0, ln_ctx, 0);
	if (ln_ptag == -1)
	{
		fprintf (stderr, "couldn't create libnet packet: %s\n", libnet_geterror(ln_ctx));
		exit (1);
	}

	num_packets = (file_size + packet_size - 1) / packet_size;
	npacket = 0;

	while ((n = fread(buf + PKT_HDR_SIZE, 1, packet_size, fp)) > 0)
	{
		pkt_hdr = (struct pkt_hdr *) buf;
		memset(pkt_hdr, 0, PKT_HDR_SIZE);
		pkt_hdr->magic = htonl(PKT_MAGIC);
		pkt_hdr->offset = htonl(npacket * packet_size);
		pkt_hdr->size = htonl(n);
        if (n < packet_size)
		    pkt_hdr->flags |= PKT_FLAG_LAST;
		pkt_hdr->flags = htonl(pkt_hdr->flags);

		npacket++;

		if (libnet_build_ethernet(dest_mac_addr, (u_int8_t *) ln_hwaddr,
			proto, buf, n + PKT_HDR_SIZE, ln_ctx, ln_ptag) == -1)
		{
			fprintf (stderr, "couldn't modify libnet packet #%d: %s\n",
				npacket, libnet_geterror(ln_ctx));
			exit (1);
		}

		for (i = 0; i < retransmit + 1; i++)
		{
			printf("sending packet #%d of #%d\n", npacket, num_packets);

			if ((libnet_write(ln_ctx)) == -1)
			{
				fprintf (stderr, "couldn't send packet #%d: %s\n", npacket, libnet_geterror(ln_ctx));
				exit (1);
			}

			vlan_eth_hdr = (struct vlan_eth_hdr *) pcap_next(pcap_ctx, &pcap_hdr);
			if (vlan_eth_hdr == NULL)
			{
				fprintf (stderr, "timeout ack for packet #%d\n", npacket);
				continue;
			}

			if (pcap_hdr.len < (VLAN_ETH_HLEN + PKT_HDR_SIZE))
			{
				fprintf (stderr, "invalid ack for packet #%d\n", npacket);
				continue;
			}

			pkt_hdr = (struct pkt_hdr *) (vlan_eth_hdr + 1);
			pkt_hdr->offset = ntohl(pkt_hdr->offset);

			if (pkt_hdr->offset == ((npacket - 1) * packet_size + n))
			{
				printf("received ack for packet #%d\n", npacket);
				break;
			}
			else
			{
				fprintf (stderr, "bad ack for packet #%d, offset 0x%x\n",
					npacket, pkt_hdr->offset);
				continue;
			}
		}

		if (i == (retransmit + 1))
		{
			fprintf (stderr, "no ack received for packet #%d\n", npacket);
			exit (1);
		}

		usleep(pause_us);
	}

	fclose(fp);

	pcap_close(pcap_ctx);

	libnet_destroy(ln_ctx);

	exit(0);
}

static int str2mac(const char *s, unsigned char mac_addr[ETH_ALEN])
{
	int i;
	unsigned char a, b;

	if (strlen(s) != (2 * ETH_ALEN))
		return -1;

	for (i = 0; i < ETH_ALEN; i++)
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
	fprintf(stderr, "usage: %s [-p <proto>] [-s <packet size>] [-w <pause us>] "
		"[-r <max retransmits>] <interface> <dest mac address> <filename>\n", progname);
	exit(1);
}
