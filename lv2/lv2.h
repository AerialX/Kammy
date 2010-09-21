#pragma once

#include <sys.h>

typedef u64 size_t;

#define LV2_EXPORT extern __attribute__((longcall))

// string.h
LV2_EXPORT void* memcpy(void* dest, const void* src, size_t n);
LV2_EXPORT void* memset(void* s, int c, size_t n);
LV2_EXPORT int memcmp(void *v1, void *v2, size_t n);

LV2_EXPORT size_t strlen(const char* s);
LV2_EXPORT char* strcpy(char* dest, const char* src);
LV2_EXPORT int strncmp(const char* s1, const char* s2, size_t n);

// stdio.h
LV2_EXPORT int printf(char *format, ...);
LV2_EXPORT void* snprintf(char *str, size_t size, char *format, ...);

// lv2
LV2_EXPORT __attribute__((malloc)) void* alloc(size_t size, int unk);
LV2_EXPORT void dealloc(void* ptr, int unk);
LV2_EXPORT void panic(int unk);

// misc
static inline void* malloc(size_t size) { return alloc(size, 0x27); }
static inline void free(void* ptr) { dealloc(ptr, 0x27); }
