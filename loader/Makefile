TARGET		:= loader
SOURCE		:= source
INCLUDE		:= include $(CELL_SDK)/target/ppu/include
BUILDDIR	:= build
LIBDIRS		:= ../libkammy
LIBS		:= kammy sysmodule_stub netctl_stub net_stub

LV2DIR		:= ../lv2
LV2BINS		:= kammy.bin

INCLUDE		+= build $(foreach dir,$(LIBDIRS),$(dir)/include)
#CFLAGS		:= -nodefaultlibs -nostdlib -m64
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

CC			:= ppu-lv2-gcc
CXX			:= ppu-lv2-g++
LD			:= ppu-lv2-g++
#CC			:= ppu-gcc
#CXX			:= ppu-g++
#LD			:= ppu-ld
FSELF		:= make_fself

CFILES		:= $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.c)))
CXXFILES	:= $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.cpp)))
OFILES		:= $(CFILES:%.c=$(BUILDDIR)/%.o) \
			   $(CXXFILES:%.cpp=$(BUILDDIR)/%.o) \
			   $(SYMBOLFILE:%.S=$(BUILDDIR)/%.o)

all: builddir $(TARGET).self

clean:
	rm -rf $(TARGET).self $(TARGET).elf $(BUILDDIR) $(OFILES) raw2h

install: $(TARGET).self
	ps3install $(TARGET).self

builddir:
	@mkdir -p $(BUILDDIR)

raw2h: raw2h.c
	$(HOST_CC) $< -o $@

$(OFILES): raw2h $(LV2BINS:%.bin=$(BUILDDIR)/%.h)

$(BUILDDIR)/%.o: $(foreach dir,$(SOURCE),$(dir)/%.c)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: $(foreach dir,$(SOURCE),$(dir)/%.cpp)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.S
	$(CC) -c $< -o $@

$(foreach bin,$(LV2BINS),$(LV2DIR)/$(bin)):
	make -C $(LV2DIR)

../libkammy/libkammy.a:
	make -C ../libkammy

$(BUILDDIR)/%.h: $(LV2DIR)/%.bin
	./raw2h $< $(BUILDDIR)/$(notdir $<).h $(notdir $(basename $<)_bin)

$(TARGET).elf: $(OFILES) ../libkammy/libkammy.a
	$(LD) $(OFILES) $(LDFLAGS) -o $@

$(TARGET).self: $(TARGET).elf
	$(FSELF) $(TARGET).elf $@
