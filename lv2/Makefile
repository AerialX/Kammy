LV2BINS		:= kammy ethdebug

LV2CLEAN	:= $(foreach bin,$(LV2BINS),$(bin)\ clean)
LV2OBJDUMP	:= $(foreach bin,$(LV2BINS),$(bin)\ objdump)

all: $(LV2BINS)

clean: $(LV2CLEAN)

objdump: $(LV2OBJDUMP)

$(LV2BINS) $(LV2CLEAN) $(LV2OBJDUMP):
	@make -f Makefile.$@

.PHONY: $(LV2BINS)
