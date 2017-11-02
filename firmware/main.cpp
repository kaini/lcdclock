#include "dcf77.hpp"
#include "dcf77_parser.hpp"
#include "display.hpp"
#include "rtc.hpp"
#include "utils.hpp"
#include "critical_section.hpp"
#include <stm32l0xx.h>

// https://andriidevel.blogspot.co.at/2016/05/size-cost-of-c-exception-handling-on.html
// This removes the default terminate handler which unfortunately pulls a lot of pretty printing
// functions that amount to about 60 kB of code. Also we cannot output stuff, therefore who
// cares.
namespace __cxxabiv1 {
    std::terminate_handler __terminate_handler = abort;
}

namespace {

struct power_and_clocks {
    power_and_clocks() {
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
};

}

static power_and_clocks power_and_clocks;
static hw::rtc rtc(RTC);
static hw::display display(LCD, {{GPIOA, {1, 2, 3, 6, 7, 8, 9, 10}}, {GPIOB, {0, 1, 3, 4, 5, 9, 10, 11}}});
static hw::dcf77 dcf(GPIOB, 6, GPIOB, 7, TIM2);

int main() {
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 200);
    SET_BIT(EXTI->IMR, EXTI_IMR_IM17);
    SET_BIT(EXTI->EMR, EXTI_EMR_EM17);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_RT17);

    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 20);

    bool need_sync = true;
    bool syncing = false;
    bool first_iteration = true;
    dcf77::parser parser;

    while (true) {
        auto now = rtc.get_time();

    	if (need_sync && !syncing) {
			syncing = true;
			parser = dcf77::parser();
			dcf.enable();
			display.set_dot(1, true);
    	}

    	if (syncing && dcf.samples_pending()) {
    		auto samples = dcf.clear_pending_samples();
    		for (int b : samples) {
    		    DEBUG_PRINTF("%d", b);
    		}
    		DEBUG_PRINTF("\n");
			if (parser.feed(&samples[0])) {
			    if (auto frame = parser.get_result()) {
                    rtc.set_time(*frame);
                    need_sync = false;
                    syncing = false;
                    dcf.disable();
                    display.set_dot(1, false);
			    }
			}
    	}

    	if (first_iteration || rtc.second_pending()) {
    		rtc.clear_second_pending();

    		auto localtime = now;
    		if (datetime::is_eu_dst(localtime)) {
    		    localtime.add_hours(2);
    		} else {
    		    localtime.add_hours(1);
    		}

			if (localtime.hour() < 10) {
				display.set_digit(0, hw::display_digit::none);
			} else {
			    display.set_digit(0, static_cast<hw::display_digit>(localtime.hour() / 10));
			}
			display.set_digit(1, static_cast<hw::display_digit>(localtime.hour() % 10));
			display.set_digit(2, static_cast<hw::display_digit>(localtime.minute() / 10));
			display.set_digit(3, static_cast<hw::display_digit>(localtime.minute() % 10));
			display.set_digit(4, static_cast<hw::display_digit>(localtime.second() / 10));
			display.set_digit(5, static_cast<hw::display_digit>(localtime.second() % 10));
			display.set_colon(true);
    	}

    	display.refresh();
    	first_iteration = false;

        // Done... for now.
    	__disable_irq();
        bool something_pending =
                (syncing && dcf.samples_pending()) ||
                rtc.second_pending();
        if (!something_pending) {
            __WFI();
        }
        __enable_irq();
    }

    return 0;
}

extern "C" void RTC_IRQHandler() {
    if (READ_BIT(RTC->ISR, RTC_ISR_ALRAF)) {
        {
            // Unfortunately the ISR register contains one read-write bit, that could be
            // changed. Therefore all non-atomic writes to this register have to be in a
            // critical section.
            hw::critical_section cs;
            CLEAR_BIT(RTC->ISR, RTC_ISR_ALRAF);
        }
        if (READ_BIT(EXTI->PR, EXTI_PR_PIF17)) {
            WRITE_REG(EXTI->PR, EXTI_PR_PIF17);
            rtc.irq_alarm_a();
        }
    }
}

extern "C" void TIM2_IRQHandler() {
    if (READ_BIT(TIM2->SR, TIM_SR_UIF)) {
        WRITE_REG(TIM2->SR, ~TIM_SR_UIF);
        dcf.irq_timer();
    }
}
