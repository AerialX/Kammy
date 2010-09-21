TARGET		:= libkammy.a
SOURCE		:= source
INCLUDE		:= include
BUILDDIR	:= build

CFLAGS		:= -nodefaultlibs -nostdlib -m64 \
			   -fno-builtin -fno-exceptions \
			   -Os -Wall $(foreach dir,$(INCLUDE),-I$(dir))
CXXFLAGS	:= $(CFLAGS) -fno-rtti
CFLAGS		+= --std=gnu99

#CC			:= ppu-gcc
#CXX			:= ppu-g++
#AR			:= ppu-ar
CC			:= ppu-lv2-gcc
CXX			:= ppu-lv2-g++
AR			:= ppu-lv2-ar

CFILES		:= $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.c)))
CXXFILES	:= $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.cpp)))
OFILES		:= $(CFILES:%.c=$(BUILDDIR)/%.o) \
			   $(CXXFILES:%.cpp=$(BUILDDIR)/%.o) \
			   $(SYMBOLFILE:%.S=$(BUILDDIR)/%.o)

all: builddir $(TARGET)

clean:
	rm -rf $(TARGET) $(BUILDDIR) $(OFILES)

builddir:
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(foreach dir,$(SOURCE),$(dir)/%.c)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: $(foreach dir,$(SOURCE),$(dir)/%.cpp)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.S
	$(CC) -c $< -o $@

$(TARGET): $(OFILES)
	$(AR) rcs $@ $(OFILES)
