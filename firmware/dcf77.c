#include "dcf77.h"
#include "stm32l0xx.h"

#define DCF77_VCC_GPIO GPIOB
#define DCF77_VCC_PIN GPIO_PIN_6

#define DCF77_SIGNAL_GPIO GPIOB
#define DCF77_SIGNAL_PIN GPIO_PIN_7

void dcf77_init(void) {
    GPIO_InitTypeDef init_vcc = {
        .Pin = DCF77_VCC_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(DCF77_VCC_GPIO, &init_vcc);
    HAL_GPIO_WritePin(DCF77_VCC_GPIO, DCF77_VCC_PIN, GPIO_PIN_RESET);

    GPIO_InitTypeDef init_signal = {
        .Pin = DCF77_SIGNAL_PIN,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(DCF77_SIGNAL_GPIO, &init_signal);
}

void dcf77_tick(void) {
}
