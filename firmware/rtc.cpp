#include "rtc.hpp"
#include "utils.hpp"
#include <stm32l0xx.h>

static volatile bool second_pending = false;

rtc::rtc() {
    // Unlock the RTC registers
    WRITE_REG(RTC->WPR, 0xCA);
    WRITE_REG(RTC->WPR, 0x53);
    // Wait for init mode
    SET_BIT(RTC->ISR, RTC_ISR_INIT);
    while (!READ_BIT(RTC->ISR, RTC_ISR_INITF))
        ;

    // Bypass the shadow registers to read directly from the (inconsistent) registers
    SET_BIT(RTC->CR, RTC_CR_BYPSHAD);

    // 2^15 / 128 = 256 Hz
    // 256 / 256 = 1 Hz
    MODIFY_REG(
            RTC->PRER,
            RTC_PRER_PREDIV_A_Msk | RTC_PRER_PREDIV_S_Msk,
            (0b1111111 << RTC_PRER_PREDIV_A_Pos) | (0b11111111 << RTC_PRER_PREDIV_S_Pos));

    // Interrupt every second using alarm A
    // Enter write mode for alarm A
    CLEAR_BIT(RTC->CR, RTC_CR_ALRAE);
    while (!READ_BIT(RTC->ISR, RTC_ISR_ALRAWF))
        ;
    // Match every second
    MODIFY_REG(RTC->ALRMAR, UINT32_MAX, RTC_ALRMAR_MSK1 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK4);
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 200);
    SET_BIT(EXTI->IMR, EXTI_IMR_IM17);
    SET_BIT(EXTI->EMR, EXTI_EMR_EM17);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_RT17);
    SET_BIT(RTC->CR, RTC_CR_ALRAIE | RTC_CR_ALRAE);

    // Quit init mode
    CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);
    // Lock the RTC registers
    WRITE_REG(RTC->WPR, 0xFE);
    WRITE_REG(RTC->WPR, 0x64);
}

bool rtc::second_pending() const {
	return ::second_pending;
}

void rtc::clear_second_pending() {
	::second_pending = false;
}

datetime rtc::get_time() const {
	uint32_t time_register = 0;
	uint32_t time_register_read = 0;
	uint32_t date_register = 0;
	uint32_t date_register_read = 0;
	do {
		time_register = time_register_read;
		date_register = date_register_read;
		time_register_read = READ_REG(RTC->TR);
		date_register_read = READ_REG(RTC->DR);
	} while (time_register != time_register_read || date_register != date_register_read);

	int year = 2000 + ((date_register & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos) * 10 + ((date_register & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos);
	int month = ((date_register & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos) * 10 + ((date_register & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos);
    int day = ((date_register & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos) * 10 + ((date_register & RTC_DR_DU_Msk) >> RTC_DR_DU_Pos);
	int hour = ((time_register & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos) * 10 + ((time_register & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos);
	int minute = ((time_register & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos) * 10 + ((time_register & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos);
	int second = ((time_register & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos) * 10 + ((time_register & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos);

    return datetime(year, month, day, hour, minute, second);
}

void rtc::set_time(const datetime& tm) {
	// Unlock the RTC registers
	WRITE_REG(RTC->WPR, 0xCA);
	WRITE_REG(RTC->WPR, 0x53);
	// Wait for init mode
	SET_BIT(RTC->ISR, RTC_ISR_INIT);
	while (!READ_BIT(RTC->ISR, RTC_ISR_INITF))
		;

	uint32_t tr_set = 0;
	tr_set |= (tm.hour() / 10) << RTC_TR_HT_Pos;
	tr_set |= (tm.hour() % 10) << RTC_TR_HU_Pos;
	tr_set |= (tm.minute() / 10) << RTC_TR_MNT_Pos;
	tr_set |= (tm.minute() % 10) << RTC_TR_MNU_Pos;
	tr_set |= (tm.second() / 10) << RTC_TR_ST_Pos;
	tr_set |= (tm.second() % 10) << RTC_TR_SU_Pos;
	MODIFY_REG(
			RTC->TR,
			RTC_TR_HT_Msk | RTC_TR_HU_Msk | RTC_TR_MNT_Msk | RTC_TR_MNU_Msk | RTC_TR_ST_Msk | RTC_TR_SU_Msk,
			tr_set);

	uint32_t dr_set = 0;
	dr_set |= ((tm.year() - 2000) / 10) << RTC_DR_YT_Pos;
	dr_set |= ((tm.year() - 2000) % 10) << RTC_DR_YU_Pos;
	dr_set |= (tm.month() / 10) << RTC_DR_MT_Pos;
	dr_set |= (tm.month() % 10) << RTC_DR_MU_Pos;
	dr_set |= (tm.day() / 10) << RTC_DR_DT_Pos;
	dr_set |= (tm.day() % 10) << RTC_DR_DU_Pos;
	MODIFY_REG(
			RTC->DR,
			RTC_DR_YT_Msk | RTC_DR_YU_Msk | RTC_DR_MT_Msk | RTC_DR_MU_Msk | RTC_DR_DT_Msk | RTC_DR_DU_Msk,
			dr_set);

	// Quit init mode
	CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);
	// Lock the RTC registers
	WRITE_REG(RTC->WPR, 0xFE);
	WRITE_REG(RTC->WPR, 0x64);
}

extern "C" void RTC_IRQHandler() {
	if (READ_BIT(RTC->ISR, RTC_ISR_ALRAF)) {
		CLEAR_BIT(RTC->ISR, RTC_ISR_ALRAF);
		if (READ_BIT(EXTI->PR, EXTI_PR_PIF17)) {
			SET_BIT(EXTI->PR, EXTI_PR_PIF17);
			second_pending = true;
		}
	}
}