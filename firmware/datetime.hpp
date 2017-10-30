#pragma once

bool is_leap_year(int year);

int month_days(int month, int year);

class datetime {
public:
    datetime(int year, int month, int day, int hour, int minute, int second);

    void add_years(int years);
    void add_months(int months);
    void add_days(int days);
    void add_hours(int hours);
    void add_minutes(int minutes);
    void add_seconds(int seconds);

    int year() const { return m_year; }
    int month() const { return m_month; }
    int day() const { return m_day; }
    int hour() const { return m_hour; }
    int minute() const { return m_minute; }
    int second() const { return m_second; }

private:
    void assert_valid();

    int m_year = 0;
    int m_month = 0;
    int m_day = 0;
    int m_hour = 0;
    int m_minute = 0;
    int m_second = 0;
};
