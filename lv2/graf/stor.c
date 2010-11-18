
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
#include <memcpy.h>
#include <stor.h>

#define STOR_BUS_ID								4

#define STOR_DMA_BASE							0x80000000133A0000ULL
#define STOR_DMA_OFFSET							0x133A0000ULL
#define STOR_DMA_PAGE_SIZE						12
#define STOR_DMA_SIZE							(1 << STOR_DMA_PAGE_SIZE)

struct stor_var
{
	u64 dev_id;
	u64 block_size;
	u64 dma_lpar_addr;
	u32 dma_bus_addr;
};

static struct stor_var stor_var;

int stor_init(u64 dev_id)
{
	u64 dma_lpar_addr, muid, dma_bus_addr;
	int result;

	result = lv1_allocate_memory(STOR_DMA_SIZE, STOR_DMA_PAGE_SIZE, 0, 0,
		&dma_lpar_addr, &muid);
	if (result != 0)
		return result;

	result = lv1_allocate_device_dma_region(STOR_BUS_ID, dev_id,
		STOR_DMA_SIZE, STOR_DMA_PAGE_SIZE, 0, &dma_bus_addr);
	if (result != 0)
		return result;

	result = lv1_map_device_dma_region(STOR_BUS_ID, dev_id, dma_lpar_addr, dma_bus_addr,
		STOR_DMA_SIZE, 0xF800000000000000ULL);
	if (result != 0)
		return result;

	result = mm_insert_htab_entry(MM_EA2VA(STOR_DMA_BASE), dma_lpar_addr, 0);
	if (result != 0)
		return result;

	stor_var.dev_id = dev_id;
	stor_var.dma_lpar_addr = dma_lpar_addr;
	stor_var.dma_bus_addr = dma_bus_addr;
	stor_var.block_size = 0x200;

	return 0;
}

int stor_read(u64 region_id, u64 start_sector, u64 num_sectors, u64 flags, void *data)
{
	u64 tag, status;
	u8 *buf;
	int result;

	result = lv1_storage_read(stor_var.dev_id, region_id, start_sector, num_sectors,
		flags, stor_var.dma_lpar_addr, &tag);
	if (result != 0)
		return result;

	for (;;)
	{
		result = lv1_storage_check_async_status(stor_var.dev_id, tag, &status);
		if (result != 0)
			continue;

		if (status == 0)
			break;
	}

	MM_LOAD_BASE(buf, STOR_DMA_OFFSET);

	memcpy(data, buf, stor_var.block_size * num_sectors);

	return 0;
}

int stor_write(u64 region_id, u64 start_sector, u64 num_sectors, u64 flags, void *data)
{
	return 0;
}
