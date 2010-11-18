
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

#ifndef _IF_VLAN_H_
#define _IF_VLAN_H_

#include <inttypes.h>

#define VLAN_HLEN				4

#define VLAN_ETH_ALEN			6
#define VLAN_ETH_HLEN			18
#define VLAN_ETH_DATA_LEN		1500
#define VLAN_ETH_FRAME_LEN		1518

#define VLAN_VID_MASK			0x0FFF

struct vlan_hdr
{
	u16 tci;
	u16 encap_proto;
};

struct vlan_eth_hdr
{
	u8 dest[VLAN_ETH_ALEN];
	u8 src[VLAN_ETH_ALEN];
	u16 proto;
	u16 tci;
	u16 encap_proto;
};

#endif
