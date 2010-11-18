
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

#ifndef _SPU_H_
#define _SPU_H_

#include <inttypes.h>

struct spu_shadow
{
	u8 padding_0x0000_0x0140[0x0140 - 0x0000];
	u64 int_status_class0;
	u64 int_status_class1;
	u64 int_status_class2;
	u8 padding_0x0158_0x0610[0x0610 - 0x0158];
	u64 mfc_dsisr;
	u8 padding_0x0618_0x0620[0x0620 - 0x0618];
	u64 mfc_dar;
	u8 padding_0x0628_0x0800[0x0800 - 0x0628];
	u64 mfc_dsipr;
	u8 padding_0x0808_0x0810[0x0810 - 0x0808];
	u64 mfc_lscrr;
	u8 padding_0x0818_0x0C00[0x0C00 - 0x0818];
	u64 mfc_cer;
	u8 padding_0x0C08_0x0F00[0x0F00 - 0x0C08];
	u64 execution_status;
	u8 padding_0x0F08_0x1000[0x1000 - 0x0F08];
};

struct spu_priv2
{
	u8 padding_0x00000_0x04000[0x04000 - 0x00000];
	u64 spu_out_intr_mbox;
	u8 padding_0x04008_0x20000[0x20000 - 0x04008];
};

#endif
