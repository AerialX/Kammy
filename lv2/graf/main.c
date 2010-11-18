
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
#include <mm.h>
#include <param.h>
#include <gelic.h>
#include <dump_lv2.h>
#include <dump_slb.h>
#include <dump_sprg0.h>
#include <dump_htab.h>
#include <dump_stor.h>
#include <dump_flash.h>
#include <dump_repo_nodes.h>
#include <dump_profile.h>
#include <decrypt_profile.h>
#include <vuart_sysmgr.h>
#include <vuart_dispmgr.h>
#include <decrypt_usb_dongle_master_key.h>
#include <update_mgr_inspect_pkg.h>
#include <query_lpar_address.h>
#include <decrypt_pkg.h>

int start(void)
{
	int i, result;

	printf("mm_init()\n");
	result = mm_init();
	if (result != 0)
		goto error;

	printf("param_init()\n");
	result = param_init();
	if (result != 0)
		goto error;
	
	printf("gelic_init()\n");
	result = gelic_init();
	if (result != 0)
		goto error;

	printf("gelic_xmit_test()\n");
	result = gelic_xmit_test();
	if (result != 0)
		goto error;

	/*
	printf("gelic_recv_test()\n");
	result = gelic_recv_test();
	if (result != 0)
		goto error;
	*/

	/*
	printf("dump_lv2()\n");
	result = dump_lv2(0x8000000000000000ULL, 64 * 1024 * 1024);
	if (result != 0)
		goto error;
	*/

	/*
	printf("dump_slb()\n");
	result = dump_slb();
	if (result != 0)
		goto error;
	*/

	/*
	printf("dump_sprg0()\n");
	result = dump_sprg0();
	if (result != 0)
		goto error;
	*/

	/*
	printf("dump_htab()\n");
	result = dump_htab();
	if (result != 0)
		goto error;
	*/

	/*
	printf("dump_stor()\n");
	result = dump_stor();
	if (result != 0)
		goto error;
	*/

	printf("dump_flash()\n");
	result = dump_flash();
	if (result != 0)
		goto error;

	/*
	printf("dump_repo_nodes()\n");
	result = dump_repo_nodes();
	if (result != 0)
		goto error;
	*/

	/*
	printf("dump_repo_nodes_spu()\n");
	result = dump_repo_nodes_spu();
	if (result != 0)
		goto error;
	*/

	/*
	printf("dump_profile()\n");
	result = dump_profile();
	if (result != 0)
		goto error;
	*/

	/*
	printf("decrypt_profile()\n");
	result = decrypt_profile();
	if (result != 0)
		goto error;
	*/

	/*
	printf("vuart_sysmgr()\n");
	result = vuart_sysmgr();
	if (result != 0)
		goto error;
	*/

	/*
	printf("vuart_dispmgr()\n");
	result = vuart_dispmgr();
	if (result != 0)
		goto error;
	*/

	/*
	printf("decrypt_usb_dongle_master_key()\n");
	result = decrypt_usb_dongle_master_key();
	if (result != 0)
		goto error;
	*/

	/*
	printf("update_mgr_inspect_pkg()\n");
	result = update_mgr_inspect_pkg();
	if (result != 0)
		goto error;
	*/

	/*
	printf("query_lpar_address()\n");
	result = query_lpar_address();
	if (result != 0)
		goto error;
	*/

	/*
	printf("decrypt_pkg()\n");
	result = decrypt_pkg();
	if (result != 0)
		goto error;
	*/

	return 0;

error:
	return -1;
}
