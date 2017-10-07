#include "display.h"
#include "dcf77.h"
#include "stm32l0xx.h"
#include "my_assert.h"

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
    HAL_StatusTypeDef result = HAL_RCC_OscConfig(&osc_init);
    ASSERT(result == HAL_OK);

    // Setup the system clock and all peripheral clocks to 1 MHz based on the MSI
    RCC_ClkInitTypeDef clk_init = {
        .ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource = RCC_SYSCLKSOURCE_MSI,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV1,
        .APB2CLKDivider = RCC_HCLK_DIV1,
    };
    result = HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_0);
    ASSERT(result == HAL_OK);

    // Clock RTC by external crystal
    // Note: The LCD clock is driven by the RTC clock
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
    __HAL_RCC_RTC_ENABLE();

    // Output the LSE at PB13 (= MCO3)
    HAL_RCC_MCOConfig(RCC_MCO3, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);

    // Init drivers
    display_init();
    dcf77_init();
}

int main(void) {
    HAL_StatusTypeDef result = HAL_Init();
    ASSERT(result == HAL_OK);
    return 0;
}
