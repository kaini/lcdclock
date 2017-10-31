#pragma once
#include <iosfwd>
#include <stdexcept>

bool is_leap_year(int year);

int month_days(int month, int year);

class invalid_datetime_exception : public std::exception {
};

class datetime {
public:
    datetime() : datetime(1970, 1, 1, 0, 0, 0) {}
    datetime(int year, int month, int day, int hour, int minute, int second);

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
    bool is_valid() const;

    int m_year;
    int m_month;
    int m_day;
    int m_hour;
    int m_minute;
    int m_second;
};

bool operator==(const datetime& a, const datetime& b);
inline bool operator!=(const datetime& a, const datetime& b) { return !(a == b); }

bool operator<(const datetime& a, const datetime& b);
inline bool operator<=(const datetime& a, const datetime& b) { return a < b || a == b; }
inline bool operator>=(const datetime& a, const datetime& b) { return !(a < b); }
inline bool operator>(const datetime& a, const datetime& b) { return !(a <= b); }

std::ostream& operator<<(std::ostream& out, const datetime& dt);
