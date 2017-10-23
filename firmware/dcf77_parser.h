// Parses a DCF77 signal. Inspired by http://www.gjlay.de/software/dcf77/konzept.html.
#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define DCF77_PARSER_SAMPLES_PER_SECOND 100
#define DCF77_PARSER_BITS_PER_MINUTE 60

typedef enum dcf77_bit {
    DCF77_BIT_UNKNOWN = 0,
    DCF77_BIT_ZERO,
    DCF77_BIT_ONE,
    DCF77_BIT_MINUTE_MARK,
} dcf77_bit;

typedef struct dcf77_parser {
    int8_t second_sync_samples[DCF77_PARSER_SAMPLES_PER_SECOND];
    int second_sync_count;
    int second_start;

    int bit_at;

    int8_t minute_marks[DCF77_PARSER_BITS_PER_MINUTE];
    int minute_mark;

    dcf77_bit new_bits[DCF77_PARSER_BITS_PER_MINUTE];
    union {
        dcf77_bit bits[DCF77_PARSER_BITS_PER_MINUTE];
        struct {
            dcf77_bit zero;
            dcf77_bit ignored0[14 + 1 + 1]; // weather data + callbit + DST in next hour
            dcf77_bit in_dst;
            dcf77_bit not_in_dst;
            dcf77_bit ignored1[1]; // leap second bit
            dcf77_bit one;
            union {
                dcf77_bit minute_bits[4 + 3];
                struct {
                    dcf77_bit minute_ones[4];
                    dcf77_bit minute_tens[3];
                };
            };
            dcf77_bit minute_parity;
            union {
                dcf77_bit hour_bits[4 + 2];
                struct {
                    dcf77_bit hour_ones[4];
                    dcf77_bit hour_tens[2];
                };
            };
            dcf77_bit hour_parity;
            union {
                dcf77_bit date_bits[4 + 2 + 3 + 4 + 1 + 4 + 4];
                struct {
                    dcf77_bit day_ones[4];
                    dcf77_bit day_tens[2];
                    dcf77_bit weekday_ones[3];
                    dcf77_bit month_ones[4];
                    dcf77_bit month_tens[1];
                    dcf77_bit year_ones[4];
                    dcf77_bit year_tens[4];
                };
            };
            dcf77_bit date_parity;
            dcf77_bit minute_mark;
        };
    } result;
    int frame_number;
} dcf77_parser;

typedef struct dcf77_result {
    int frame_number;

    bool layout_valid;
    bool minute_valid;
    bool hour_valid;
    bool date_valid;

    int minute;
    int hour;
    int day;
    int month;
    int year;
    bool dst;
} dcf77_result;

// Initialize a dcf77_parser structure. There is no need to free it.
void dcf77_parser_init(dcf77_parser* parser);

// Feeds one second of samples into the parser. Returns true once a new frame is ready.
bool dcf77_parser_feed(dcf77_parser* parser, const bool* samples);

// Gets the current result.
dcf77_result dcf77_parser_result(const dcf77_parser* parser);

