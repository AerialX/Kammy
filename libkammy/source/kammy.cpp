#include "kammy.h"

// lv2 retail 3.41 addresses
#define LV2_ALLOC			0x8000000000062088ULL
#define KAMMY_RTOC_BASE		0x8000000000332B58ULL

// Some function that gets trashed by the payload already
#define KAMMY_HACK_BASE		0x800000000004ED28ULL

// Custom syscall map
#define KAMMY_SYSCALL_ALLOC	8
#define KAMMY_SYSCALL_EXEC	9
#define SYSCALL_PEEK		6
#define SYSCALL_POKE		7

static void SyscallPoke(u64 address, u64 value)
{
	Lv2Syscall(SYSCALL_POKE, address, value);
}

static u64 SyscallPeek(u64 address)
{
	return Lv2Syscall(SYSCALL_PEEK, address);
}

static u64 SyscallAlloc(u64 size)
{
	return Lv2Syscall(KAMMY_SYSCALL_ALLOC, size);
}

bool Kammy_Init()
{
	// Uses poke to create a new alloc syscall //
	// .opd entry for syscall
	SyscallPoke(KAMMY_HACK_BASE, KAMMY_HACK_BASE + 8 * 3);
	SyscallPoke(KAMMY_HACK_BASE + 8, SyscallPeek(KAMMY_RTOC_BASE));
	SyscallPoke(KAMMY_HACK_BASE + 8 * 2, 0);
	// Function itself
	u64 offset = KAMMY_HACK_BASE + 8 * 3 + 4;
	u64 value = 0x38800027ULL << 32; // li %r4, 0x27
	value |= 0x48000000 | (LV2_ALLOC - offset); // b alloc
	SyscallPoke(KAMMY_HACK_BASE + 8 * 3, value);
	// Syscall table entry
	SyscallPoke(SYSCALL_TABLE + 8 * KAMMY_SYSCALL_ALLOC, KAMMY_HACK_BASE);

	// Verify that something got through
	return SyscallPeek(SYSCALL_TABLE + 8 * KAMMY_SYSCALL_ALLOC) == KAMMY_HACK_BASE;
}

const Lv2Module* Kammy_Load(const void* data)
{
	return (const Lv2Module*)data;
}

static void RelocateMemcpy(u64 dest, u64* data, u32 size, u64 base, u64 newbase)
{
	for (u32 i = 0; i < size / 8; i++) {
		u64 value = data[i];
		if ((value & 0xFFFFFFFF00000000ULL) == base)
//		if (value - base < size)
			value = value - base + newbase;
		SyscallPoke(dest + i * 8, value);
	}
}

bool Lv2Module::Execute(u64* ret) const
{
	if (!Verify())
		return false;
	u32 size = ROUND_UP(GetDataSize(), 0x08);
	u64 address = SyscallAlloc(size);
	if (!address)
		return false;
	RelocateMemcpy(address, (u64*)Data, size, TextBase, address);
	SyscallPoke(SYSCALL_TABLE + 8 * KAMMY_SYSCALL_EXEC, MainEntry - TextBase + address);
	// HACK: The hypervisor does obey the opd rtoc, so we have to pass it.
	u64 value = Lv2Syscall(KAMMY_SYSCALL_EXEC, *(u64*)(Data + MainEntry - TextBase + 8) - TextBase + address);
	if (ret)
		*ret = value;
	return true;
}
