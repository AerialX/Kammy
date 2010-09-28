#pragma once

#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

typedef int8_t				s8;
typedef uint8_t				u8;
typedef int16_t				s16;
typedef uint16_t			u16;
typedef int32_t				s32;
typedef uint32_t			u32;
typedef int64_t				s64;
typedef uint64_t			u64;
typedef volatile int8_t		vs8;
typedef volatile uint8_t	vu8;
typedef volatile int16_t	vs16;
typedef volatile uint16_t	vu16;
typedef volatile int32_t	vs32;
typedef volatile uint32_t	vu32;
typedef volatile int64_t	vs64;
typedef volatile uint64_t	vu64;

// lv2 retail 3.41
#define SYSCALL_TABLE		0x80000000002EB128ULL

#define ROUND_UP(p, round) \
		((p + round - 1) & ~(round - 1))

#ifdef __cplusplus
static inline u64 Lv2Syscall(u64 syscall,
		u64 param1 = 0, u64 param2 = 0,
		u64 param3 = 0, u64 param4 = 0,
		u64 param5 = 0, u64 param6 = 0,
		u64 param7 = 0, u64 param8 = 0)
#else
static inline u64 Lv2Syscall(u64 syscall,
		u64 param1, u64 param2,
		u64 param3, u64 param4,
		u64 param5, u64 param6,
		u64 param7, u64 param8)
#endif
{
#ifdef LV2 // Within lv2, we have to branch to the entry directly.
	u64 (*syscallopd)(u64, u64, u64, u64, u64, u64, u64, u64) = (u64 (*)(u64, u64, u64, u64, u64, u64, u64, u64))*(u64*)(SYSCALL_TABLE + syscall * 8);
	return syscallopd(param1, param2, param3, param4, param5, param6, param7, param8);
#else
	register u64 p1 asm("3") = param1;
	register u64 p2 asm("4") = param2;
	register u64 p3 asm("5") = param3;
	register u64 p4 asm("6") = param4;
	register u64 p5 asm("7") = param5;
	register u64 p6 asm("8") = param6;
	register u64 p7 asm("9") = param7;
	register u64 p8 asm("10") = param8;
	register u64 n  __asm__ ("11") = syscall;
	asm volatile("sc"
		: "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),
		  "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(n)
		: "r"(p1), "r"(p2), "r"(p3), "r"(p4),
		  "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(n)
		: "0", "2", "12", "lr", "ctr", "xer", "cr0", "cr1", "cr5", "cr6", "cr7", "memory");
	return p1;
#endif
}

