ifdef RELEASE
	OUTDIR := release
else
	OUTDIR := debug
endif

C_SOURCES := $(wildcard cmsis/src/*.c) $(wildcard hal/src/*.c) $(wildcard *.c)
S_SOURCES := $(wildcard cmsis/src/*.s)
C_OBJECTS := $(patsubst %.c,$(OUTDIR)/%.o,$(C_SOURCES))
S_OBJECTS := $(patsubst %.s,$(OUTDIR)/%.o,$(S_SOURCES))
FIRMWARE := $(OUTDIR)/firmware.elf

TEST_C_SOURCES := $(wildcard test/*.c) dcf77_parser.c
TEST_C_OBJECTS := $(patsubst %.c,$(OUTDIR)/%.test.o,$(TEST_C_SOURCES))
TEST_EXE := $(OUTDIR)/testrunner

RM := rm -rf
MKDIR := mkdir -p

CC := arm-none-eabi-gcc
READELF := arm-none-eabi-readelf
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
TEST_CC := gcc

COMMON_CFLAGS := \
	-Icmsis/include -Ihal/include -I. \
	-DSTM32L073xx -DUSE_HAL_DRIVER \
	-std=gnu11 -Wall -Wextra -Wno-unused-parameter \
	-fno-strict-aliasing
TARGET_CFLAGS := \
	-mcpu=cortex-m0plus -mfloat-abi=soft -mthumb  \
	-fdata-sections -ffunction-sections -Wl,--gc-sections \
	-fshort-enums
TARGET_DEBUG_CFLAGS := \
	-Og -g3 --specs=rdimon.specs
TARGET_RELEASE_CFLAGS := \
	-DNDEBUG -flto -O3 --specs=nosys.specs
TEST_CFLAGS := \
	-O0 -g3 -fsanitize=undefined -fno-sanitize-recover

FINAL_TARGET_CFLAGS := $(COMMON_CFLAGS) $(TARGET_CFLAGS)
ifdef RELEASE
	FINAL_TARGET_CFLAGS += $(TARGET_RELEASE_CFLAGS)
else
	FINAL_TARGET_CFLAGS += $(TARGET_DEBUG_CFLAGS)
endif

FINAL_TEST_CFLAGS := $(COMMON_CFLAGS) $(TEST_CFLAGS)

TEST_LIBS := -lcmocka

.SUFFIXES:  # disable builtin rules
.PHONY: clean test

all: $(FIRMWARE) test

test: $(TEST_EXE)
	$(TEST_EXE)

$(FIRMWARE): $(S_OBJECTS) $(C_OBJECTS) | linker.ld
	@$(MKDIR) $(@D)
	$(CC) $(FINAL_TARGET_CFLAGS) -T linker.ld -o $@ $^
	$(READELF) -a $@ > $@.txt
	$(OBJDUMP) -d $@ > $@.asm
	$(SIZE) $@

$(TEST_EXE): $(TEST_C_OBJECTS)
	@$(MKDIR) $(@D)
	$(TEST_CC) $(FINAL_TEST_CFLAGS) -o $@ $^ $(TEST_LIBS)

-include $(patsubst %.o,%.d,$(C_OBJECTS))
-include $(patsubst %.o,%.d,$(S_OBJECTS))
-include $(patsubst %.o,%.d,$(TEST_C_OBJECTS))

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
	$(RM) debug release
