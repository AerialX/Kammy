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

LV2_FUNC(alloc, 0x62088)
LV2_FUNC(free, 0x624C8)
LV2_FUNC(panic, 0x288568)
