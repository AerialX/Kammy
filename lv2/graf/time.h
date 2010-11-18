
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

#ifndef _TIME_H_
#define _TIME_H_

#define TB_TICKS_PER_SEC				79800000

static inline unsigned long long get_tb_ticks(void)
{
	unsigned long long ticks;

	__asm__ __volatile__ ("mftb %0" : "=r"(ticks));

	return ticks;
}

void sleep(unsigned long long ticks);

#endif
