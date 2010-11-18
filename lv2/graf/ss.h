
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

#ifndef _SS_H_
#define _SS_H_

#include <inttypes.h>

#define SS_FUNCTION_ID_VIRTUAL_TRM_MGR						0x2000
#define SS_FUNCTION_ID_SECURE_RTC							0x3000
#define SS_FUNCTION_ID_STORAGE_MGR							0x5000
#define SS_FUNCTION_ID_UPDATE_MGR							0x6000
#define SS_FUNCTION_ID_SECURE_PROFILE_LOADER				0x15000
#define SS_FUNCTION_ID_INDI_INFO_MGR						0x17000
#define SS_FUNCTION_ID_AIM									0x19000
#define SS_FUNCTION_ID_USB_DONGLE_AUTH						0x24000
#define SS_FUNCTION_ID_USER_TOKEN_MGR						0x25000

struct ss_header
{
	u64 packet_id;
	u64 function_id;
	u32 retval;
	u8 res[4];
	u64 laid;
	u64 paid;
};

struct ss_update_mgr_read_eprom
{
	u32 offset;
	u8 res[4];
	u8 value;
};

struct ss_update_mgr_write_eprom
{
	u32 offset;
	u8 res[4];
	u8 value;
};

struct ss_update_mgr_get_applicable_version
{
	u32 field0;
};

struct ss_update_mgr_get_package_info
{
	u32 type;
	u8 res[4];
	u64 version;
};

struct ss_update_mgr_get_token_seed
{
	u64 token_size;
	u8 token[80];
	u64 seed_size;
	u8 seed[80];
};

struct ss_update_mgr_set_token
{
	u64 token_size;
	u8 token[80];
};

struct ss_update_mgr_inspect_pkg
{
	u32 field0;
	u8 res1[4];
	u32 pkg_type;
	u8 res2[4];
	u32 field10;
	u8 res3[4];
	u64 lpar_id;
	u64 pkg_size;
	u8 pkg_data[0];
	/* u64 reques_id */
};

#define SS_SECURE_PROFILE_LOADER_GET_CONTENTS_SIZE			0x10000
#define SS_SECURE_PROFILE_LOADER_GET_CONTENTS				0x20000

struct ss_secure_profile_loader_get
{
	u8 profile_name[256];
	u32 contents_type;
	u8 res[4];
	u8 contents_name[32];
	u64 contents_size;
	u8 contents[0];
	/* u64 fieldX; */
	/* u64 request_type; */
};

struct ss_indi_info_mgr_read_eid0_size
{
	u64 field0;
	u64 field8;
};

struct ss_indi_info_mgr_read_eid0
{
	u64 field0;
	u64 field8;
	u8 field10[0];
	/* u64 fieldX */;
};

struct ss_virtual_trm_mgr_decrypt_master
{
	u8 seed[16];
	u8 msg[64];
};

#endif
