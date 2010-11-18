
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
#include <vuart.h>
#include <dispmgr.h>
#include <ss.h>
#include <vuart_dispmgr.h>

#define VUART_DISPMGR_BASE				0x80000000133A0000ULL
#define VUART_DISPMGR_OFFSET			0x133A0000ULL
#define VUART_DISPMGR_PAGE_SIZE			12
#define VUART_DISPMGR_SIZE				(1 << VUART_DISPMGR_PAGE_SIZE)

int vuart_dispmgr(void)
{
#define N(a)	(sizeof((a)) / sizeof((a)[0]))

	static u64 subject_id[][2] =
	{
		{ 0x1070000002000001, 0x10700003FF000001 },
		{ 0x1070000002000001, 0x10700005FC000001 },
		{ 0x1070000002000001, 0x10700003FD000001 },
		{ 0x1070000002000001, 0x1070000300000001 },
	};
	u64 vuart_lpar_addr, muid, lpar_addr, ea_addr, nread, nwritten, val;
	void *msgbuf;
	struct dispmgr_header *dispmgr_header;
	struct ss_header *ss_header;
	struct ss_update_mgr_get_package_info *ss_update_mgr_get_package_info;
	int i, result;

	result = lv1_allocate_memory(VUART_DISPMGR_SIZE, VUART_DISPMGR_PAGE_SIZE,
		0, 0, &vuart_lpar_addr, &muid);
 	if (result != 0)
 		return result;

	MM_LOAD_BASE(ea_addr, VUART_DISPMGR_OFFSET);
	lpar_addr = vuart_lpar_addr;

	for (i = 0; i < VUART_DISPMGR_SIZE >> 12; i++)
	{
		result = mm_insert_htab_entry(MM_EA2VA(ea_addr), lpar_addr, 0);
 		if (result != 0)
 			return result;

		ea_addr += (1 << 12);
		lpar_addr += (1 << 12);
	}

	MM_LOAD_BASE(msgbuf, VUART_DISPMGR_OFFSET);

	for (i = 0; i < N(subject_id); i++)
	{
		memset(msgbuf, 0, VUART_DISPMGR_SIZE);

		dispmgr_header = (struct dispmgr_header *) msgbuf;
		dispmgr_header->version = 1;
		dispmgr_header->function_id = 0x6000;
		dispmgr_header->request_size = 0x28;
		dispmgr_header->response_size = 0x100;

		ss_header = (struct ss_header *) (dispmgr_header + 1);
		memset(ss_header, 0, sizeof(struct ss_header));
		ss_header->packet_id = 0x6003;
		ss_header->function_id = 0x6000;
		ss_header->laid = subject_id[i][0];
		ss_header->paid = subject_id[i][1];

		ss_update_mgr_get_package_info =
			(struct ss_update_mgr_get_package_info *) (ss_header + 1);
		memset(ss_update_mgr_get_package_info, 0,
			sizeof(struct ss_update_mgr_get_package_info));
		ss_update_mgr_get_package_info->type = 1;

		dispmgr_header->request_size += sizeof(struct ss_update_mgr_get_package_info);

		result = lv1_write_virtual_uart(DISPMGR_VUART_PORT, vuart_lpar_addr,
			sizeof(struct dispmgr_header) + sizeof(struct ss_header) +
			sizeof(struct ss_update_mgr_get_package_info), &nwritten);
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

		for (;;)
		{
			result = lv1_read_virtual_uart(DISPMGR_VUART_PORT, vuart_lpar_addr, VUART_DISPMGR_SIZE,
				&nread);
			if (result < 0)
				return result;

			if (nread == 0)
				break;

			result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, msgbuf, nread);
			if (result < 0)
				return result;
		}
	}

	return 0;

#undef N
}
