
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

#include <mm.h>
#include <gelic.h>
#include <memcpy.h>
#include <dump_lv2.h>

#define DUMP_LV2_BASE			0x80000000133A0000ULL
#define DUMP_LV2_OFFSET			0x133A0000ULL
#define DUMP_LV2_PAGE_SIZE		12
#define DUMP_LV2_SIZE			(1 << DUMP_LV2_PAGE_SIZE)

int dump_lv2(u64 start_addr, u64 size)
{
	u64 lpar_addr, muid, addr;
	u8 *buf;
 	int result;

	result = lv1_allocate_memory(DUMP_LV2_SIZE, DUMP_LV2_PAGE_SIZE, 0, 0,
		&lpar_addr, &muid);
 	if (result != 0)
 		return result;

 	result = mm_insert_htab_entry(MM_EA2VA(DUMP_LV2_BASE), lpar_addr, 0);
 	if (result != 0)
 		return result;

	MM_LOAD_BASE(buf, DUMP_LV2_OFFSET);

	for (addr = start_addr; addr < start_addr + size; addr += 1024)
	{
		memcpy(buf, &addr, 8);
		memcpy(buf + 8, (u64 *) addr, 1024);

		result = gelic_xmit(gelic_bcast_mac_addr, 0xCAFE, buf, 8 + 1024);
		if (result != 0)
			return result;
	}

	return 0;
}
