#include "display.h"
#include "dcf77.h"
#include "stm32l0xx.h"
#include "utils.h"
#include "dcf77_parser.h"
#include <stdio.h>

extern void initialise_monitor_handles(void);

void system_init(void) {
	// Enable the power interface clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);

	// Go into the lowest power range (Range 3)
	while (READ_BIT(PWR->CSR, PWR_CSR_VOSF))
		;
	MODIFY_REG(PWR->CR, PWR_CR_VOS_Msk, 0b11 << PWR_CR_VOS_Pos);
	while (READ_BIT(PWR->CSR, PWR_CSR_VOSF))
		;

	// Set the MSE (multiple speed internal) oscillator to 1 MHz
	MODIFY_REG(RCC->ICSCR, RCC_ICSCR_MSIRANGE_Msk, RCC_ICSCR_MSIRANGE_4);
	while (!READ_BIT(RCC->CR, RCC_CR_MSIRDY))
		;

	// Disable the RTC domain write protection
	SET_BIT(PWR->CR, PWR_CR_DBP);

	// Enable the 2^15 Hz LSE (low speed external) crystal and drive it in high drive mode
	MODIFY_REG(RCC->CSR, RCC_CSR_LSEDRV_Msk, (0b11 << RCC_CSR_LSEDRV_Pos) | RCC_CSR_LSEON);
	while (!READ_BIT(RCC->CSR, RCC_CSR_LSERDY))
		;

	// Enable GPIO clocks
	SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN);
	SET_BIT(RCC->IOPSMENR, RCC_IOPSMENR_GPIOASMEN | RCC_IOPSMENR_GPIOBSMEN);

	// Enable TIM2 clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
}

int main(void) {
    initialise_monitor_handles();
    system_init();
    dcf77_init();

    dcf77_parser parser;
    dcf77_parser_init(&parser);

    dcf77_enable();

    dcf77_disable();

    return 0;
}
