#include "dcf77_parser.h"
#include "dcf77_10ms_pulses.h"
#include "utils.h"
#include <assert.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <stdio.h>

#define TWO(...) __VA_ARGS__, __VA_ARGS__
#define THREE(...) __VA_ARGS__, __VA_ARGS__, __VA_ARGS__
#define FIVE(...) __VA_ARGS__, __VA_ARGS__, __VA_ARGS__, __VA_ARGS__, __VA_ARGS__
#define NINE(...) THREE(THREE(__VA_ARGS__))
#define TEN(...) TWO(FIVE(__VA_ARGS__))
#define TWENTY(...) TWO(TEN(__VA_ARGS__))
#define EIGHTY(...) TEN(TWO(TWO(TWO(__VA_ARGS__))))
#define NINETY(...) TEN(THREE(THREE(__VA_ARGS__)))
#define HUNDRED(...) TEN(TEN(__VA_ARGS__))

#if 0
#define DEBUG_PRINTF(...) do { printf(__VA_ARGS__); } while (0)
#else
#define DEBUG_PRINTF(...) do { snprintf(NULL, 0, __VA_ARGS__); } while (0)
#endif

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

static char dcf77_bit_char(dcf77_bit bit) {
    switch (bit) {
        case DCF77_BIT_UNKNOWN:
            return '_';
        case DCF77_BIT_ZERO:
            return '0';
        case DCF77_BIT_ONE:
            return '1';
        case DCF77_BIT_MINUTE_MARK:
            return 'M';
        default:
            ASSERT(false);
            return '?';
    }
}

static void test_dcf77_parser_parse(void** state) {
    dcf77_parser parser;
    dcf77_parser_init(&parser);

    dcf77_result frame6;
    dcf77_result frame7;
    dcf77_result frame9;
    dcf77_result frame220;
    int valid_frames = 0;

    DEBUG_PRINTF("  0XXXXXXXXXXXXXXXXXXX1MMMMMMMpHHHHHHpDDDDDDWWWMMMMMJJJJJJJJPM\n");

    int prev_frame = 0;
    for (int y = 0; y < DCF77_10MS_PULSES_HEIGHT; ++y) {
        bool samples[DCF77_10MS_PULSES_WIDTH];
        for (int x = 0; x < DCF77_10MS_PULSES_WIDTH; ++x) {
            samples[x] = DCF77_10MS_PULSES[y][x] == ' ';
        }
        dcf77_parser_feed(&parser, samples);

        dcf77_result result = dcf77_parser_result(&parser);
        if (result.frame_number != prev_frame) {
            prev_frame = result.frame_number;

            switch (result.frame_number) {
                case 6:
                    frame6 = result;
                    break;
                case 7:
                    frame7 = result;
                    break;
                case 9:
                    frame9 = result;
                    break;
                case 220:
                    frame220 = result;
                    break;
            }

            DEBUG_PRINTF("+ ");
            for (int i = 0; i < DCF77_PARSER_BITS_PER_MINUTE; ++i) {
                DEBUG_PRINTF("%c", dcf77_bit_char(parser.new_bits[(i + parser.minute_mark + 1) % DCF77_PARSER_BITS_PER_MINUTE]));
            }
            DEBUG_PRINTF("\n= ");

            for (int i = 0; i < DCF77_PARSER_BITS_PER_MINUTE; ++i) {
                DEBUG_PRINTF("%c", dcf77_bit_char(parser.result.bits[i]));
            }
            DEBUG_PRINTF("    Valid: %c%c%c%c", result.layout_valid ? 'L' : ' ',
                                                result.minute_valid ? 'M' : ' ',
                                                result.hour_valid ? 'H' : ' ',
                                                result.date_valid ? 'D' : ' ');
            if (result.layout_valid && result.minute_valid && result.hour_valid && result.date_valid) {
                valid_frames += 1;
                DEBUG_PRINTF("    %02d.%02d.%04d %02d:%02d", result.day, result.month, result.year + 2000, result.hour, result.minute);
            }
            DEBUG_PRINTF("    Frame: %d\n", result.frame_number);
        }
    }

    assert_int_equal(frame6.year, 8);
    assert_int_equal(frame6.month, 7);
    assert_int_equal(frame6.day, 10);
    assert_int_equal(frame6.hour, 21);
    assert_int_equal(frame6.minute, 42);

    assert_int_equal(frame7.year, 8);
    assert_int_equal(frame7.month, 7);
    assert_int_equal(frame7.day, 10);
    assert_int_equal(frame7.hour, 21);
    assert_int_equal(frame7.minute, 43);

    assert_int_equal(frame9.year, 8);
    assert_int_equal(frame9.month, 7);
    assert_int_equal(frame9.day, 10);
    assert_int_equal(frame9.hour, -1);
    assert_int_equal(frame9.minute, -1);

    assert_int_equal(frame220.year, 8);
    assert_int_equal(frame220.month, 7);
    assert_int_equal(frame220.day, 11);
    assert_int_equal(frame220.hour, 1);
    assert_int_equal(frame220.minute, 16);

    assert_int_equal(valid_frames, 61);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dcf77_parser_second_sync),
        cmocka_unit_test(test_dcf77_parser_minute_mark),
        cmocka_unit_test(test_dcf77_parser_parse),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
