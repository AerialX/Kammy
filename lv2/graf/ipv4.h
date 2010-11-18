
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

#ifndef _IPV4_H_
#define _IPV4_H_

#define IPV4_VERSION				4

#define IPV4_MAX_TTL				255
#define IPV4_DEFAULT_TTL			64

struct ipv4_hdr
{
	u8 version:4;
	u8 ihl:4;
	u8 tos;
	u16 total_len;
	u16 id;
	u16 frag_off;
	u8 ttl;
	u8 proto;
	u16 cksum;
	u32 src;
	u32 dest;
};

#endif
