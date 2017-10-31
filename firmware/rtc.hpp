#pragma once
#include "datetime.hpp"
#include <stm32l073xx.h>

// TODO encapsule RTC, EXTI and interrupt
class rtc {
public:
    rtc();

    rtc(const rtc&) = delete;
    rtc(rtc&&) = delete;
    rtc& operator=(const rtc&) = delete;
    rtc& operator=(rtc&&) = delete;

    bool second_pending() const;
    void clear_second_pending();

    datetime::datetime get_time() const;
    void set_time(const datetime::datetime& time);
};
