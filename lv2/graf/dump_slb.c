
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

#include <inttypes.h>
#include <gelic.h>
#include <dump_slb.h>

#define SLB_SIZE    64

int dump_slb(void)
{
	u64 entries[2 * SLB_SIZE], e, v;
	int i;

	for (i = 0; i < SLB_SIZE; i += 2)
	{
		__asm__ __volatile__ ("slbmfee %0, %1" : "=r"(e) : "r"(i));
		__asm__ __volatile__ ("slbmfev %0, %1" : "=r"(v) : "r"(i));

		entries[i] = e;
		entries[i + 1] = v;
	}

	return gelic_xmit(gelic_bcast_mac_addr, 0xCAFE, entries, sizeof(entries));
}
