#pragma once

#define MAXPATHLEN 0x400

typedef s32 FsMode;
typedef s32 FsFile;
typedef s64 time_t;

// Open Modes
#define O_RDONLY	000000
#define O_WRONLY	000001
#define O_RDWR		000002
#define O_ACCMODE	000003
#define O_CREAT		000100
#define O_EXCL		000200
#define O_TRUNC		001000
#define O_APPEND	002000

// Seek Types
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

// Dir Modes
#define	S_IFDIR		0040000
#define	S_IFREG		0100000
#define	S_IFLNK		0120000

typedef struct {	
	FsMode st_mode;
	s32 st_uid;
	s32 st_gid;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	u64 st_size;
	u64 st_blksize;
} FsStat;

typedef struct {
	u8 d_type;
	u8 d_namlen;
	char d_name[MAXPATHLEN + 1];
} FsDirent;

typedef struct {
	time_t actime;
	time_t modtime;
} FsUtime;

#define LV2_STDIO static inline __attribute__ ((unused)) s32
LV2_STDIO fsOpen(const char* path, s32 oflags, FsFile* fd, u64 unk, const void* arg, u64 argsize) { return Lv2Syscall(801, (u64)path, oflags, (u64)fd, unk, (u64)arg, argsize); }
LV2_STDIO fsRead(FsFile fd, void* buf, u64 size, u64* read) { return Lv2Syscall(802, fd, (u64)buf, size, (u64)read); }
LV2_STDIO fsWrite(FsFile fd, const void* buf, u64 size, u64* written) { return Lv2Syscall(803, fd, (u64)buf, size, (u64)written); }
LV2_STDIO fsClose(FsFile fd) { return Lv2Syscall(804, fd); }
LV2_STDIO fsOpenDir(const char* path, FsFile* fd) { return Lv2Syscall(805, (u64)path, (u64)fd); }
LV2_STDIO fsReadDir(FsFile fd, FsDirent* entry, u64* read) { return Lv2Syscall(806, fd, (u64)entry, (u64)read); }
LV2_STDIO fsCloseDir(FsFile fd) { return Lv2Syscall(807, fd); }
LV2_STDIO fsStat(const char* path, FsStat* buf) { return Lv2Syscall(808, (u64)path, (u64)buf); }
LV2_STDIO fsFstat(FsFile fd, FsStat* buf) { return Lv2Syscall(809, fd, (u64)buf); }
LV2_STDIO fsMkdir(const char* path, FsMode mode) { return Lv2Syscall(811, (u64)path, mode); }
LV2_STDIO fsRename(const char* path, const char* newpath) { return Lv2Syscall(812, (u64)path, (u64)newpath); }
LV2_STDIO fsRmdir(const char* path) { return Lv2Syscall(813, (u64)path); }
LV2_STDIO fsUnlink(const char* path) { return Lv2Syscall(814, (u64)path); }
LV2_STDIO fsLSeek64(FsFile fd, s64 offset, s32 whence, u64* position) { return Lv2Syscall(818, fd, offset, whence, (u64)position); }
LV2_STDIO fsFsync(FsFile fd) { return Lv2Syscall(820, fd); }
LV2_STDIO fsTruncate(const char* path, u64 size) { return Lv2Syscall(831, (u64)path, size); }
LV2_STDIO fsFtruncate(FsFile fd, u64 size) { return Lv2Syscall(832, fd, size); }
LV2_STDIO fsUtime(const char* path, const FsUtime* times) { return Lv2Syscall(815, (u64)path, (u64)times); }
LV2_STDIO fsChmod(const char* path, FsMode mode) { return Lv2Syscall(834, (u64)path, mode); }
