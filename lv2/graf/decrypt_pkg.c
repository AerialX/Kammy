
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
#include <mm.h>
#include <gelic.h>
#include <vas.h>
#include <if_ether.h>
#include <memcpy.h>
#include <system.h>
#include <spu.h>
#include <rvk_pkg_341.h>
#include <decrypt_pkg.h>

#define SPU_BASE					0x8000000014000000ULL
#define SPU_OFFSET					0x14000000ULL
#define SPU_PAGE_SIZE				16
#define SPU_SIZE					0x1000000
#define SPU_MODULE_SIZE				(1 << SPU_PAGE_SIZE)
#define SPU_ARG_SIZE				(SPU_SIZE - SPU_MODULE_SIZE)

#define SPU_SHADOW_BASE				0x80000000133D0000ULL
#define SPU_SHADOW_OFFSET			0x133D0000ULL
#define SPU_SHADOW_SIZE				0x1000

#define SPU_PRIV2_BASE				0x80000000133E0000ULL
#define SPU_PRIV2_OFFSET			0x133E0000ULL
#define SPU_PRIV2_SIZE				0x20000

struct spu_args
{
	u64 type;
	u64 flags;
	u8 *src_addr;
	u64 src_size;
	u8 *dst_addr;
	u64 dst_size;
	u8 *rvk_src_addr;
	u64 rvk_src_size;
	u8 *rvk_dst_addr;
	u64 rvk_dst_size;
};

static volatile u64 spu_lpar_addr = 0x700020000000ULL + 0xE900000ULL - SPU_SIZE;

static volatile u64 paid = 0x1050000003000001;
static volatile u64 esid = 0x8000000018000000;
static volatile u64 vsid = 0x0000000000001400;

static struct spu_args __attribute__ ((aligned ((128)))) spu_args;

int decrypt_pkg(void)
{
	u64 hdr_size, encrypted_size;
	u64 priv2_addr, problem_phys, local_store_phys, unused, shadow_addr, spe_id, intr_status;
	u8 *spu_module, *spu_arg;
	struct spu_shadow *spu_shadow;
	struct spu_priv2 *spu_priv2;
	volatile u64 dummy;
 	int spu_module_size, spu_arg_size, i, result;

	MM_LOAD_BASE(spu_module, SPU_OFFSET);

	result = mm_map_lpar_memory_region(spu_lpar_addr, (u64) spu_module, SPU_SIZE, 0xC, 0);
	if (result != 0)
		return result;

	spu_module_size = gelic_recv_data(spu_module, SPU_MODULE_SIZE);
	if (spu_module_size <= 0)
		return spu_module_size;

	spu_arg = (u8 *) spu_module + SPU_MODULE_SIZE;

	spu_arg_size = gelic_recv_data(spu_arg, SPU_ARG_SIZE);
	if (spu_arg_size <= 0)
		return spu_arg_size;

	result = lv1_construct_logical_spe(0xC, 0xC, 0xC, 0xC, 0xC, vas_get_id(), 2,
		&priv2_addr, &problem_phys, &local_store_phys, &unused, &shadow_addr, &spe_id);
	if (result != 0)
		return result;

	MM_LOAD_BASE(spu_shadow, SPU_SHADOW_OFFSET);

	result = mm_map_lpar_memory_region(shadow_addr, (u64) spu_shadow, SPU_SHADOW_SIZE, 0xC, 3);
	if (result != 0)
		return result;

	hdr_size = *(u64 *) (spu_arg + 0x10);
	encrypted_size = *(u64 *) (spu_arg + 0x18);

	memset(&spu_args, 0, sizeof(struct spu_args));
	//spu_args.type = *(u32 *) (spu_arg + hdr_size + 4) + 1;
	spu_args.type = 1;
	spu_args.flags = 5;
	spu_args.src_addr = spu_arg;
	spu_args.src_size = (hdr_size + encrypted_size);
	spu_args.dst_addr = spu_arg + hdr_size;
	spu_args.dst_size = encrypted_size;
	spu_args.rvk_src_addr = rvk_pkg_341;
	spu_args.rvk_src_size = sizeof(rvk_pkg_341);
	spu_args.rvk_dst_addr = rvk_pkg_341;
	spu_args.rvk_dst_size = sizeof(rvk_pkg_341);

	result = lv1_undocumented_function_209(spe_id, paid, (u64) spu_module,
		(u64) &spu_args, sizeof(spu_args), 0, 0, 6);
	if (result != 0)
		return result;

	result = lv1_undocumented_function_62(spe_id, 0, 0, esid, vsid);
	if (result != 0)
		return result;

	result = lv1_clear_spe_interrupt_status(spe_id, 1, intr_status, 0);
	if (result != 0)
		return result;

	result = lv1_undocumented_function_168(spe_id, 0x3000, 1ULL << 32);
	if (result != 0)
		return result;

	while (1)
	{
		result = lv1_get_spe_interrupt_status(spe_id, 1, &intr_status);
		if (result != 0)
			return result;

		if (intr_status)
		{
			result = lv1_undocumented_function_62(spe_id, 0, 0, esid, vsid);
			if (result != 0)
				return result;

			result = lv1_clear_spe_interrupt_status(spe_id, 1, intr_status, 0);
			if (result != 0)
				return result;

			result = lv1_undocumented_function_168(spe_id, 0x3000, 1ULL << 32);
			if (result != 0)
				return result;
		}

		if (spu_shadow->execution_status == 0x7)
		{
			MM_LOAD_BASE(spu_priv2, SPU_PRIV2_OFFSET);

			result = mm_map_lpar_memory_region(priv2_addr, (u64) spu_priv2, SPU_PRIV2_SIZE, 0xC, 3);
			if (result != 0)
				return result;

			result = lv1_get_spe_interrupt_status(spe_id, 2, &intr_status);
			if (result != 0)
				return result;

			dummy = spu_priv2->spu_out_intr_mbox;

			result = lv1_undocumented_function_167(spe_id, 0x4000, &dummy);
			if (result != 0)
				return result;

			result = lv1_clear_spe_interrupt_status(spe_id, 2, intr_status, 0);
			if (result != 0)
				return result;

			result = lv1_undocumented_function_200(spe_id);
			if (result != 0)
				return result;
		}

		if ((spu_shadow->execution_status == 0x3) ||
			(spu_shadow->execution_status == 0xB))
			break;
	}

	lv1_pause(0);

	result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, spu_shadow, SPU_SHADOW_SIZE);
	if (result != 0)
		return result;

	result = gelic_xmit_data(gelic_bcast_mac_addr, 0xCAFE, spu_arg, hdr_size + encrypted_size);
	if (result != 0)
		return result;

	lv1_panic(1);

    return 0;
}
