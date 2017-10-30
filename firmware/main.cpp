#include "dcf77.hpp"
#include "dcf77_parser.hpp"
#include "display.hpp"
#include "rtc.hpp"
#include "utils.hpp"
#include <stm32l0xx.h>

void power_clock_init() {
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

	// Set the RTC/LCD clock to the LSE and enable the RTC
	MODIFY_REG(RCC->CSR, RCC_CSR_RTCSEL_Msk, (0b01 << RCC_CSR_RTCSEL_Pos) | RCC_CSR_RTCEN);

	// Enable LCD clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_LCDEN);
	SET_BIT(RCC->APB1SMENR, RCC_APB1SMENR_LCDSMEN);

	// Enable GPIO clocks
	SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN);
	SET_BIT(RCC->IOPSMENR, RCC_IOPSMENR_GPIOASMEN | RCC_IOPSMENR_GPIOBSMEN);

	// Enable TIM2 clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
	SET_BIT(RCC->APB1SMENR, RCC_APB1SMENR_TIM2SMEN);
}

int main() {
    __disable_irq();
    power_clock_init();
    dcf77_init();
    display_init();
    rtc rtc;
    __enable_irq();

    bool need_sync = true;
    bool syncing = false;
    dcf77_parser parser;

    while (true) {
        auto now = rtc.get_time();
    	bool need_display_refresh = false;

    	if (need_sync && !syncing) {
			syncing = true;
			parser = dcf77_parser();
			dcf77_enable();
	    	display_content.dots[1] = true;
	    	need_display_refresh = true;
    	}

    	if (syncing && dcf77_samples_pending()) {
    		bool samples[dcf77_parser::samples_per_second];
    		dcf77_clear_samples_pending(samples);
			if (auto frame = parser.feed(samples)) {
			    rtc.set_time(*frame);
                need_sync = false;
                syncing = false;
                dcf77_disable();
                display_content.dots[1] = false;
                need_display_refresh = true;
			}
    	}

    	if (rtc.second_pending()) {
    		rtc.clear_second_pending();

    		datetime localtime = now;
			if (localtime.hour() < 10) {
				display_content.digits[0] = DISPLAY_DIGIT_NONE;
			} else {
				display_content.digits[0] = static_cast<display_digit>(localtime.hour() / 10);
			}
			display_content.digits[1] = static_cast<display_digit>(localtime.hour() % 10);
			display_content.digits[2] = static_cast<display_digit>(localtime.minute() / 10);
			display_content.digits[3] = static_cast<display_digit>(localtime.minute() % 10);
			display_content.digits[4] = static_cast<display_digit>(localtime.second() / 10);
			display_content.digits[5] = static_cast<display_digit>(localtime.second() % 10);
			display_content.colon = true;
			need_display_refresh = true;
    	}

    	if (need_display_refresh) {
    		display_refresh();
    	}

        // Done... for now.
    	__disable_irq();
        bool something_pending =
                (syncing && dcf77_samples_pending()) ||
                rtc.second_pending();
        if (!something_pending) {
            __WFI();
        }
        __enable_irq();
    }
}
