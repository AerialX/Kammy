
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

#include <lv1call.h>
#include <mm.h>
#include <gelic.h>
#include <memset.h>
#include <memcpy.h>
#include <vuart.h>
#include <dispmgr.h>
#include <ss.h>
#include <dump_profile.h>

#define DUMP_PROFILE_BASE				0x80000000133A0000ULL
#define DUMP_PROFILE_OFFSET				0x133A0000ULL
#define DUMP_PROFILE_PAGE_SIZE			12
#define DUMP_PROFILE_SIZE				(1 << DUMP_PROFILE_PAGE_SIZE)

int dump_profile(void)
{
#define N(a)	(sizeof((a)) / sizeof((a)[0]))

	static u64 subject_id[][2] =
	{
		{ 0x1070000002000001, 0x10700003FF000001 },
		{ 0x1070000002000001, 0x10700003FF000001 },
		{ 0x1070000002000001, 0x10700003FF000001 },
	};
	u64 lpar_addr, muid, nread, nwritten, val;
	void *msgbuf;
	struct dispmgr_header *dispmgr_header;
	struct ss_header *ss_header;
	struct ss_secure_profile_loader_get *ss_secure_profile_loader_get;
	int i, result;

	result = lv1_allocate_memory(DUMP_PROFILE_SIZE, DUMP_PROFILE_PAGE_SIZE,
		0, 0, &lpar_addr, &muid);
 	if (result != 0)
 		return result;

	result = mm_insert_htab_entry(MM_EA2VA(DUMP_PROFILE_BASE), lpar_addr, 0);
 	if (result != 0)
 		return result;

	MM_LOAD_BASE(msgbuf, DUMP_PROFILE_OFFSET);

	for (i = 0; i < N(subject_id); i++)
	{
		memset(msgbuf, 0, DUMP_PROFILE_SIZE);

		dispmgr_header = (struct dispmgr_header *) msgbuf;
		dispmgr_header->version = 1;
		dispmgr_header->function_id = 0x15000;
		dispmgr_header->request_size = 0x28;
		dispmgr_header->response_size = 0x400;

		ss_header = (struct ss_header *) (dispmgr_header + 1);
		ss_header->packet_id = 0x15003;
		ss_header->function_id = 0x15000;
		ss_header->laid = subject_id[i][0];
		ss_header->paid = subject_id[i][1];

		ss_secure_profile_loader_get =
			(struct ss_secure_profile_loader_get *) (ss_header + 1);
		memcpy(ss_secure_profile_loader_get->profile_name, "DEFAULT.SPP", 11);
		ss_secure_profile_loader_get->contents_type = 0xA;
		memcpy(ss_secure_profile_loader_get->contents_name, "SCE_CELLOS_SYSTEM_MGR", 21);
		ss_secure_profile_loader_get->contents_size = 8;
		*(u64 *) ((u8 *) (ss_secure_profile_loader_get + 1) +
			ss_secure_profile_loader_get->contents_size + 8) =
				SS_SECURE_PROFILE_LOADER_GET_CONTENTS_SIZE;

		dispmgr_header->request_size += sizeof(struct ss_secure_profile_loader_get) +
			ss_secure_profile_loader_get->contents_size + 16;

		result = lv1_write_virtual_uart(DISPMGR_VUART_PORT, lpar_addr,
			sizeof(struct dispmgr_header) + sizeof(struct ss_header) +
			sizeof(struct ss_secure_profile_loader_get) +
			ss_secure_profile_loader_get->contents_size + 16, &nwritten);
		if (result < 0)
			return result;

		for (;;)
		{
			result = lv1_get_virtual_uart_param(DISPMGR_VUART_PORT, VUART_PARAM_RX_BYTES, &val);
			if (result < 0)
				return result;

			if (val != 0)
				break;
		}

		result = lv1_read_virtual_uart(DISPMGR_VUART_PORT, lpar_addr, DUMP_PROFILE_SIZE, &nread);
		if (result < 0)
			return result;

		result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, msgbuf, nread);
		if (result < 0)
			return result;
	}

	return 0;

#undef N
}
