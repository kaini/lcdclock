#include "../datetime.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(datetime_tests)

BOOST_AUTO_TEST_CASE(equals_test) {
    datetime::datetime a(2017, 10, 30, 16, 5, 23);
    datetime::datetime b(a);

    BOOST_CHECK_EQUAL(a, b);
}

BOOST_AUTO_TEST_CASE(add_test) {
    datetime::datetime a(2000, 3, 1, 0, 1, 1);
    datetime::datetime b(a);
    datetime::datetime c(a);

    a.add_seconds(-62);
    b.add_minutes(-182);
    c.add_seconds(121);

    BOOST_CHECK_EQUAL(a, datetime::datetime(2000, 2, 29, 23, 59, 59));
    BOOST_CHECK_EQUAL(b, datetime::datetime(2000, 2, 29, 20, 59, 1));
    BOOST_CHECK_EQUAL(c, datetime::datetime(2000, 3, 1, 0, 3, 2));
}

BOOST_AUTO_TEST_CASE(is_leap_year_test) {
    BOOST_CHECK_EQUAL(datetime::is_leap_year(1996), true);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(1997), false);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(1998), false);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(1999), false);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(2000), true);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(2001), false);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(2002), false);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(2003), false);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(2004), true);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(2005), false);
    BOOST_CHECK_EQUAL(datetime::is_leap_year(2100), false);
}

BOOST_AUTO_TEST_SUITE_END()
