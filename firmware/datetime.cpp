#include "datetime.hpp"
#include "utils.hpp"
#include <tuple>
#include <iostream>

// :(
// https://stackoverflow.com/questions/12089514/real-modulo-operator-in-c-c
static int mod(int a, int b) {
    ASSERT(b > 0);
    int result = a % b;
    return result + b * (result < 0);
}

static int floor_div(int a, int b) {
    ASSERT(b > 0);
    return a / b - (a < 0 && a % b != 0);
}

bool is_leap_year(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int month_days(int month, int year) {
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        return is_leap_year(year) ? 29 : 28;
    default:
        ASSERT(false);
        return 0;
    }
}

datetime::datetime(int year, int month, int day, int hour, int minute, int second) :
    m_year(year), m_month(month), m_day(day), m_hour(hour), m_minute(minute), m_second(second) {
    if (!is_valid()) {
        throw invalid_datetime_exception();
    }
}

void datetime::add_days(int days) {
    m_day += days;

    while (m_day > month_days(m_month, m_year)) {
        m_day -= month_days(m_month, m_year);
        m_month += 1;
        if (m_month > 12) {
            m_month = 1;
            m_year += 1;
        }
    }

    while (m_day < 1) {
        m_month -= 1;
        if (m_month < 1) {
            m_month = 12;
            m_year -= 1;
        }
        m_day += month_days(m_month, m_year);
    }

    ASSERT(is_valid());
}

void datetime::add_hours(int hours) {
    m_hour += hours;
    int days = floor_div(m_hour, 24);
    m_hour = mod(m_hour, 24);
    if (days != 0) {
        add_days(days);
    }
    ASSERT(is_valid());
}

void datetime::add_minutes(int minutes) {
    m_minute += minutes;
    int hours = floor_div(m_minute, 60);
    m_minute = mod(m_minute, 60);
    if (hours != 0) {
        add_hours(hours);
    }
    ASSERT(is_valid());
}

void datetime::add_seconds(int seconds) {
    m_second += seconds;
    int minutes = floor_div(m_second, 60);
    m_second = mod(m_second, 60);
    if (minutes != 0) {
        add_minutes(minutes);
    }
    ASSERT(is_valid());
}

bool datetime::is_valid() const {
    return
        m_year >= 0 &&
        1 <= m_month && m_month <= 12 &&
        1 <= m_day && m_day <= month_days(m_month, m_year) &&
        0 <= m_hour && m_hour <= 23 &&
        0 <= m_minute && m_minute <= 59 &&
        0 <= m_second && m_second <= 59;
}

bool operator==(const datetime& a, const datetime& b) {
    return
        a.year() == b.year() &&
        a.month() == b.month() &&
        a.day() == b.day() &&
        a.hour() == b.hour() &&
        a.minute() == b.minute() &&
        a.second() == b.second();
}

bool operator<(const datetime& a, const datetime& b) {
    return
        std::make_tuple(a.year(), a.month(), a.day(), a.hour(), a.minute(), a.second()) <
        std::make_tuple(b.year(), b.month(), b.day(), b.hour(), b.minute(), b.second());
}

std::ostream& operator<<(std::ostream& out, const datetime& dt) {
    out << dt.year() << '-' << dt.month() << '-' << dt.day() << ' ' << dt.hour() << ':' << dt.minute() << ':' << dt.second();
    return out;
}

