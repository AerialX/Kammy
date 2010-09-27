#include "lv2.h"

#define __memprotcpy_bl 10
u32 __memprotcpy[] = {
	0x28230000,	//	cmpldi  r3,0
	0x4182003c,	//	beq-    40 <fail>
	0x28240000,	//	cmpldi  r4,0
	0x41820034,	//	beq-    40 <fail>
	0x7c601b78,	//	mr      r0,r3
	0x7c832378,	//	mr      r3,r4
	0x7c040378,	//	mr      r4,r0
	0xf821ff81,	//	stdu    r1,-128(r1)
	0x7c0802a6,	//	mflr    r0
	0xf8010090,	//	std     r0,144(r1)
	0x48000001, //	bl      memcpy : 0x28
	0xe8010090,	//	ld      r0,144(r1)
	0x38210080,	//	addi    r1,r1,128
	0x7c0803a6,	//	mtlr    r0
	0x38600000,	//	li      r3,0
	0x4e800020,	//	blr
	// fail:
	0x38600001,	//	li      r3,1
	0x4e800020	//	blr
};

#define __memprotcpycount 2
u8 __memprotcpybak[__memprotcpycount][sizeof(__memprotcpy)];
u64 __memprotcpyaddr[__memprotcpycount] = {
	0x800000000000e050,	// protmemcpy_chkdest
	0x800000000000e26c	// protmemcpy_chksrc
};

#define MEMCPY 0x800000000007c01c
void RemoveMemoryProtection()
{
	for (int i = 0; i < __memprotcpycount; i++) {
		memcpy(__memprotcpybak[i], (void*)__memprotcpyaddr[i], sizeof(__memprotcpy));
		__memprotcpy[__memprotcpy_bl] = 0x48000001 | (u32)(MEMCPY - (__memprotcpyaddr[i] + __memprotcpy_bl * 4));
		memcpy((void*)__memprotcpyaddr[i], __memprotcpy, sizeof(__memprotcpy));
	}
//	*(u32*)0x800000000006D834 = 0x4e800020; // Partially a problem
}
void RestoreMemoryProtection()
{
	for (int i = 0; i < __memprotcpycount; i++)
		memcpy((void*)__memprotcpyaddr[i], __memprotcpybak[i], sizeof(__memprotcpy));
}
