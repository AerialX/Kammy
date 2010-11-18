
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
#include <sysmgr.h>
#include <vuart_sysmgr.h>

#define VUART_SYSMGR_BASE				0x80000000133A0000ULL
#define VUART_SYSMGR_OFFSET				0x133A0000ULL
#define VUART_SYSMGR_PAGE_SIZE			12
#define VUART_SYSMGR_SIZE				(1 << VUART_SYSMGR_PAGE_SIZE)

int vuart_sysmgr(void)
{
	u64 lpar_addr, muid, nread, nwritten, val;
	void *msgbuf;
	struct sysmgr_header *header;
	struct sysmgr_ctrl_led *ctrl_led;
	struct sysmgr_ring_buzzer *ring_buzzer;
	int result;

	result = lv1_allocate_memory(VUART_SYSMGR_SIZE, VUART_SYSMGR_PAGE_SIZE,
		0, 0, &lpar_addr, &muid);
 	if (result != 0)
 		return result;

	result = mm_insert_htab_entry(MM_EA2VA(VUART_SYSMGR_BASE), lpar_addr, 0);
 	if (result != 0)
 		return result;

	MM_LOAD_BASE(msgbuf, VUART_SYSMGR_OFFSET);

	header = (struct sysmgr_header *) msgbuf;
	memset(header, 0, sizeof(struct sysmgr_header));
	header->version = 1;
	header->size = 16;
	header->payload_size = 16;
	header->sid = SYSMGR_SID_CTRL_LED;

	ctrl_led = (struct sysmgr_ctrl_led *) (header + 1);
	memset(ctrl_led, 0, sizeof(struct sysmgr_ctrl_led));
	ctrl_led->field0 = 1;
	ctrl_led->field1 = 0;
	ctrl_led->field2 = 0xFF;
	ctrl_led->field4 = 0xFF;
	ctrl_led->field5 = 0xFF;

	result = lv1_write_virtual_uart(SYSMGR_VUART_PORT,
		lpar_addr, sizeof(struct sysmgr_header) + sizeof(struct sysmgr_ctrl_led), &nwritten);
	if (result < 0)
		return result;

	header = (struct sysmgr_header *) msgbuf;
	memset(header, 0, sizeof(struct sysmgr_header));
	header->version = 1;
	header->size = 16;
	header->payload_size = 8;
	header->sid = SYSMGR_SID_RING_BUZZER;

	ring_buzzer = (struct sysmgr_ring_buzzer *) (header + 1);
	memset(ring_buzzer, 0, sizeof(struct sysmgr_ring_buzzer));
	ring_buzzer->field1 = 0x29;
	ring_buzzer->field2 = 0x4;
	ring_buzzer->field4 = 0x6;

	result = lv1_write_virtual_uart(SYSMGR_VUART_PORT,
		lpar_addr, sizeof(struct sysmgr_header) + sizeof(struct sysmgr_ring_buzzer), &nwritten);
	if (result < 0)
		return result;

	for (;;)
	{
		result = lv1_get_virtual_uart_param(SYSMGR_VUART_PORT, VUART_PARAM_RX_BYTES, &val);
		if (result < 0)
			return result;

		if (val == 0)
			continue;

		result = lv1_read_virtual_uart(SYSMGR_VUART_PORT, lpar_addr, VUART_SYSMGR_SIZE, &nread);
		if (result < 0)
			return result;

		result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, msgbuf, nread);
		if (result < 0)
			return result;
	}

	return 0;
}
