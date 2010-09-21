#pragma once

#include <sys.h>

typedef u64 size_t;

#ifdef __cplusplus
#define LV2_EXPORT extern "C" __attribute__((longcall))
#else
#define LV2_EXPORT extern __attribute__((longcall))
#endif

// string.h
LV2_EXPORT void* memcpy(void* dest, const void* src, size_t n);
LV2_EXPORT void* memset(void* s, int c, size_t n);
LV2_EXPORT int memcmp(void *v1, void *v2, size_t n);

LV2_EXPORT char *strcat(char *destination, const char *source);
LV2_EXPORT char *strchr(const char* str, char chr);
LV2_EXPORT char* strcpy(char* dest, const char* src);
LV2_EXPORT char *strncpy(char *destination, const char *source, size_t num);
LV2_EXPORT size_t strlen(const char* s);
LV2_EXPORT int strcmp(const char *s1, const char *s2);
LV2_EXPORT int strncmp(const char* s1, const char* s2, size_t n);

// stdio.h
LV2_EXPORT int printf(char *format, ...);
LV2_EXPORT void* snprintf(char *str, size_t size, char *format, ...);

// lv2
LV2_EXPORT __attribute__((malloc)) void* alloc(size_t size, int unk);
LV2_EXPORT void dealloc(void* ptr, int unk);
LV2_EXPORT void panic(int unk);

// Example fsOpen usage
// fsOpen(?, 0x242, ?, 0x180, 0, 0)
// fsOpen(?, 0x242, ?, 0x180, u32*, 8)
// fsOpen(?, 0, ?, 0x180, 0, 0)
LV2_EXPORT int fsOpen(const char* path, int mode, s32* fd, int unk1, int unk2, int unk3);
LV2_EXPORT int fsRead(s32 fd, void* buffer, int size, u64* read);
LV2_EXPORT int fsWrite(s32 fd, const void* buffer, int size, u64* written);
LV2_EXPORT int fsSeek(s32 fd, s64 where, int whence, s64* position); // Not really sure about this one
LV2_EXPORT int fsClose(int fd);

// misc
static inline void* malloc(size_t size) { return alloc(size, 0x27); }
static inline void free(void* ptr) { dealloc(ptr, 0x27); }
