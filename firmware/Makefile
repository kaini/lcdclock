OUTDIR := build
C_SOURCES := $(wildcard cmsis/*.c) $(wildcard hal/*.c) $(wildcard *.c)
S_SOURCES := $(wildcard cmsis/*.s)
C_OBJECTS := $(patsubst %.c,$(OUTDIR)/%.o,$(C_SOURCES))
S_OBJECTS := $(patsubst %.s,$(OUTDIR)/%.o,$(S_SOURCES))
FIRMWARE := $(OUTDIR)/firmware

TEST_C_SOURCES := $(wildcard test/*.c) dcf77_parser.c
TEST_C_OBJECTS := $(patsubst %.c,$(OUTDIR)/%.test.o,$(TEST_C_SOURCES))
TEST_EXE := $(OUTDIR)/testrunner

RM := rm -rf
MKDIR := mkdir -p
JLINK := jlink

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
TEST_CC := gcc

COMMON_CFLAGS := \
	-I. -Icmsis -Ihal \
	-DSTM32L073xx -DUSE_HAL_DRIVER \
	-std=gnu11 -Wall -Wextra -Wno-unused-parameter \
	-fno-strict-aliasing -fwrapv -g3
TARGET_CFLAGS := \
	-mcpu=cortex-m0plus -mfloat-abi=soft -mthumb \
	-fdata-sections -ffunction-sections -Wl,--gc-sections \
	-fshort-enums
TARGET_DEBUG_CFLAGS := \
	-Og --specs=rdimon.specs
TARGET_RELEASE_CFLAGS := \
	-DNDEBUG -flto -O3 --specs=nosys.specs
TEST_CFLAGS := \
	-O0

FINAL_TARGET_CFLAGS := $(COMMON_CFLAGS) $(TARGET_CFLAGS)
ifdef RELEASE
	FINAL_TARGET_CFLAGS += $(TARGET_RELEASE_CFLAGS)
else
	FINAL_TARGET_CFLAGS += $(TARGET_DEBUG_CFLAGS)
endif

FINAL_TEST_CFLAGS := $(COMMON_CFLAGS) $(TEST_CFLAGS)

TEST_LIBS := -lcmocka

.SUFFIXES:  # disable builtin rules
.PHONY: clean test flash

all: $(FIRMWARE).elf $(FIRMWARE).bin test

test: $(TEST_EXE)
	$(TEST_EXE)

flash: $(FIRMWARE).bin | flash.jlink
	$(JLINK) -CommandFile flash.jlink

$(FIRMWARE).elf: $(S_OBJECTS) $(C_OBJECTS) | linker.ld
	@$(MKDIR) $(@D)
	$(CC) $(FINAL_TARGET_CFLAGS) -T linker.ld -o $@ $^
	$(SIZE) $@

$(TEST_EXE): $(TEST_C_OBJECTS)
	@$(MKDIR) $(@D)
	$(TEST_CC) $(FINAL_TEST_CFLAGS) -o $@ $^ $(TEST_LIBS)

-include $(patsubst %.o,%.d,$(C_OBJECTS))
-include $(patsubst %.o,%.d,$(S_OBJECTS))
-include $(patsubst %.o,%.d,$(TEST_C_OBJECTS))

%.bin: %.elf
	@$(MKDIR) $(@D)
	$(OBJCOPY) -O binary $^ $@

$(C_OBJECTS): $(OUTDIR)/%.o: %.c
	@$(MKDIR) $(@D)
	$(CC) $(FINAL_TARGET_CFLAGS) -MD -c -o $@ $<

$(S_OBJECTS): $(OUTDIR)/%.o: %.s
	@$(MKDIR) $(@D)
	$(CC) $(FINAL_TARGET_CFLAGS) -MD -c -o $@ $<

$(TEST_C_OBJECTS): $(OUTDIR)/%.test.o: %.c
	@$(MKDIR) $(@D)
	$(TEST_CC) $(FINAL_TEST_CFLAGS) -MD -c -o $@ $<

clean:
	$(RM) $(OUTDIR)
