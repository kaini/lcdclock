#include "display.h"
#include "dcf77.h"
#include "stm32l0xx.h"
#include "utils.h"
#include "dcf77_parser.h"
#include "rtc.h"
#include <string.h>
#include <time.h>

void power_clock_init(void) {
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

int main(void) {
	__disable_irq();
	power_clock_init();
    dcf77_init();
    display_init();
    rtc_init();
    setenv("TZ", "CET-1CEST-2,M3.5.0/2,M10.5.0/3", 1);
    tzset();
    __enable_irq();

    bool need_sync = true;
    bool syncing = false;
    dcf77_parser parser;
    int valid_frames = 0;

    while (true) {
    	struct tm now;
    	rtc_get_time(&now);
    	bool need_display_refresh = false;

    	if (need_sync && !syncing) {
			syncing = true;
			dcf77_parser_init(&parser);
			valid_frames = 0;
			dcf77_enable();
	    	display_content.dots[1] = true;
	    	need_display_refresh = true;
    	}

    	if (syncing && dcf77_samples_pending()) {
    		bool samples[DCF77_PARSER_SAMPLES_PER_SECOND];
    		dcf77_clear_samples_pending(samples);
			if (dcf77_parser_feed(&parser, samples)) {
			    struct tm received_time;
			    if (dcf77_parser_result(&parser, &received_time)) {
			        valid_frames += 1;
			        if (valid_frames >= 2) {
                        time_t time = mktime(&received_time);
                        struct tm utc_time;
                        gmtime_r(&time, &utc_time);
                        rtc_set_time(&utc_time);

                        need_sync = false;
                        syncing = false;
                        dcf77_disable();
                        display_content.dots[1] = false;
                        need_display_refresh = true;
                    }
			    } else {
			        valid_frames = 0;
			    }
			}
    	}

    	if (rtc_second_pending()) {
    		rtc_clear_second_pending();
			if (now.tm_hour < 10) {
				display_content.digits[0] = DISPLAY_DIGIT_NONE;
			} else {
				display_content.digits[0] = now.tm_hour / 10;
			}
			display_content.digits[1] = now.tm_hour % 10;
			display_content.digits[2] = now.tm_min / 10;
			display_content.digits[3] = now.tm_min % 10;
			display_content.digits[4] = now.tm_sec / 10;
			display_content.digits[5] = now.tm_sec % 10;
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
				rtc_second_pending();
    	if (!something_pending) {
    		__WFI();
    	}
    	__enable_irq();
    }
}
