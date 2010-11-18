
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
#include <query_lpar_address.h>

#define QUERY_LPAR_ADDR_OFFSET			0x0033F658ULL

int query_lpar_address(void)
{
	u64 lpar_addr, start_address, size, access_right, max_page_size, flags;
	u64 entries[3][7];
	u64 *ptr;
	int i, result;

	MM_LOAD_BASE(ptr, QUERY_LPAR_ADDR_OFFSET);

	lpar_addr = 0x0000000000000000ULL;

	result = lv1_query_logical_partition_address_region_info(lpar_addr,
		&start_address, &size, &access_right, &max_page_size, &flags);

	i = 0;

	entries[i][0] = result;
	entries[i][1] = lpar_addr;
	entries[i][2] = start_address;
	entries[i][3] = size;
	entries[i][4] = access_right;
	entries[i][5] = max_page_size;
	entries[i][6] = flags;

	lpar_addr = *((u64 *) (*(u64 *) (*(u64 *) ptr)) + 0x11);

	result = lv1_query_logical_partition_address_region_info(lpar_addr,
		&start_address, &size, &access_right, &max_page_size, &flags);

	i++;

	entries[i][0] = result;
	entries[i][1] = lpar_addr;
	entries[i][2] = start_address;
	entries[i][3] = size;
	entries[i][4] = access_right;
	entries[i][5] = max_page_size;
	entries[i][6] = flags;

	lpar_addr = *((u64 *) (*(u64 *) (*(u64 *) ptr)) + 0x13);

	result = lv1_query_logical_partition_address_region_info(lpar_addr,
		&start_address, &size, &access_right, &max_page_size, &flags);

	i++;

	entries[i][0] = result;
	entries[i][1] = lpar_addr;
	entries[i][2] = start_address;
	entries[i][3] = size;
	entries[i][4] = access_right;
	entries[i][5] = max_page_size;
	entries[i][6] = flags;

	result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, entries, sizeof(entries));
	if (result < 0)
		return result;

	return 0;
}
