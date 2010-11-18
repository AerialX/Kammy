
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

#ifndef _GELIC_H_
#define _GELIC_H_

/* Kammy Modification: This define causes gelic to use printf to send
 * data using lv2 tty (like Kammy's ethdebug) rather than actually using
 * the gelic ethernet interface. Why? Because I like using ethdebug and
 * the gelic code doesn't support PS3 Slim systems.
 */
#define GELIC_TTY

#include <if_vlan.h>

extern const u8 gelic_bcast_mac_addr[VLAN_ETH_ALEN];

int gelic_init(void);

int gelic_xmit(const u8 dest_mac_addr[VLAN_ETH_ALEN], u16 proto, const void *data, u64 size);

int gelic_recv(void *data, u64 max_size);

int gelic_xmit_data(const u8 dest_mac_addr[VLAN_ETH_ALEN], u16 proto, const void *data, u64 size);

int gelic_recv_data(const void *data, u64 max_size);

int gelic_xmit_test(void);

int gelic_recv_test(void);

#endif
