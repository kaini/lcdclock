#include "display.h"
#include "dcf77.h"
#include "stm32l0xx.h"
#include "my_assert.h"
#include <stdbool.h>

/* override */ void HAL_MspInit(void) {
    // Go into the lowest power range
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    // Set MSI (multiple speed internal) to 1 MHz and enable LSE (low speed external clock crystal)
    RCC_OscInitTypeDef osc_init = {
        .OscillatorType = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_LSE,
        .LSEState = RCC_LSE_ON,
        .MSIState = RCC_MSI_ON,
        .MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT,
        .MSIClockRange = RCC_MSIRANGE_4,
        .PLL = {
            .PLLState = RCC_PLL_OFF,
        },
    };
    ASSERT_HAL(HAL_RCC_OscConfig(&osc_init));

    // Setup the system clock and all peripheral clocks to 1 MHz based on the MSI
    RCC_ClkInitTypeDef clk_init = {
        .ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource = RCC_SYSCLKSOURCE_MSI,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV1,
        .APB2CLKDivider = RCC_HCLK_DIV1,
    };
    ASSERT_HAL(HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_0));

    // Enable GPIO clocks
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();

    // Clock RTC by external crystal
    // Note: The LCD clock and RTC clock is the same thing!
    RCC_PeriphCLKInitTypeDef periph_clk_init = {
        .PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_LCD,
        .RTCClockSelection = RCC_RTCCLKSOURCE_LSE,
        .LCDClockSelection = RCC_RTCCLKSOURCE_LSE,
    };
    ASSERT_HAL(HAL_RCCEx_PeriphCLKConfig(&periph_clk_init));
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_LCD_CLK_ENABLE();

#ifndef NDEBUG
    // Output the LSE at PB13 (= MCO3) for debugging purposes
    HAL_RCC_MCOConfig(RCC_MCO3, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
#endif

    // Init drivers
    display_init();
    dcf77_init();
}

/* override */ void SysTick_Handler(void) {
    HAL_IncTick();
}

extern void initialise_monitor_handles(void);

static RTC_HandleTypeDef rtc = {
    .Instance = RTC,
    .Init = {
        .HourFormat = RTC_HOURFORMAT_24,
        .AsynchPrediv = 127,  // divide 32768 by 128 = 256
        .SynchPrediv = 255  // divide 256 by 256 = 1
    }
};

int main(void) {
    initialise_monitor_handles();
    ASSERT_HAL(HAL_Init());

    ASSERT_HAL(HAL_RTC_Init(&rtc));

    return 0;
}
