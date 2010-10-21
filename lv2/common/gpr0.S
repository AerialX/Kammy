/* Required when compiling under gcc 4.4.x and higher, *
 * without linking to libgcc                           */

#define _GLOBAL(sym) \
	.global sym; \
	sym:

_GLOBAL(_savegpr0_14)   std     14,-0x90(1)
_GLOBAL(_savegpr0_15)   std     15,-0x88(1)
_GLOBAL(_savegpr0_16)   std     16,-0x80(1)
_GLOBAL(_savegpr0_17)   std     17,-0x78(1)
_GLOBAL(_savegpr0_18)   std     18,-0x70(1)
_GLOBAL(_savegpr0_19)   std     19,-0x68(1)
_GLOBAL(_savegpr0_20)   std     20,-0x60(1)
_GLOBAL(_savegpr0_21)   std     21,-0x58(1)
_GLOBAL(_savegpr0_22)   std     22,-0x50(1)
_GLOBAL(_savegpr0_23)   std     23,-0x48(1)
_GLOBAL(_savegpr0_24)   std     24,-0x40(1)
_GLOBAL(_savegpr0_25)   std     25,-0x38(1)
_GLOBAL(_savegpr0_26)   std     26,-0x30(1)
_GLOBAL(_savegpr0_27)   std     27,-0x28(1)
_GLOBAL(_savegpr0_28)   std     28,-0x20(1)
_GLOBAL(_savegpr0_29)   std     29,-0x18(1)
_GLOBAL(_savegpr0_30)   std     30,-0x10(1)
_GLOBAL(_savegpr0_31)   std     31,-0x08(1)
                        std     0,0x10(1)
                        blr

_GLOBAL(_restgpr0_14)   ld     14,-0x90(1)
_GLOBAL(_restgpr0_15)   ld     15,-0x88(1)
_GLOBAL(_restgpr0_16)   ld     16,-0x80(1)
_GLOBAL(_restgpr0_17)   ld     17,-0x78(1)
_GLOBAL(_restgpr0_18)   ld     18,-0x70(1)
_GLOBAL(_restgpr0_19)   ld     19,-0x68(1)
_GLOBAL(_restgpr0_20)   ld     20,-0x60(1)
_GLOBAL(_restgpr0_21)   ld     21,-0x58(1)
_GLOBAL(_restgpr0_22)   ld     22,-0x50(1)
_GLOBAL(_restgpr0_23)   ld     23,-0x48(1)
_GLOBAL(_restgpr0_24)   ld     24,-0x40(1)
_GLOBAL(_restgpr0_25)   ld     25,-0x38(1)
_GLOBAL(_restgpr0_26)   ld     26,-0x30(1)
_GLOBAL(_restgpr0_27)   ld     27,-0x28(1)
_GLOBAL(_restgpr0_28)   ld     28,-0x20(1)
_GLOBAL(_restgpr0_29)   ld     29,-0x18(1)
_GLOBAL(_restgpr0_30)   ld     30,-0x10(1)
_GLOBAL(_restgpr0_31)   ld     31,-0x08(1)
                        ld     0,0x10(1)
                        mtlr   0
                        blr

