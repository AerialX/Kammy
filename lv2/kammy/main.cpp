#include "lv2.h"
#include "kammy_lv2.h"

typedef u64 (*Subcall)(u64, u64, u64, u64, u64, u64, u64);
Subcall SubcallTable[] = {
	(Subcall)Kammy_Version,
	(Subcall)Kammy_Alloc,
	(Subcall)Kammy_Dealloc,
	(Subcall)Kammy_Execute,
	(Subcall)Kammy_Memcpy,
	(Subcall)Kammy_HookSyscall,
	(Subcall)Kammy_HookBranch,
	(Subcall)Kammy_HookSubcall,
	(Subcall)Kammy_CallSubcall,
	(Subcall)Kammy_GetModules,
	(Subcall)Kammy_UnloadModule,
	(Subcall)Kammy_RegisterModule
};

u64 Kammy_Handler(u64 subcall, u64 param1, u64 param2, u64 param3, u64 param4, u64 param5, u64 param6, u64 param7)
{
	if (subcall >= KAMMY_SUBCALL_MAX)
		return KAMMY_ERROR_INVALID;
	return SubcallTable[subcall](param1, param2, param3, param4, param5, param6, param7);
}

u64 Kammy_Version()
{
	return KAMMY_VERSION;
}

#define KAMMY_ALLOC_UNK 0x27
void* Kammy_Alloc(u64 size)
{
	return alloc(size, KAMMY_ALLOC_UNK);
}
void Kammy_Dealloc(void* addr)
{
	dealloc(addr, KAMMY_ALLOC_UNK);
}

void Kammy_Memcpy(void* dest, void* src, u64 size)
{
	memcpy(dest, src, size);
}

u64 Kammy_Execute(void* addr, u64 param1, u64 param2, u64 param3, u64 param4, u64 param5, u64 param6)
{
	return ((Subcall)addr)(param1, param2, param3, param4, param5, param6, 0);
}

void RemoveMemoryProtection();
void __Init_Module();

extern "C" u64 start()
{
	RemoveMemoryProtection();
	Kammy_HookSyscall(KAMMY_SYSCALL, (void*)Kammy_Handler);
	__Init_Module();
	return Kammy_Version();
}

