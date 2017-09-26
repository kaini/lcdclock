ifdef RELEASE
	OUTDIR := release
else
	OUTDIR := debug
endif

CMSIS_C_SOURCES := $(wildcard cmsis/src/*.c)
CMSIS_C_OBJECTS := $(patsubst %.c,$(OUTDIR)/%.o,$(notdir $(CMSIS_C_SOURCES)))
CMSIS_S_SOURCES := $(wildcard cmsis/src/*.s)
CMSIS_S_OBJECTS := $(patsubst %.s,$(OUTDIR)/%.o,$(notdir $(CMSIS_S_SOURCES)))

HAL_C_SOURCES := $(wildcard hal/src/*.c)
HAL_C_OBJECTS := $(patsubst %.c,$(OUTDIR)/%.o,$(notdir $(HAL_C_SOURCES)))

C_SOURCES := $(wildcard *.c)
C_OBJECTS := $(patsubst %.c,$(OUTDIR)/%.o,$(C_SOURCES))

FIRMWARE := $(OUTDIR)/firmware.elf

CC := arm-none-eabi-gcc
RM := rm -rf
MKDIR := mkdir -p

INCLUDES := -isystem cmsis/include -isystem hal/include
CPU := -mcpu=cortex-m0plus -mfloat-abi=soft -mthumb
DEFINES := -DSTM32L073xx
WARNINGS := -std=gnu11 -Wall -Wextra -Wno-unused-parameter
OPTIMIZATION := -fdata-sections -ffunction-sections -Wl,--gc-sections
ifdef RELEASE
	OPTIMIZATION += -O3 -flto --specs=nosys.specs
else
	OPTIMIZATION += -Og -g3 --specs=rdimon.specs
endif
CFLAGS := $(INCLUDES) $(DEFINES) $(CPU) $(WARNINGS) $(OPTIMIZATION)

.PHONY: clean

all: $(FIRMWARE)

$(FIRMWARE): $(CMSIS_S_OBJECTS) $(CMSIS_C_OBJECTS) $(HAL_C_OBJECTS) $(C_OBJECTS) linker.ld
	$(CC) $(CFLAGS) -T linker.ld -o $@ $(CMSIS_S_OBJECTS) $(CMSIS_C_OBJECTS) $(HAL_C_OBJECTS) $(C_OBJECTS)

-include $(patsubst %.o,%.d,$(CMSIS_C_OBJECTS))
$(CMSIS_C_OBJECTS): $(OUTDIR)/%.o: cmsis/src/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) -MD -c -o $@ $<

$(CMSIS_S_OBJECTS): $(OUTDIR)/%.o: cmsis/src/%.s | $(OUTDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(patsubst %.o,%.d,$(HAL_C_OBJECTS))
$(HAL_C_OBJECTS): $(OUTDIR)/%.o: hal/src/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) -MD -c -o $@ $<

-include $(patsubst %.o,%.d,$(C_OBJECTS))
$(C_OBJECTS): $(OUTDIR)/%.o: %.c | $(OUTDIR)
	$(CC) $(CFLAGS) -MD -c -o $@ $<

$(OUTDIR):
	$(MKDIR) $@

clean:
	$(RM) debug release
