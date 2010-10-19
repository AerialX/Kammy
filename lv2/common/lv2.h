#pragma once

#include <psl1ght/lv2.h>

#ifdef LV2

typedef u64 size_t;

#define ALIGNED(x) __attribute__((aligned(x)))

#ifdef __cplusplus
#define LV2_EXPORT extern "C" __attribute__((longcall))
#else
#define LV2_EXPORT extern __attribute__((longcall))
#endif

// string.h
LV2_EXPORT void* memcpy(void* dest, const void* src, size_t n);
LV2_EXPORT void* memset(void* s, int c, size_t n);
LV2_EXPORT int memcmp(const void *v1, const void *v2, size_t n);

LV2_EXPORT char *strcat(char *destination, const char *source);
LV2_EXPORT char *strchr(const char* str, int chr);
LV2_EXPORT char* strcpy(char* dest, const char* src);
LV2_EXPORT char *strncpy(char *destination, const char *source, size_t num);
LV2_EXPORT size_t strlen(const char* s);
LV2_EXPORT int strcmp(const char *s1, const char *s2);
LV2_EXPORT int strncmp(const char* s1, const char* s2, size_t n);

// stdio.h
LV2_EXPORT int printf(const char *format, ...);
LV2_EXPORT int snprintf(char *str, long unsigned int size, const char *format, ...);
#include "lv2_stdio.h"

// lv2
LV2_EXPORT __attribute__((malloc)) void* alloc(size_t size, int unk);
LV2_EXPORT void dealloc(void* ptr, int unk);
LV2_EXPORT void panic(int unk);

// misc
#ifdef __cplusplus
extern "C" {
#endif
static inline void* malloc(size_t size) { return alloc(size, 0x27); }
static inline void free(void* ptr) { dealloc(ptr, 0x27); }
#ifdef __cplusplus
}
inline void* operator new(size_t size) { return malloc(size); }
inline void* operator new[](size_t size) { return malloc(size); }
inline void operator delete(void* ptr) { return free(ptr); }
inline void operator delete[](void* ptr) { return free(ptr); }
#endif

#endif
