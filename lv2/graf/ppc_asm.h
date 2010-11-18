
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

#ifndef _PPC_ASM_H_
#define _PPC_ASM_H_

#define XGLUE(a,b)								a##b
#define GLUE(a,b)								XGLUE(a,b)

#define _GLOBAL(name)							\
	.section ".text";							\
	.align 2 ;									\
	.globl name;								\
	.type name,@function;						\
name:											\

#endif
