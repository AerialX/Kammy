#pragma once

#include "lv2.h"

// PPC emitter http://pastebin.com/wrUBnMKT
static inline u32 ppc_addi(u32 D,u32 A,u32 SIMM) {
	return 0x38000000 | ((0x1f & (u32)D)<<21) | ((0x1f & (u32)A)<<16) | ((0xffff & (u32)SIMM)<<0);
}
static inline u32 ppc_li(u32 D, u32 SIMM) {
	return ppc_addi(D, 0, SIMM);
}
static inline u32 ppc_ori(u32 A,u32 S,u32 UIMM) {
	return 0x60000000 | ((0x1f & (u32)A)<<16) | ((0x1f & (u32)S)<<21) | ((0xffff & (u32)UIMM)<<0);
}
static inline u32 ppc_oris(u32 A,u32 S,u32 UIMM) {
	return 0x64000000 | ((0x1f & (u32)A)<<16) | ((0x1f & (u32)S)<<21) | ((0xffff & (u32)UIMM)<<0);
}
static inline u32 ppc_rldcl(u32 A, u32 S, u32 B, u32 MB) {
	return 0x78000010 | ((0x1F & A) << 16) | ((0x1F & S) << 21) | ((0x1f & B) << 11) | ((0x1f & MB) << 5);
}
static inline u32 ppc_rotld(u32 A, u32 S, u32 B) {
	return ppc_rldcl(A, S, B, 0);
}
static inline u32 ppc_mtspr(u32 spr,u32 S) {
	return 0x7c0003a6 | ((0x3ff & (u32)spr)<<11) | ((0x1f & (u32)S)<<21);
}
static inline u32 ppc_mtctr(u32 S) {
	return ppc_mtspr(9, S);
}
static inline u32 ppc_bctr() {
	return 0x4e800420;
}


static void Kammy_HookSyscall(int syscall, void* ptr)
{
	u64* stub64 = (u64*)malloc(0x50);
	u32* stub32 = (u32*)(stub64 + 3);
	stub64[0] = (u64)stub32;
	stub64[1] = ((u64*)ptr)[1];
	stub64[2] = ((u64*)ptr)[2];
#define stub_load(idx, reg, value) \
	stub32[idx + 0] = ppc_li(reg, value >> 48); \
	stub32[idx + 1] = ppc_ori(reg, reg, value >> 32); \
	stub32[idx + 2] = ppc_rotld(reg, reg, 32); \
	stub32[idx + 3] = ppc_oris(reg, reg, value >> 16); \
	stub32[idx + 4] = ppc_ori(reg, reg, value);
	stub_load(0, 2, stub64[1]);
	stub_load(5, 0, (u64)ptr);
	stub32[10] = ppc_mtctr(0);
	stub32[11] = ppc_bctr();
	((u64*)SYSCALL_TABLE)[syscall] = (u64)stub64;
}

