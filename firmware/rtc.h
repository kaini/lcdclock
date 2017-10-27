#pragma once
#include <stdbool.h>
#include <time.h>

void rtc_init(void);

bool rtc_second_pending(void);
void rtc_clear_second_pending(void);

void rtc_get_time(struct tm* tm);
void rtc_set_time(const struct tm* tm);
