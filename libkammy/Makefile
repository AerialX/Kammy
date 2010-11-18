.SUFFIXES:
ifeq ($(strip $(PSL1GHT)),)
$(error "PSL1GHT must be set in the environment.")
endif

include $(PSL1GHT)/Makefile.base

TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCE		:=	source
LV2SOURCE	:=	../lv2/common
INCLUDE		:=	include ../lv2/common
DATA		:=	data
LIBS		:=	

CFLAGS		+= -g -O2 -Wall
CXXFLAGS	+= -g -O2 -Wall

ifneq ($(BUILD),$(notdir $(CURDIR)))

.PHONY: $(BUILD) clean

export OUTDIR	:=  $(CURDIR)
export OUTPUT	:=	$(CURDIR)/$(TARGET)
export VPATH	:=	$(foreach dir,$(SOURCE),$(CURDIR)/$(dir)) \
					$(foreach dir,$(LV2SOURCE),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))
export BUILDDIR	:=	$(CURDIR)/$(BUILD)
export DEPSDIR	:=	$(BUILDDIR)

CFILES		:= $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.c)))
CXXFILES	:= $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.cpp))) \
			   $(foreach dir,$(LV2SOURCE),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:= $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:= $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.bin))) \
			   kammy.bin

export OFILES	:=	$(CFILES:.c=.o) \
					$(CXXFILES:.cpp=.o) \
					$(SFILES:.S=.o)

export BINFILES	:=	$(BINFILES:.bin=.bin.h)

export INCLUDES	:=	$(foreach dir,$(INCLUDE),-I$(CURDIR)/$(dir)) \
					-I$(CURDIR)/$(BUILD)
$(BUILD):	
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C ../lv2 -f Makefile.kammy
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo Clean...
	@rm -rf $(BUILD) $(OUTPUT).a

install:
	@echo "Installing into $(PS3DEV)/ppu..."
	@cp $(OUTPUT).a $(PS3DEV)/ppu/lib/
	@cp -r include $(PS3DEV)/ppu/
	@cp ../lv2/common/kammy_lv2.h $(PS3DEV)/ppu/include/
	@cp ../lv2/common/kammy.x $(PS3DEV)/ppu/lib/
	@mkdir -p $(PS3DEV)/ppu/include/kammy
	@cp ../lv2/common/lv2.h $(PS3DEV)/ppu/include/kammy/

else

DEPENDS	:= $(OFILES:.o=.d)

$(OUTPUT).a: $(OFILES)

$(OFILES): $(BINFILES)

-include $(DEPENDS)

endif
