#include <psl1ght/lv2.h>

#include <stdio.h>

#include "kammy.h"
#include "kammy_lv2.h"

#include "kammy.bin.h"

#define ROUND_UP(p, round) \
		((p + round - 1) & ~(round - 1))

// lv2 retail 3.41 addresses
#define LV2_ALLOC			(void*)0x8000000000062088ULL
#define KAMMY_RTOC_BASE		(void*)0x8000000000003000ULL
#define KAMMY_RTOC			0x800000000033E720ULL

// Some function that gets trashed by the payload already
#define KAMMY_HACK_BASE		0x800000000004ED28ULL

// Custom syscalls
#define SYSCALL_PEEK		6
#define SYSCALL_POKE		7

static void SyscallPoke(void* address, u64 value)
{
	Lv2Syscall2(SYSCALL_POKE, (u64)address, value);
}

static u64 SyscallPeek(void* address)
{
	return Lv2Syscall1(SYSCALL_PEEK, (u64)address);
}

bool Kammy_IsInitialised()
{
	return (Kammy_Version() & KAMMY_VERSION_MASK) == (KAMMY_VERSION & KAMMY_VERSION_MASK);
}

bool Kammy_Initialise()
{
	if (Kammy_IsInitialised())
		return true;

	const Lv2Module* kammy = Kammy_Load(kammy_bin);
	if (!kammy)
		return false;
	
	u64 ret;
	if (!kammy->ExecuteInternal(&ret))
		return false;
	return ret == KAMMY_VERSION;
}

const Lv2Module* Kammy_Load(const void* data)
{
	return (const Lv2Module*)data;
}

static void RelocateMemcpy(u64* dest, u64* data, u32 size, u64 base, void* newbase)
{
	for (u32 i = 0; i < size / 8; i++) {
		u64 value = data[i];
		if ((value & 0xFFFFFFFF00000000ULL) == base)
//		if (value - base < size)
			value = value - base + (u64)newbase;
		SyscallPoke(dest + i, value);
	}
}

bool Lv2Module::ExecuteInternal(u64* ret) const
{
	if (!Verify())
		return false;

	if (SyscallPeek(KAMMY_RTOC_BASE) != KAMMY_RTOC) {
		fprintf(stderr, "\tError! Kammy only works on retail 3.41.\n");
		return false;
	}
	
	// Uses poke to create a new alloc syscall //
	u64* addr = (u64*)SyscallPeek((u64*)LV2_SYSCALL_TABLE + KAMMY_SYSCALL);
	if (!addr || (u32)(u64)addr == 0x8001003) {
		fprintf(stderr, "\tError! Peek and poke not implemented.\n");
		return false;
	}
	SyscallPoke(addr, (u64)LV2_ALLOC);
	
	u32 size = ROUND_UP(GetDataSize(), 0x08);
	u64* address = (u64*)Lv2Syscall2(KAMMY_SYSCALL, size, 0x27);
	if (!address || (u32)(u64)address == 0x8001003)
		return false;
	RelocateMemcpy(address, (u64*)Data, size, TextBase, address);
	SyscallPoke((u64*)LV2_SYSCALL_TABLE + KAMMY_SYSCALL, (MainEntry - TextBase) + (u64)address);
	// HACK: The hypervisor doesn't obey the opd rtoc, so we have to pass it
	u64 value = Lv2Syscall1(KAMMY_SYSCALL, *(u64*)(Data + (MainEntry - TextBase) + 8) - TextBase + (u64)address);
	if (ret)
		*ret = value;
	return true;
}

bool Lv2Module::Execute(u64* ret, u64 param1, u64 param2, u64 param3, u64 param4, u64 param5, u64 param6) const
{
	if (!Verify())
		return false;
	u32 size = ROUND_UP(GetDataSize(), 0x08);
	void* address = Kammy_Alloc(size);
	if (!address)
		return false;
	RelocateMemcpy((u64*)address, (u64*)Data, size, TextBase, address);
	u64 value = Kammy_Execute((void*)((MainEntry - TextBase) + (u64)address), param1, param2, param3, param4, param5, param6);
	if (ret)
		*ret = value;
	return true;
}
