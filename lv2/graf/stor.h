
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

#ifndef _STOR_H_
#define _STOR_H_

#include <inttypes.h>

#define STOR_FLASH_DEV_ID			1
#define STOR_FLASH_BLOCK_SIZE		0x200

int stor_init(u64 dev_id);

int stor_read(u64 region_id, u64 start_sector, u64 num_sectors, u64 flags, void *data);

int stor_write(u64 region_id, u64 start_sector, u64 num_sectors, u64 flags, void *data);

#endif
