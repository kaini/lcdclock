#pragma once
#include "datetime.hpp"
#include <stm32l073xx.h>

namespace hw {

class rtc {
public:
    // RTC is the RTC controller.
    rtc(RTC_TypeDef* rtc);

    // Call this function in the RTC ALARM A interrupt (RTC IRQ, External Interrupt 17)
    void irq_alarm_a() { m_second_pending = true; }

    rtc(const rtc&) = delete;
    rtc(rtc&&) = delete;
    rtc& operator=(const rtc&) = delete;
    rtc& operator=(rtc&&) = delete;

    bool second_pending() const { return m_second_pending; }
    void clear_second_pending() { m_second_pending = false; }

    datetime::datetime get_time() const;
    void set_time(const datetime::datetime& time);

private:
    RTC_TypeDef* m_rtc;
    volatile bool m_second_pending = false;
};

}
