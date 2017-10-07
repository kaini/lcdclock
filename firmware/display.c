#include "display.h"
#include "stm32l0xx.h"
#include "my_assert.h"

// 1/4 duty, 1/3 bias, 64 Hz frame time
static LCD_HandleTypeDef hlcd = {
    .Instance = LCD,
    .Init = {
        .Prescaler = LCD_PRESCALER_8,
        .Divider = LCD_DIVIDER_16,
        .Duty = LCD_DUTY_1_4,
        .Bias = LCD_BIAS_1_3,
        .VoltageSource = LCD_VOLTAGESOURCE_EXTERNAL,
        .DeadTime = LCD_DEADTIME_0,
        .PulseOnDuration = LCD_PULSEONDURATION_0,  // TODO maybe increase
        .HighDrive = LCD_HIGHDRIVE_0,
        .BlinkMode = LCD_BLINKMODE_OFF,
    },
};

/* override */ void HAL_LCD_MspInit(LCD_HandleTypeDef* hlcd) {
    GPIO_InitTypeDef init = {
        .Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7 |
               GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_HIGH,
        .Alternate = GPIO_AF1_LCD,
    };
    HAL_GPIO_Init(GPIOA, &init);
    init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
               GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &init);
}

void display_init(void) {
    HAL_StatusTypeDef result = HAL_LCD_Init(&hlcd);
    ASSERT(result == HAL_OK);

    result = HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0, UINT32_MAX, UINT32_MAX);
    ASSERT(result == HAL_OK);
    result = HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, UINT32_MAX, UINT32_MAX);
    ASSERT(result == HAL_OK);
    result = HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, UINT32_MAX, UINT32_MAX);
    ASSERT(result == HAL_OK);
    result = HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, UINT32_MAX, UINT32_MAX);
    ASSERT(result == HAL_OK);
    result = HAL_LCD_UpdateDisplayRequest(&hlcd);
    ASSERT(result == HAL_OK);
}
