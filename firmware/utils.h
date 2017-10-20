#pragma once
#include <assert.h>

#define CONCAT_IMPL(A, B) A ## B
#define CONCAT(A, B) CONCAT_IMPL(A, B)

#define COUNTOF(ARR) ((int)(sizeof(ARR) / sizeof(ARR[0])))

#ifdef NDEBUG
    #define ASSERT(EXPR) do { (void)(EXPR); } while (0)
#else
    #define ASSERT(EXPR) do { assert(EXPR); } while (0)
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
