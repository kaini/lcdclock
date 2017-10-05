// Parses a DCF77 signal. Inspired by http://www.gjlay.de/software/dcf77/konzept.html.
#pragma once
#include <stdbool.h>
#include <stdlib.h>

#define DCF77_PARSER_SAMPLES_PER_SECOND 100

// Do not change this number, the algorithm expects this to be three minutes.
// Yes, I know, the parser will fail during a leap second ...
#define DCF77_PARSER_BIT_COUNT 180

typedef enum dcf77_bit {
    DCF77_BIT_UNKNOWN = 0,
    DCF77_BIT_ZERO,
    DCF77_BIT_ONE,
    DCF77_BIT_MINUTE_MARK,
} dcf77_bit;

typedef union dcf77_parser {
    struct header {
        bool (*feed)(union dcf77_parser* parser, const bool* samples);
    } header;
    
    struct {
        struct header header;
        unsigned char samples[DCF77_PARSER_SAMPLES_PER_SECOND];
        int seconds;
    } find_second;

    struct {
        struct header header;
        bool samples[DCF77_PARSER_SAMPLES_PER_SECOND];
        bool first;
        size_t offset;
        dcf77_bit bits[DCF77_PARSER_BIT_COUNT];
        size_t bit_at;
    } collect_bits;
} dcf77_parser;

void dcf77_parser_init(dcf77_parser* parser);
bool dcf77_parser_feed(dcf77_parser* parser, const bool* samples);
