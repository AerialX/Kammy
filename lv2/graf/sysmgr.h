
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

#ifndef _SYSMGR_H_
#define _SYSMGR_H_

#include <inttypes.h>

#define SYSMGR_VUART_PORT						2

#define SYSMGR_SID_REQUEST						1
#define SYSMGR_SID_RESPONSE						2
#define SYSMGR_SID_CMD							3
#define SYSMGR_SID_EXT_EVENT					4
#define SYSMGR_SID_SET_NEXT_OP					5
#define SYSMGR_SID_SET_ATTR						8
#define SYSMGR_SID_CTRL_LED						12
#define SYSMGR_SID_RING_BUZZER					21
#define SYSMGR_SID_REQ_BE_COUNT					28
#define SYSMGR_SID_REQ_SC_VERSION				30
#define SYSMGR_SID_REQ_SYS_EVENT_LOG			32

#define SYSMGR_NEXT_OP_SYS_SHUTDOWN				1
#define SYSMGR_NEXT_OP_SYS_REBOOT				2
#define SYSMGR_NEXT_OP_LPAR_REBOOT				0x82

#define SYSMGR_WAKE_SRC_DEFAULT					0x00000000
#define SYSMGR_WAKE_SRC_WOL						0x00000400
#define SYSMGR_WAKE_SRC_POR						0x80000000

struct sysmgr_header
{
	u8 version;
	u8 size;
	u16 res1;
	u32 payload_size;
	u16 sid;
	u16 res2;
	u32 tag;
};

struct sysmgr_request
{
	u8 version;
	u8 type;
	u8 gos_id;
	u8 reserved[13];
};

struct sysmgr_set_next_op
{
	u8 version;
	u8 type;
	u8 gos_id;
	u8 res1;
	u32 wake_src;
	u8 res2[8];
};

struct sysmgr_set_attr
{
	u8 version;
	u8 res[3];
	u32 attr;
};

struct sysmgr_ctrl_led
{
	u8 field0;
	u8 field1;
	u8 field2;
	u8 res1;
	u8 field4;
	u8 field5;
	u8 res2[10];
};

struct sysmgr_ring_buzzer
{
	u8 res1;
	u8 field1;
	u8 field2;
	u8 res2;
	u32 field4;
};

struct sysmgr_req_sys_event_log
{
	u8 field0;
	u8 offset;
	u8 flag;
	u8 field3;
};

#endif
