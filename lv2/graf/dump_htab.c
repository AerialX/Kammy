
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
#include <inttypes.h>
#include <mm.h>
#include <gelic.h>
#include <dump_htab.h>

#define DUMP_HTAB_BASE				0x80000000133A0000ULL
#define DUMP_HTAB_OFFSET			0x133A0000ULL
#define DUMP_HTAB_PAGE_SIZE			12
#define DUMP_HTAB_SIZE				(1024 * 1024)

int dump_htab(void)
{
	u64 htab_lpar_addr, lpar_addr, ea_addr;
	void *htab;
	int i, result;

	/*
	result = lv1_map_htab(0, &htab_lpar_addr);
	if (result != 0)
		return result;

	MM_LOAD_BASE(ea_addr, DUMP_HTAB_OFFSET);
	lpar_addr = htab_lpar_addr;

	for (i = 0; i < DUMP_HTAB_SIZE >> DUMP_HTAB_PAGE_SIZE; i++)
	{
		result = mm_insert_htab_entry(MM_EA2VA(ea_addr), lpar_addr, 3);
 		if (result != 0)
 			return result;

		ea_addr += (1 << DUMP_HTAB_PAGE_SIZE);
		lpar_addr += (1 << DUMP_HTAB_PAGE_SIZE);
	}

	MM_LOAD_BASE(htab, DUMP_HTAB_OFFSET);

	result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, htab, DUMP_HTAB_SIZE);
	if (result < 0)
		return result;
	*/

	MM_LOAD_BASE(htab, 0xF000000ULL);

	result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, htab, 0x40000);
	if (result < 0)
		return result;

	return 0;
}
