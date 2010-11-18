
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

#include <lv1call.h>
#include <inttypes.h>
#include <gelic.h>
#include <if_ether.h>
#include <memset.h>
#include <memcpy.h>
#include <repo_node.h>
#include <dump_repo_nodes.h>

int dump_repo_nodes(void)
{
#define N(a)	(sizeof((a)) / sizeof((a)[0]))

	static const u64 nodes[][4] =
	{
		/* ss.laid */
		{ 0x0000000073730000, 0x6C61696400000000, 0x0000000000000000, 0x0000000000000000 },
		/* sys.lv1.iso_enbl */
		{ 0x0000000073797300, 0x6C76310000000000, 0x69736F5F656E626C, 0x0000000000000000 },
		/* sys.flash.fmt */
		{ 0x0000000073797300, 0x666C617368000000, 0x666D740000000000, 0x0000000000000000 },
		/* sys.flash.boot */
		{ 0x0000000073797300, 0x666C617368000000, 0x626F6F7400000000, 0x0000000000000000 },
		/* bus1.num_dev */
		{ 0x0000000062757301, 0x6E756D5F64657600, 0x0000000000000000, 0x0000000000000000 },
		/* bus4.num_dev */
		{ 0x0000000062757304, 0x6E756D5F64657600, 0x0000000000000000, 0x0000000000000000 },
		/* bus4.dev1.region0.size */
		{ 0x0000000062757304, 0x6465760000000001, 0x726567696F6E0000, 0x73697A6500000000 },
		/* bus4.dev2.region0.size */
		{ 0x0000000062757304, 0x6465760000000002, 0x726567696F6E0000, 0x73697A6500000000 },
		/* bus4.dev4.region0.size */
		{ 0x0000000062757304, 0x6465760000000004, 0x726567696F6E0000, 0x73697A6500000000 },
		/* bus4.dev0.blk_size */
		{ 0x0000000062757304, 0x6465760000000000, 0x626C6B5F73697A65, 0x0000000000000000 },
		/* bus4.dev4.blk_size */
		{ 0x0000000062757304, 0x6465760000000004, 0x626C6B5F73697A65, 0x0000000000000000 },
		/* ss.param.qa.flag */
		{ 0x0000000073730000, 0x706172616D000000, 0x7161000000000000, 0x666C616700000000 },
	};
	u64 entries[N(nodes)][7], v1, v2;
	int i, result;

	for (i = 0; i < N(nodes); i++)
	{
		result = lv1_get_repository_node_value(REPO_NODE_LPAR_ID_PME, nodes[i][0], nodes[i][1],
			nodes[i][2], nodes[i][3], &v1, &v2);

		entries[i][0] = result;
		memcpy(&entries[i][1], &nodes[i][0], 4 * 8);
		entries[i][5] = v1;
		entries[i][6] = v2;
	}

	return gelic_xmit(gelic_bcast_mac_addr, 0xCAFE, entries, sizeof(entries));

#undef N
}

int dump_repo_nodes_spu(void)
{
#define N(a)	(sizeof((a)) / sizeof((a)[0]))

	static const u64 nodes[][5] =
	{
		/* bi.spursvn */
		{ 0x0000000062690000, 0x7370757273766E00, 0x0000000000000000, 0x0000000000000000 },
		/* bi.spursv.0 */
		{ 0x0000000062690000, 0x7370757273760000, 0x0000000000000000, 0x0000000000000000 },
		/* bi.spursv.1 */
		{ 0x0000000062690000, 0x7370757273760000, 0x0000000000000001, 0x0000000000000000 },
		/* bi.spursv.2 */
		{ 0x0000000062690000, 0x7370757273760000, 0x0000000000000002, 0x0000000000000000 },
		/* bi.spursv.3 */
		{ 0x0000000062690000, 0x7370757273760000, 0x0000000000000003, 0x0000000000000000 },
		/* bi.spursv.4 */
		{ 0x0000000062690000, 0x7370757273760000, 0x0000000000000004, 0x0000000000000000 },
		/* bi.spursv.5 */
		{ 0x0000000062690000, 0x7370757273760000, 0x0000000000000005, 0x0000000000000000 },
		/* bi.spursv.6 */
		{ 0x0000000062690000, 0x7370757273760000, 0x0000000000000006, 0x0000000000000000 },
		/* bi.spursv.7 */
		{ 0x0000000062690000, 0x7370757273760000, 0x0000000000000007, 0x0000000000000000 },
	};
	u64 entries[N(nodes)][7], v1, v2;
	int i, result;

	for (i = 0; i < N(nodes); i++)
	{
		result = lv1_get_repository_node_value(2, nodes[i][0], nodes[i][1],
			nodes[i][2], nodes[i][3], &v1, &v2);

		entries[i][0] = result;
		memcpy(&entries[i][1], &nodes[i][0], 4 * 8);
		entries[i][5] = v1;
		entries[i][6] = v2;
	}

	return gelic_xmit(gelic_bcast_mac_addr, 0xCAFE, entries, sizeof(entries));

#undef N
}
