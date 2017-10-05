ifdef RELEASE
	OUTDIR := release
else
	OUTDIR := debug
endif

SOURCES := $(wildcard cmsis/src/*.s) $(wildcard cmsis/src/*.c) $(wildcard hal/src/*.c) $(wildcard *.c)
OBJECTS := $(patsubst %.s,$(OUTDIR)/%.o,$(patsubst %.c,$(OUTDIR)/%.o,$(SOURCES)))
FIRMWARE := $(OUTDIR)/firmware.elf

RM := rm -rf
MKDIR := mkdir -p

CC := arm-none-eabi-gcc
READELF := arm-none-eabi-readelf
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
CFLAGS := \
	-Icmsis/include -Ihal/include -I. \
	-DSTM32L073xx -DUSE_HAL_DRIVER \
	-mcpu=cortex-m0plus -mfloat-abi=soft -mthumb  \
	-fdata-sections -ffunction-sections -Wl,--gc-sections \
	-std=gnu11 -Werror -Wall -Wextra -Wno-unused-parameter
ifdef RELEASE
	CFLAGS += -DNDEBUG -flto -O3 --specs=nosys.specs
else
	CFLAGS += -Og -g3 --specs=rdimon.specs
endif

.PHONY: clean tests

all: $(FIRMWARE)

$(FIRMWARE): $(OBJECTS) linker.ld
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -T linker.ld -o $@ $(OBJECTS)
	$(READELF) -a $@ > $@.txt
	$(OBJDUMP) -d $@ > $@.asm
	$(SIZE) $@

-include $(patsubst %.o,%.d,$(OBJECTS))

$(OUTDIR)/%.o: %.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -MD -c -o $@ $<

$(OUTDIR)/%.o: %.s
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -MD -c -o $@ $<

clean:
	$(RM) debug release
