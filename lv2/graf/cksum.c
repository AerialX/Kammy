
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

#include <cksum.h>

u16 cksum(const void *data, int size)
{
	const u16 *ptr;
	long sum;

	ptr = data;
	sum = 0;

	while(size > 1)
	{
		sum += *ptr++;
		if(sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);

		size -= 2;
	}

	if (size)
		sum += (u16) *(u8 *) ptr;

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return ~sum;
}
