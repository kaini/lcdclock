#include "output.hpp"
#include <iostream>

std::ostream& datetime::operator<<(std::ostream& out, const datetime& dt) {
    out << dt.year() << '-' << dt.month() << '-' << dt.day() << ' ' << dt.hour() << ':' << dt.minute() << ':' << dt.second();
    return out;
}
