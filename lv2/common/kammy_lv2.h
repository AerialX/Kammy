#pragma once

#include "lv2.h"

#define KAMMY_VERSION		0x4A33100000000001ULL
#define KAMMY_VERSION_MASK	0xFFFFFFFF00000000ULL
#define KAMMY_SYSCALL		11

#ifdef LV2
#define KAMMY_PTR void*
#else
#define KAMMY_PTR u64
#endif

enum {
	KAMMY_ERROR_OK = 0,
	KAMMY_ERROR = -0x100,
	KAMMY_ERROR_UNIMPLEMENTED,
	KAMMY_ERROR_INVALID
};

u64 Kammy_Version();
KAMMY_PTR Kammy_Alloc(u64 size);
void Kammy_Dealloc(KAMMY_PTR addr);
u64 Kammy_Execute(KAMMY_PTR addr, u64 param1 = 0, u64 param2 = 0, u64 param3 = 0, u64 param4 = 0, u64 param5 = 0, u64 param6 = 0);
void Kammy_Memcpy(KAMMY_PTR dest, KAMMY_PTR src, u64 size);
void Kammy_HookSyscall(u32 syscall, KAMMY_PTR func);
void Kammy_HookBranch(KAMMY_PTR branch, KAMMY_PTR func, bool lr);

s64 Kammy_HookSubcall(u64 subcall, KAMMY_PTR func);
u64 Kammy_CallSubcall(u64 subcall, u64 param1 = 0, u64 param2 = 0, u64 param3 = 0, u64 param4 = 0, u64 param5 = 0, u64 param6 = 0);
s64 Kammy_GetModules(KAMMY_PTR* buffer = NULL, u64* size = 0);
s64 Kammy_UnloadModule(KAMMY_PTR base);
#ifdef KAMMY
s64 Kammy_RegisterModule(KAMMY_PTR base);
#else
s64 Kammy_RegisterModule();
#endif

enum {
	KAMMY_SUBCALL_VERSION = 0,
	KAMMY_SUBCALL_ALLOC,
	KAMMY_SUBCALL_DEALLOC,
	KAMMY_SUBCALL_EXEC,
	KAMMY_SUBCALL_MEMCPY,
	KAMMY_SUBCALL_HOOK_SYSCALL,
	KAMMY_SUBCALL_HOOK_BRANCH,
	KAMMY_SUBCALL_HOOK_SUBCALL,
	KAMMY_SUBCALL_CALL_SUBCALL,
	KAMMY_SUBCALL_GET_MODULES,
	KAMMY_SUBCALL_UNLOAD_MODULE,
	KAMMY_SUBCALL_REGISTER_MODULE,
	KAMMY_SUBCALL_MAX
};

