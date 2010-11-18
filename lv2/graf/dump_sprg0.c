
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

#include <gelic.h>
#include <dump_sprg0.h>

int dump_sprg0(void)
{
	u64 sprg0;

	__asm__ __volatile__ ("mfsprg0 %0" : "=r"(sprg0));

	return gelic_xmit(gelic_bcast_mac_addr, 0xCAFE, &sprg0, 8);
}
