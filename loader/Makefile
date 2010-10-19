TARGET		:= loader
SOURCE		:= source
INCLUDE		:= include $(CELL_SDK)/target/ppu/include ../lv2
BUILDDIR	:= build
LIBDIRS		:= ../libkammy
LIBS		:= kammy sysmodule_stub netctl_stub net_stub fs_stub

LV2DIR		:= ../lv2
LV2BINS		:= ethdebug.bin

INCLUDE		+= $(BUILDDIR) $(foreach dir,$(LIBDIRS),$(dir)/include)
CFLAGS		:= -mp32 -DBIG_ENDIAN -DBYTE_ORDER \
			   -fno-builtin -fno-exceptions \
			   -Os -Wall $(foreach dir,$(INCLUDE),-I$(dir))
CXXFLAGS	:= $(CFLAGS) -fno-rtti
CFLAGS		+= --std=gnu99
LDFLAGS		:= -s \
			   $(foreach dir,$(LIBDIRS),-L$(dir)) \
			   $(foreach lib,$(LIBS),-l$(lib)) \
			   -L$(CELL_SDK)/target/ppu/lib -static

HOST_CC		:= gcc

#CC			:= ppu-gcc
#CXX			:= ppu-g++
#LD			:= ppu-ld
CC			:= ppu-lv2-gcc
CXX			:= ppu-lv2-g++
LD			:= ppu-lv2-g++
FSELF		:= make_fself
RAW2H		:= ../raw2h/raw2h

CFILES		:= $(foreach dir,$(SOURCE),$(wildcard $(dir)/*.c))
CXXFILES	:= $(foreach dir,$(SOURCE),$(wildcard $(dir)/*.cpp))
OFILES		:= $(CFILES:%.c=$(BUILDDIR)/%.o) \
			   $(CXXFILES:%.cpp=$(BUILDDIR)/%.o)

all: builddir $(TARGET).self

clean:
	rm -rf $(TARGET).self $(TARGET).elf $(BUILDDIR) $(OFILES)

install: $(TARGET).self
	@echo "[INS] $<"
	@ps3install $(TARGET).self

builddir:
	@mkdir -p $(BUILDDIR) $(foreach dir,$(SOURCE),$(BUILDDIR)/$(dir))

$(RAW2H):
	make -C ../raw2h

$(OFILES): $(RAW2H) $(LV2BINS:%.bin=$(BUILDDIR)/%.h)

$(BUILDDIR)/%.o: %.c
	@echo "[CC]  $<"
	@$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.cpp
	@echo "[CXX] $<"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

$(foreach bin,$(LV2BINS),$(LV2DIR)/$(bin)):
	make -C $(LV2DIR)

../libkammy/libkammy.a:
	make -C ../libkammy

$(BUILDDIR)/%.h: $(LV2DIR)/%.bin
	@echo "[R2H] $<"
	@$(RAW2H) $< $(BUILDDIR)/$(notdir $<).h $(notdir $(basename $<)_bin)

$(TARGET).elf: $(OFILES) ../libkammy/libkammy.a
	@echo "[LD]  $@"
	@$(LD) $(OFILES) $(LDFLAGS) -o $@

$(TARGET).self: $(TARGET).elf
	@$(FSELF) $(TARGET).elf $@
