#include "dcf77_parser.h"
#include "dcf77_10ms_pulses.h"
#include "utils.h"
#include <assert.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TWO(...) __VA_ARGS__, __VA_ARGS__
#define THREE(...) __VA_ARGS__, __VA_ARGS__, __VA_ARGS__
#define FIVE(...) __VA_ARGS__, __VA_ARGS__, __VA_ARGS__, __VA_ARGS__, __VA_ARGS__
#define NINE(...) THREE(THREE(__VA_ARGS__))
#define TEN(...) TWO(FIVE(__VA_ARGS__))
#define TWENTY(...) TWO(TEN(__VA_ARGS__))
#define EIGHTY(...) TEN(TWO(TWO(TWO(__VA_ARGS__))))
#define NINETY(...) TEN(THREE(THREE(__VA_ARGS__)))
#define HUNDRED(...) TEN(TEN(__VA_ARGS__))

static const bool MINUTE_MARK_SAMPLE[100] = { HUNDRED(1) };
static const bool ZERO_SAMPLE[100] = { TEN(0), NINETY(1) };
static const bool ONE_SAMPLE[100] = { TWENTY(0), EIGHTY(1) };

static void sync_second(dcf77_parser* parser) {
    for (int i = 0; i < 10; ++i) {
        dcf77_parser_feed(parser, (i % 2 == 0) ? ZERO_SAMPLE : ONE_SAMPLE);
    }
}

static void test_dcf77_parser_second_sync(void** state) {
    dcf77_parser parser;
    dcf77_parser_init(&parser);
    assert_int_equal(parser.second_start, -1);

    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            bool sample[100] = { TEN(1), TEN(0), EIGHTY(1) };
            dcf77_parser_feed(&parser, sample);
        } else {
            bool sample[100] = { NINE(1), TEN(0), EIGHTY(1), 0 };
            dcf77_parser_feed(&parser, sample);
        }
    }
    assert_int_equal(parser.second_start, 9);

    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            bool sample[100] = { 1, 0, 1, 0, 1, TEN(0), FIVE(1), EIGHTY(1) };
            dcf77_parser_feed(&parser, sample);
        } else {
            bool sample[100] = { 0, 1, 0, 1, 0, TEN(0), FIVE(1), EIGHTY(1) };
            dcf77_parser_feed(&parser, sample);
        }
    }
    assert_int_equal(parser.second_start, 5);
}

static void test_dcf77_parser_minute_mark(void** state) {
    dcf77_parser parser;
    dcf77_parser_init(&parser);
    sync_second(&parser);
    assert_int_equal(parser.minute_mark, -1);

    dcf77_parser_feed(&parser, MINUTE_MARK_SAMPLE);
    assert_int_equal(parser.minute_mark, 1);

    for (int i = 0; i < 59; ++i) {
        dcf77_parser_feed(&parser, ONE_SAMPLE);
    }
    dcf77_parser_feed(&parser, MINUTE_MARK_SAMPLE);
    dcf77_parser_feed(&parser, MINUTE_MARK_SAMPLE);
    assert_int_equal(parser.minute_mark, 1);

    for (int i = 0; i < 59; ++i) {
        dcf77_parser_feed(&parser, ONE_SAMPLE);
    }
    dcf77_parser_feed(&parser, MINUTE_MARK_SAMPLE);
    assert_int_equal(parser.minute_mark, 2);

    for (int i = 0; i < 1000; ++i) {
        for (int i = 0; i < 59; ++i) {
            dcf77_parser_feed(&parser, ONE_SAMPLE);
        }
        dcf77_parser_feed(&parser, MINUTE_MARK_SAMPLE);
        assert_int_equal(parser.minute_mark, 2);
    }
}

static void test_dcf77_parser_parse(void** state) {
    dcf77_parser parser;
    dcf77_parser_init(&parser);

    struct tm frame6;
    struct tm frame7;
    struct tm frame220;
    int valid_frames = 0;
    int frame_number = 0;
    for (int y = 0; y < DCF77_10MS_PULSES_HEIGHT; ++y) {
        bool samples[DCF77_10MS_PULSES_WIDTH];
        for (int x = 0; x < DCF77_10MS_PULSES_WIDTH; ++x) {
            samples[x] = DCF77_10MS_PULSES[y][x] == ' ';
        }

        if (dcf77_parser_feed(&parser, samples)) {
            frame_number += 1;
            struct tm result;
            if (dcf77_parser_result(&parser, &result)) {
                valid_frames += 1;
                switch (frame_number) {
                    case 6:
                        frame6 = result;
                        break;
                    case 7:
                        frame7 = result;
                        break;
                    case 220:
                        frame220 = result;
                        break;
                }
            }
        }
    }

    assert_int_equal(valid_frames, 57);

    assert_int_equal(frame6.tm_year, 108);
    assert_int_equal(frame6.tm_mon, 7);
    assert_int_equal(frame6.tm_mday, 10);
    assert_int_equal(frame6.tm_hour, 21);
    assert_int_equal(frame6.tm_min, 42);
    assert_true(frame6.tm_isdst);

    assert_int_equal(frame7.tm_year, 108);
    assert_int_equal(frame7.tm_mon, 7);
    assert_int_equal(frame7.tm_mday, 10);
    assert_int_equal(frame7.tm_hour, 21);
    assert_int_equal(frame7.tm_min, 43);
    assert_true(frame7.tm_isdst);

    assert_int_equal(frame220.tm_year, 108);
    assert_int_equal(frame220.tm_mon, 7);
    assert_int_equal(frame220.tm_mday, 11);
    assert_int_equal(frame220.tm_hour, 1);
    assert_int_equal(frame220.tm_min, 16);
    assert_true(frame220.tm_isdst);
}

int main(void) {
    putenv("TZ=CET-1CEST-2,M3.5.0/2,M10.5.0/3");
    tzset();
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dcf77_parser_second_sync),
        cmocka_unit_test(test_dcf77_parser_minute_mark),
        cmocka_unit_test(test_dcf77_parser_parse),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
