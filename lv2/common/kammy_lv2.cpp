#include "kammy_lv2.h"

#ifndef KAMMY

u64 Kammy_Version()
{
	return Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_VERSION);
}

KAMMY_PTR Kammy_Alloc(u64 size)
{
	return (KAMMY_PTR)Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_ALLOC, size);
}

void Kammy_Dealloc(KAMMY_PTR addr)
{
	Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_DEALLOC, (u64)addr);
}

u64 Kammy_Execute(KAMMY_PTR addr, u64 param1, u64 param2, u64 param3, u64 param4, u64 param5, u64 param6)
{
	return Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_EXEC, (u64)addr, param1, param2, param3, param4, param5, param6);
}

void Kammy_Memcpy(KAMMY_PTR dest, KAMMY_PTR src, u64 size)
{
	Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_MEMCPY, (u64)dest, (u64)src, size);
}

void Kammy_HookSyscall(u32 syscall, KAMMY_PTR func)
{
	Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_HOOK_SYSCALL, syscall, (u64)func);
}

void Kammy_HookBranch(KAMMY_PTR branch, KAMMY_PTR func, bool lr)
{
	Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_HOOK_BRANCH, (u64)branch, (u64)func, lr);
}

s64 Kammy_HookSubcall(u64 subcall, KAMMY_PTR func)
{
	return Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_HOOK_SUBCALL, subcall, (u64)func);
}

u64 Kammy_CallSubcall(u64 subcall, u64 param1, u64 param2, u64 param3, u64 param4, u64 param5, u64 param6)
{
	return Lv2Syscall(KAMMY_SYSCALL, KAMMY_SUBCALL_CALL_SUBCALL, subcall, param1, param2, param3, param4, param5, param6);
}

#endif
