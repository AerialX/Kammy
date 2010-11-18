
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
#include <param.h>

#define PARAM_BASE			0x8000000013390000ULL
#define PARAM_OFFSET		0x13390000ULL

u64 param_lpar_addr;

u64 param_lpar_size;

u64 param_ea_addr;

int param_init(void)
{
	u64 *ptr;

	MM_LOAD_BASE(ptr, PARAM_OFFSET);

	/* These cause lv2 to panic, because the memory is unmapped?
	 * http://pastie.org/private/imivznhbjpnwubsm9vl6uw
	param_lpar_addr = ptr[0x2000 - 1];

	param_lpar_size = ptr[0x2000 - 2];

	param_ea_addr = ptr[0x2000 - 3];
	*/

	return 0;
}
