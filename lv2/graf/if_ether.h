
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

#ifndef _IF_ETHER_H_
#define _IF_ETHER_H_

#include <inttypes.h>

#define ETH_ALEN			6
#define ETH_HLEN			14
#define ETH_DATA_LEN		1500
#define ETH_FRAME_LEN		1518
#define ETH_FCS_LEN			4

#define ETH_P_IP			0x0800
#define ETH_P_8021Q			0x8100

struct eth_hdr
{
	u8 dest[ETH_ALEN];
	u8 src[ETH_ALEN];
	u16 proto;
};

#endif
