#define RTOC 0x800000000033E720

#define LV2_FUNC(name, addr) \
	.global name; \
	name: \
		.quad 0x8000000000000000 | addr, RTOC, 0

.section .opd, "aw"

LV2_FUNC(memcpy, 0x7C01C)
LV2_FUNC(memset, 0x4D144)
LV2_FUNC(memcmp, 0x4C454)

LV2_FUNC(strcat, 0x4D220)
LV2_FUNC(strchr, 0x4D258)
LV2_FUNC(strlen, 0x4D318)
LV2_FUNC(strcpy, 0x4D2F0)
LV2_FUNC(strncpy, 0x4D3B8)
LV2_FUNC(strcmp, 0x4D29C)
LV2_FUNC(strncmp, 0x4D344)

LV2_FUNC(printf, 0x28A654)
LV2_FUNC(snprintf, 0x04E4D8)
LV2_FUNC(sprintf, 0x04E56C)

/* lv2 internal, do not work!
LV2_FUNC(fsOpen, 0x2AB290)
LV2_FUNC(fsRead, 0x2AB234)
LV2_FUNC(fsWrite, 0x2AB1A0)
LV2_FUNC(fsSeek, 0x2AA9F4)
LV2_FUNC(fsClose, 0x2AB0F8)
LV2_FUNC(fsStat, 0x2AAAAC)
*/

LV2_FUNC(alloc, 0x62088)
LV2_FUNC(dealloc, 0x624C8)
LV2_FUNC(panic, 0x288568)
