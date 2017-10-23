#pragma once
#include <stdbool.h>

typedef struct rtc_time {
	int year;
	int month;
	int day;

	int hour;
	int minute;
	int second;

	bool dst;
} rtc_time;

void rtc_init(void);

bool rtc_second_pending(void);
void rtc_clear_second_pending(void);

rtc_time rtc_get_time(void);
void rtc_set_time(const rtc_time* time);
