#pragma once
#include "stm32l0xx.h"
#include <assert.h>
#include <stdio.h>

#define CONCAT_IMPL(A, B) A ## B
#define CONCAT(A, B) CONCAT_IMPL(A, B)

#define COUNTOF(ARR) ((int)(sizeof(ARR) / sizeof(ARR[0])))

#ifdef NDEBUG
    #define ASSERT(EXPR) do { (void)(EXPR); } while (0)
#else
    #define ASSERT(EXPR) do { assert(EXPR); } while (0)
#endif

#ifdef NDEBUG
	#define DEBUG_PRINTF() do { } while (0)
#else
	#define DEBUG_PRINTF(...) do { fprintf(stdout, __VA_ARGS__); } while (0)
#endif

typedef uint32_t critical_section_handle;
static inline __attribute__((warn_unused_result)) critical_section_handle enter_critical_section() {
	critical_section_handle handle = __get_PRIMASK();
	__disable_irq();
	return handle;
}
static inline void leave_critical_section(critical_section_handle handle) {
	__set_PRIMASK(handle);
}

#define GPIO_PUPDR_PUPD_Pos(PIN) (2 * (PIN))
#define GPIO_PUPDR_PUPD_Msk(PIN) (0b11 << GPIO_PUPDR_PUPD_Pos(PIN))

#define GPIO_OSPEEDER_OSPEED_Pos(PIN) (2 * (PIN))
#define GPIO_OSPEEDER_OSPEED_Msk(PIN) (0b11 << GPIO_OSPEEDER_OSPEED_Pos(PIN))

#define GPIO_OTYPER_OT(PIN) (1 << (PIN))

#define GPIO_MODER_MODE_Pos(PIN) (2 * (PIN))
#define GPIO_MODER_MODE_Msk(PIN) (0b11 << GPIO_MODER_MODE_Pos(PIN))

#define GPIO_ODR_OD(PIN) (1 << (PIN))

#define GPIO_IDR_ID(PIN) (1 << (PIN))

#define GPIO_AFR_Idx(PIN) ((PIN < 8) ? 0 : 1)

#define GPIO_AFR_AFSEL_Pos(PIN) (4 * ((PIN) % 8))
#define GPIO_AFR_AFSEL_Msk(PIN) (0b1111 << GPIO_AFR_AFSEL_Pos(PIN))
