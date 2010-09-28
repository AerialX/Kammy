TARGET		:= libkammy.a
SOURCE		:= source ../lv2/common
INCLUDE		:= include ../lv2/common
BUILDDIR	:= build

LV2DIR		:= ../lv2
LV2BINS		:= kammy.bin

CFLAGS		:= -nodefaultlibs -nostdlib -m64 \
			   -fno-builtin -fno-exceptions \
			   -Os -Wall $(foreach dir,$(INCLUDE),-I$(dir)) \
			   -I$(BUILDDIR)
CXXFLAGS	:= $(CFLAGS) -fno-rtti
CFLAGS		+= --std=gnu99

#CC			:= ppu-gcc
#CXX			:= ppu-g++
#AR			:= ppu-ar
CC			:= ppu-lv2-gcc
CXX			:= ppu-lv2-g++
AR			:= ppu-lv2-ar
RAW2H		:= ../raw2h/raw2h

#CFILES		:= $(foreach dir,$(SOURCE),$(wildcard $(dir)/*.c))
CXXFILES	:= $(foreach dir,$(SOURCE),$(wildcard $(dir)/*.cpp))
OFILES		:= $(CFILES:%.c=$(BUILDDIR)/%.o) \
			   $(CXXFILES:%.cpp=$(BUILDDIR)/%.o)

all: builddir $(TARGET)

$(OFILES): $(RAW2H) $(LV2BINS:%.bin=$(BUILDDIR)/%.h)

clean:
	rm -rf $(TARGET) $(BUILDDIR) lv2

builddir:
	@mkdir -p $(BUILDDIR) $(foreach dir,$(SOURCE),$(BUILDDIR)/$(dir))

$(RAW2H):
	make -C ../raw2h

$(BUILDDIR)/%.o: %.c
	@echo "[CC]  $<"
	@$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.cpp
	@echo "[CXX] $<"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

$(foreach bin,$(LV2BINS),$(LV2DIR)/$(bin)):
	make -C $(LV2DIR)

$(BUILDDIR)/%.h: $(LV2DIR)/%.bin
	@echo "[R2H] $<"
	@$(RAW2H) $< $(BUILDDIR)/$(notdir $<).h $(notdir $(basename $<)_bin)

$(TARGET): $(OFILES)
	@echo "[AR]  $@"
	@$(AR) rcs $@ $(OFILES)
