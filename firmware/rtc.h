#pragma once

typedef struct rtc_time {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} rtc_time;

void rtc_init(void);
rtc_time rtc_get_time(void);
void rtc_set_time(const rtc_time* time);
