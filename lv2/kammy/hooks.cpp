#include "kammy_lv2.h"

extern u32 Kammy_Stub;
extern u32 Kammy_Stub_Address;
extern u32 Kammy_Stub_End;
static void __Kammy_CreateHook(u32* stub32, u64 func)
{
	memcpy(stub32, &Kammy_Stub, (&Kammy_Stub_End - &Kammy_Stub) * 4);
	stub32[0] = Kammy_Stub;
	stub32 += &Kammy_Stub_Address - &Kammy_Stub;
	stub32[0] |= ((func >> 48) & 0xFFFF);
	stub32[1] |= ((func >> 32) & 0xFFFF);
	stub32[3] |= ((func >> 16) & 0xFFFF);
	stub32[4] |= (func & 0xFFFF);
}

void Kammy_HookSyscall(u32 syscall, void* ptr)
{
	u64* func = (u64*)ptr;
	u64* stub64 = (u64*)malloc(0x80);
	u32* stub32 = (u32*)(stub64 + 4);
	stub64[0] = (u64)stub32;
	stub64[1] = func[1];
	stub64[2] = func[2];
	
	__Kammy_CreateHook(stub32, (u64)ptr);
	
	((u64*)SYSCALL_TABLE)[syscall] = (u64)stub64;
}

void Kammy_HookBranch(void* branch, void* ptr, bool lr)
{
	u32* stub32 = (u32*)malloc(0x60);
	__Kammy_CreateHook(stub32, (u64)ptr);

	*(u32*)branch = 0x48000000 | ((u64)stub32 - (u64)branch) | (lr ? 1 : 0);
}

