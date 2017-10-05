#include "stm32_assert.h"
#include <stdio.h>
#include <inttypes.h>

void assert_failed(uint8_t* file, uint32_t line) {
    printf("Assertion failed! File: %s Line: %" PRIu32 "\n", (const char*)file, line);
}
