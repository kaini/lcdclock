#include "../dcf77_parser.hpp"
#include "dcf77_10ms_pulses.hpp"
#include <boost/test/unit_test.hpp>

#define TWO(...) __VA_ARGS__, __VA_ARGS__
#define THREE(...) __VA_ARGS__, __VA_ARGS__, __VA_ARGS__
#define FIVE(...) __VA_ARGS__, __VA_ARGS__, __VA_ARGS__, __VA_ARGS__, __VA_ARGS__
#define NINE(...) THREE(THREE(__VA_ARGS__))
#define TEN(...) TWO(FIVE(__VA_ARGS__))
#define TWENTY(...) TWO(TEN(__VA_ARGS__))
#define EIGHTY(...) TEN(TWO(TWO(TWO(__VA_ARGS__))))
#define NINETY(...) TEN(THREE(THREE(__VA_ARGS__)))
#define HUNDRED(...) TEN(TEN(__VA_ARGS__))

BOOST_AUTO_TEST_SUITE(dcf77_parser_tests)

BOOST_AUTO_TEST_CASE(bit_sync_test) {
    dcf77::bit_sync bit_sync;
    BOOST_CHECK_EQUAL(bit_sync.bit_start(), -1);

    for (size_t i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            bool sample[100] = { TEN(1), TEN(0), EIGHTY(1) };
            bit_sync.feed(sample);
        } else {
            bool sample[100] = { NINE(1), TEN(0), EIGHTY(1), 0 };
            bit_sync.feed(sample);
        }
    }
    BOOST_CHECK_EQUAL(bit_sync.bit_start(), 9);

    for (size_t i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            bool sample[100] = { 1, 0, 1, 0, 1, TEN(0), FIVE(1), EIGHTY(1) };
            bit_sync.feed(sample);
        } else {
            bool sample[100] = { 0, 1, 0, 1, 0, TEN(0), FIVE(1), EIGHTY(1) };
            bit_sync.feed(sample);
        }
    }
    BOOST_CHECK_EQUAL(bit_sync.bit_start(), 5);
}

BOOST_AUTO_TEST_CASE(frame_sync_test) {
    dcf77::frame_sync frame_sync;
    BOOST_CHECK_EQUAL(frame_sync.frame_start(), -1);

    frame_sync.feed(dcf77::bit::one);
    frame_sync.feed(dcf77::bit::minute_mark);
    BOOST_CHECK_EQUAL(frame_sync.frame_start(), 1);

    for (size_t i = 0; i < 59; ++i) {
        frame_sync.feed(dcf77::bit::one);
    }
    frame_sync.feed(dcf77::bit::minute_mark);
    frame_sync.feed(dcf77::bit::minute_mark);
    BOOST_CHECK_EQUAL(frame_sync.frame_start(), 1);

    for (size_t i = 0; i < 59; ++i) {
        frame_sync.feed(dcf77::bit::one);
    }
    frame_sync.feed(dcf77::bit::minute_mark);
    BOOST_CHECK_EQUAL(frame_sync.frame_start(), 1);

    for (size_t i = 0; i < 1000; ++i) {
        for (size_t i = 0; i < 59; ++i) {
            frame_sync.feed(dcf77::bit::one);
        }
        frame_sync.feed(dcf77::bit::minute_mark);
        BOOST_CHECK_EQUAL(frame_sync.frame_start(), 2);
    }
}

BOOST_AUTO_TEST_CASE(parser_testdata_test) {
    dcf77::parser parser;

    datetime::datetime frame6;
    datetime::datetime frame7;
    datetime::datetime frame220;
    int valid_frames = 0;
    int frame_number = 0;
    for (size_t y = 0; y < DCF77_10MS_PULSES_HEIGHT; ++y) {
        bool samples[DCF77_10MS_PULSES_WIDTH];
        for (size_t x = 0; x < DCF77_10MS_PULSES_WIDTH; ++x) {
            samples[x] = DCF77_10MS_PULSES[y][x] == ' ';
        }

        if (parser.feed(samples)) {
            frame_number += 1;
            if (auto result = parser.get_result()) {
                valid_frames += 1;
                switch (frame_number) {
                    case 6:
                        frame6 = *result;
                        break;
                    case 7:
                        frame7 = *result;
                        break;
                    case 220:
                        frame220 = *result;
                        break;
                }
            }
        }
    }

    BOOST_CHECK_EQUAL(valid_frames, 58);
    BOOST_CHECK_EQUAL(frame6, datetime::datetime(2008, 7, 10, 19, 43, 0));
    BOOST_CHECK_EQUAL(frame7, datetime::datetime(2008, 7, 10, 19, 44, 0));
    BOOST_CHECK_EQUAL(frame220, datetime::datetime(2008, 7, 10, 23, 17, 0));
}

BOOST_AUTO_TEST_SUITE_END()
