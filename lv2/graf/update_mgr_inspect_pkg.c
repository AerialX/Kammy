
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
#include <repo_node.h>
#include <dispmgr.h>
#include <ss.h>
#include <update_mgr_inspect_pkg.h>

#define UPDATE_MGR_INSPECT_PKG_BASE				0x80000000133A0000ULL
#define UPDATE_MGR_INSPECT_PKG_OFFSET			0x133A0000ULL
#define UPDATE_MGR_INSPECT_PKG_PAGE_SIZE		12
#define UPDATE_MGR_INSPECT_PKG_SIZE				(1 << UPDATE_MGR_INSPECT_PKG_PAGE_SIZE)

#define PKG_BASE								0x80000000133B0000ULL
#define PKG_OFFSET								0x133B0000ULL
#define PKG_PAGE_SIZE							12
#define PKG_SIZE								(16 << PKG_PAGE_SIZE)

static int update_mgr_inspect_pkg_dump_status(u64 reques_id);

int update_mgr_inspect_pkg(void)
{
#define N(a)	(sizeof((a)) / sizeof((a)[0]))

	static const u64 subject_id[][2] =
	{
		{ 0x1070000002000001, 0x10700003FF000001 },
		{ 0x1070000002000001, 0x0000000000000000 },
	};
	u64 vuart_lpar_addr, pkg_lpar_addr, muid, lpar_addr, ea_addr, nread, nwritten, val;
	u64 reques_id;
	void *msgbuf, *pkgbuf;
	struct dispmgr_header *dispmgr_header;
	struct ss_header *ss_header;
	struct ss_update_mgr_inspect_pkg *ss_update_mgr_inspect_pkg;
	int i, pkg_size, result;

	result = lv1_allocate_memory(UPDATE_MGR_INSPECT_PKG_SIZE, UPDATE_MGR_INSPECT_PKG_PAGE_SIZE,
		0, 0, &vuart_lpar_addr, &muid);
 	if (result != 0)
 		return result;

	MM_LOAD_BASE(ea_addr, UPDATE_MGR_INSPECT_PKG_OFFSET);
	lpar_addr = vuart_lpar_addr;

	for (i = 0; i < UPDATE_MGR_INSPECT_PKG_SIZE >> 12; i++)
	{
		result = mm_insert_htab_entry(MM_EA2VA(ea_addr), lpar_addr, 0);
 		if (result != 0)
 			return result;

		ea_addr += (1 << 12);
		lpar_addr += (1 << 12);
	}

	result = lv1_allocate_memory(PKG_SIZE, PKG_PAGE_SIZE, 0, 0, &pkg_lpar_addr, &muid);
 	if (result != 0)
 		return result;

	MM_LOAD_BASE(ea_addr, PKG_OFFSET);
	lpar_addr = pkg_lpar_addr;

	for (i = 0; i < PKG_SIZE >> 12; i++)
	{
		result = mm_insert_htab_entry(MM_EA2VA(ea_addr), lpar_addr, 0);
 		if (result != 0)
 			return result;

		ea_addr += (1 << 12);
		lpar_addr += (1 << 12);
	}

	MM_LOAD_BASE(pkgbuf, PKG_OFFSET);

	pkg_size = gelic_recv_data(pkgbuf, PKG_SIZE);
	if (pkg_size <= 0)
		return pkg_size;

	MM_LOAD_BASE(msgbuf, UPDATE_MGR_INSPECT_PKG_OFFSET);

	for (i = 0; i < N(subject_id); i++)
	{
		memset(msgbuf, 0, UPDATE_MGR_INSPECT_PKG_SIZE);

		dispmgr_header = (struct dispmgr_header *) msgbuf;
		dispmgr_header->version = 1;
		dispmgr_header->function_id = 0x6000;
		dispmgr_header->request_size = 0x28;
		dispmgr_header->response_size = 0x100;

		ss_header = (struct ss_header *) (dispmgr_header + 1);
		ss_header->packet_id = 0x6002;
		ss_header->function_id = 0x6000;
		ss_header->laid = subject_id[i][0];
		ss_header->paid = subject_id[i][1];

		ss_update_mgr_inspect_pkg =
			(struct ss_update_mgr_inspect_pkg *) (ss_header + 1);
		ss_update_mgr_inspect_pkg->field0 = 1;
		ss_update_mgr_inspect_pkg->pkg_type = 9;
		ss_update_mgr_inspect_pkg->field10 = 0x9;
		ss_update_mgr_inspect_pkg->lpar_id = 2;
		ss_update_mgr_inspect_pkg->pkg_size = 1;
		*(u64 *) ss_update_mgr_inspect_pkg->pkg_data = pkg_lpar_addr;
		*(u64 *) (ss_update_mgr_inspect_pkg->pkg_data + 8) = pkg_size;

		dispmgr_header->request_size += sizeof(struct ss_update_mgr_inspect_pkg) + 0x20;

		result = lv1_write_virtual_uart(DISPMGR_VUART_PORT, vuart_lpar_addr,
			sizeof(struct dispmgr_header) + dispmgr_header->request_size, &nwritten);
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
			result = lv1_read_virtual_uart(DISPMGR_VUART_PORT, vuart_lpar_addr,
				UPDATE_MGR_INSPECT_PKG_SIZE, &nread);
			if (result != 0)
				return result;

			if (nread == 0)
				break;

			reques_id = *(u64 *) (ss_update_mgr_inspect_pkg->pkg_data + 0x18);

			result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, msgbuf, nread);
			if (result < 0)
				return result;

			for (;;)
			{
				update_mgr_inspect_pkg_dump_status(reques_id);

				lv1_pause(0);
			}
		}
	}

	return 0;

#undef N
}

static int update_mgr_inspect_pkg_dump_status(u64 reques_id)
{
	u64 entries[7], v1, v2;
	int i, result;

	entries[0] = lv1_get_repository_node_value(REPO_NODE_LPAR_ID_PME,
		repo_node_key_ss_inspect_request[0], repo_node_key_ss_inspect_request[1],
		repo_node_key_ss_inspect_request[2], reques_id,
		&entries[5], &entries[6]);

	memcpy(&entries[1], repo_node_key_ss_inspect_request, 4 * 8);

	return gelic_xmit(gelic_bcast_mac_addr, 0xCAFE, entries, sizeof(entries));
}
