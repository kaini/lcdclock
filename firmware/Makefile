OUTDIR := build

SOURCES := $(wildcard vendor/cmsis/*.c) $(wildcard vendor/cmsis/*.s) $(wildcard vendor/segger_rtt/*.c) $(wildcard *.c) $(wildcard *.cpp)
OBJECTS := $(patsubst %,$(OUTDIR)/%.o,$(SOURCES))
FIRMWARE_ELF := $(OUTDIR)/firmware.elf
FIRMWARE_BIN := $(OUTDIR)/firmware.bin

TEST_SOURCES := $(wildcard test/*.cpp) dcf77_parser.cpp datetime.cpp
TEST_OBJECTS := $(patsubst %,$(OUTDIR)/%.test.o,$(TEST_SOURCES))
TEST_EXE := $(OUTDIR)/testrunner.exe

RM := rm -rf
MKDIR := mkdir -p
JLINK := jlink
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
READELF := arm-none-eabi-readelf
OBJDUMP := arm-none-eabi-objdump

AS := arm-none-eabi-as
ASFLAGS := -mcpu=cortex-m0plus -mfloat-abi=soft -mthumb -g

COMMON_C_CXX_FLAGS := \
	-I$(CURDIR)/vendor/cmsis -I$(CURDIR)/vendor/segger_rtt -I$(CURDIR)/vendor \
	-DSTM32L073xx \
	-Wall -Wextra -Wno-unused-parameter \
	-fno-strict-aliasing -fwrapv -g
TARGET_C_CXX_FLAGS := \
	-mcpu=cortex-m0plus -mfloat-abi=soft -mthumb \
	--specs=nosys.specs \
	-fdata-sections -ffunction-sections -Wl,--gc-sections
TARGET_DEBUG_C_CXX_FLAGS := -Og
TARGET_RELEASE_C_CXX_FLAGS := -DNDEBUG -flto -O3

CC := arm-none-eabi-gcc
CFLAGS := $(COMMON_C_CXX_FLAGS) $(TARGET_C_CXX_FLAGS) -std=c11
ifdef RELEASE
	CFLAGS += $(TARGET_RELEASE_C_CXX_FLAGS)
else
	CFLAGS += $(TARGET_DEBUG_C_CXX_FLAGS)
endif

CXX := arm-none-eabi-g++
CXXFLAGS := $(COMMON_C_CXX_FLAGS) $(TARGET_C_CXX_FLAGS) -std=c++14
ifdef RELEASE
	CXXFLAGS += $(TARGET_RELEASE_C_CXX_FLAGS)
else
	CXXFLAGS += $(TARGET_DEBUG_C_CXX_FLAGS)
endif

TEST_CXX := g++
TEST_CXXFLAGS := $(COMMON_C_CXX_FLAGS) -DTEST -std=gnu++14 -O0
TEST_LIBS := -lboost_unit_test_framework-mt

.SUFFIXES:  # disable builtin rules

.PHONY: all
all: $(FIRMWARE_ELF) $(FIRMWARE_BIN) test

.PHONY: test
test: $(TEST_EXE)
	$(CURDIR)/$<

.PHONY: flash
flash: $(FIRMWARE_BIN)
	$(JLINK) -CommandFile flash.jlink

.PHONY: clean
clean:
	$(RM) $(OUTDIR)

$(FIRMWARE_ELF): $(OBJECTS)
	@$(MKDIR) $(@D)
	$(CXX) $(CXXFLAGS) -T linker.ld -Wl,-Map=$@.map -o $@ $^
	$(OBJDUMP) -dS $@ > $@.objdump.txt
	$(READELF) -a $@ > $@.readelf.txt
	$(SIZE) $@

$(TEST_EXE): $(TEST_OBJECTS)
	@$(MKDIR) $(@D)
	$(TEST_CXX) $(TEST_CXXFLAGS) -o $@ $^ $(TEST_LIBS)

-include $(patsubst %.o,%.d,$(OBJECTS))
-include $(patsubst %.o,%.d,$(TEST_OBJECTS))

%.bin: %.elf
	@$(MKDIR) $(@D)
	$(OBJCOPY) -O binary $^ $@

$(OUTDIR)/%.c.o: %.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -MD -MP -c -o $@ $<

$(OUTDIR)/%.s.o: %.s
	@$(MKDIR) $(@D)
	$(AS) $(ASFLAGS) -o $@ $<
	
$(OUTDIR)/%.cpp.o: %.cpp
	@$(MKDIR) $(@D)
	$(CXX) $(CXXFLAGS) -MD -MP -c -o $@ $<

$(OUTDIR)/%.cpp.test.o: %.cpp
	@$(MKDIR) $(@D)
	$(TEST_CXX) $(TEST_CXXFLAGS) -MD -MP -c -o $@ $<
