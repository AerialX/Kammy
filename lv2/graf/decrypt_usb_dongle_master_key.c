
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
#include <decrypt_usb_dongle_master_key.h>

#define DECRYPT_USB_DONGLE_MASTER_KEY_BASE			0x80000000133A0000ULL
#define DECRYPT_USB_DONGLE_MASTER_KEY_OFFSET		0x133A0000ULL
#define DECRYPT_USB_DONGLE_MASTER_KEY_PAGE_SIZE		12
#define DECRYPT_USB_DONGLE_MASTER_KEY_SIZE			(1 << DECRYPT_USB_DONGLE_MASTER_KEY_PAGE_SIZE)

int decrypt_usb_dongle_master_key(void)
{
#define N(a)	(sizeof((a)) / sizeof((a)[0]))

	static u64 subject_id[][2] =
	{
		{ 0x1070000001000001, 0x1070000045000001 },
		{ 0x1070000002000001, 0x1070000001000002 },
		{ 0x1070000002000001, 0x1070000002000002 },
		{ 0x1070000002000001, 0x1070000003000002 },
		{ 0x1070000002000001, 0x1070000004000002 },
		{ 0x1070000002000001, 0x10700005FF000001 },
		{ 0x1070000002000001, 0x107000004E000001 },
		{ 0x1070000002000001, 0x1070000051000001 },
	};
	static u8 encrypted_usb_dongle_master_key[64] =
	{
		0x22, 0xD5, 0xD1, 0x8C, 0xFF, 0xE2, 0x4F, 0xAC, 0xEC, 0x72, 0xA2,
		0x42, 0xA7, 0x18, 0x98, 0x10, 0x25, 0x33, 0xE0, 0x96, 0xF2, 0xC1,
		0x91, 0x0D, 0x15, 0x23, 0xD3, 0x07, 0x74, 0xE7, 0x2B, 0x72, 0xDF,
		0xA6, 0xDD, 0xE9, 0x68, 0x8B, 0x76, 0x2A, 0x6A, 0x87, 0x51, 0x7F,
		0x85, 0x39, 0x0B, 0xD4, 0x20, 0x3F, 0x46, 0x89, 0x04, 0x82, 0xB7,
		0x30, 0x84, 0x89, 0x4B, 0xCC, 0x9D, 0xB1, 0x24, 0x7C
	};
	u64 vuart_lpar_addr, muid, lpar_addr, ea_addr, nread, nwritten, val;
	void *msgbuf;
	struct dispmgr_header *dispmgr_header;
	struct ss_header *ss_header;
	struct ss_virtual_trm_mgr_decrypt_master *ss_virtual_trm_mgr_decrypt_master;
	int i, result;

	result = lv1_allocate_memory(DECRYPT_USB_DONGLE_MASTER_KEY_SIZE,
		DECRYPT_USB_DONGLE_MASTER_KEY_PAGE_SIZE, 0, 0, &vuart_lpar_addr, &muid);
 	if (result != 0)
 		return result;

	MM_LOAD_BASE(ea_addr, DECRYPT_USB_DONGLE_MASTER_KEY_OFFSET);
	lpar_addr = vuart_lpar_addr;

	for (i = 0; i < DECRYPT_USB_DONGLE_MASTER_KEY_SIZE >> DECRYPT_USB_DONGLE_MASTER_KEY_PAGE_SIZE; i++)
	{
		result = mm_insert_htab_entry(MM_EA2VA(ea_addr), lpar_addr, 0);
 		if (result != 0)
 			return result;

		ea_addr += (1 << DECRYPT_USB_DONGLE_MASTER_KEY_PAGE_SIZE);
		lpar_addr += (1 << DECRYPT_USB_DONGLE_MASTER_KEY_PAGE_SIZE);
	}

	MM_LOAD_BASE(msgbuf, DECRYPT_USB_DONGLE_MASTER_KEY_OFFSET);

	for (i = 0; i < N(subject_id); i++)
	{
		memset(msgbuf, 0, DECRYPT_USB_DONGLE_MASTER_KEY_SIZE);

		dispmgr_header = (struct dispmgr_header *) msgbuf;
		dispmgr_header->version = 1;
		dispmgr_header->function_id = 0x2000;
		dispmgr_header->request_size = 0x28;
		dispmgr_header->response_size = 0x100;

		ss_header = (struct ss_header *) (dispmgr_header + 1);
		memset(ss_header, 0, sizeof(struct ss_header));
		ss_header->packet_id = 0x200E;
		ss_header->function_id = 0x2000;
		ss_header->laid = subject_id[i][0];
		ss_header->paid = subject_id[i][1];

		ss_virtual_trm_mgr_decrypt_master =
			(struct ss_virtual_trm_mgr_decrypt_master *) (ss_header + 1);
		memset(ss_virtual_trm_mgr_decrypt_master, 0,
			sizeof(struct ss_virtual_trm_mgr_decrypt_master));
		memcpy(ss_virtual_trm_mgr_decrypt_master->seed, "_USB_DONGLE_AUTH", 16);
		memcpy(ss_virtual_trm_mgr_decrypt_master->msg, encrypted_usb_dongle_master_key, 64);

		dispmgr_header->request_size += sizeof(struct ss_virtual_trm_mgr_decrypt_master);

		result = lv1_write_virtual_uart(DISPMGR_VUART_PORT, vuart_lpar_addr,
			sizeof(struct dispmgr_header) + sizeof(struct ss_header) +
			sizeof(struct ss_virtual_trm_mgr_decrypt_master), &nwritten);
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
				DECRYPT_USB_DONGLE_MASTER_KEY_SIZE, &nread);
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
