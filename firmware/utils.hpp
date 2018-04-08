#pragma once
#include <cassert>
#include <cstdlib>

#ifdef NDEBUG
    #define ASSERT(EXPR) do { if (!(EXPR)) { abort(); while (1) {} } } while (0)
#else
    #define ASSERT(EXPR) do { assert(EXPR); } while (0)
#endif

#define ASSERT_UNREACHABLE() do { ASSERT(false); abort(); while (1) {} } while (0)

#if defined(NDEBUG) || defined(TEST)
	#define DEBUG_PRINTF(...) do { } while (0)
#else
    #include "SEGGER_RTT.h"
    #define DEBUG_PRINTF(...) do { SEGGER_RTT_printf(0, __VA_ARGS__); } while (0)
#endif

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
