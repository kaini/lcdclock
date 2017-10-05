#include "dcf77_parser.h"
#include <string.h>

static bool feed_collect_bits(dcf77_parser* parser, const bool* samples) {
    if (parser->collect_bits.first) {
        parser->collect_bits.first = false;
    } else {
        memcpy(&parser->collect_bits.samples[DCF77_PARSER_SAMPLES_PER_SECOND - parser->collect_bits.offset],
               &samples[0],
               parser->collect_bits.offset * sizeof(samples[0]));
            
        // Check the last 200 ms.
        int high_count = 0;
        for (size_t i = DCF77_PARSER_SAMPLES_PER_SECOND * 4 / 5; i < DCF77_PARSER_SAMPLES_PER_SECOND; ++i) {
            high_count += parser->collect_bits.samples[i];
        }

        if (high_count <= 2) {
            parser->collect_bits.bits[parser->collect_bits.bit_at] = DCF77_BIT_MINUTE_MARK;
        } else if (8 <= high_count && high_count <= 12) {
            parser->collect_bits.bits[parser->collect_bits.bit_at] = DCF77_BIT_MINUTE_MARK;
        } else if (18 <= high_count) {
            parser->collect_bits.bits[parser->collect_bits.bit_at] = DCF77_BIT_MINUTE_MARK;
        } else {
            parser->collect_bits.bits[parser->collect_bits.bit_at] = DCF77_BIT_UNKNOWN;
        }
        parser->collect_bits.bit_at += 1;
    }

    memcpy(&parser->collect_bits.samples[0],
           &samples[parser->collect_bits.offset],
           (DCF77_PARSER_SAMPLES_PER_SECOND - parser->collect_bits.offset) * sizeof(samples[0]));

    return false;
}

static void switch_to_collect_bits(dcf77_parser* parser, size_t offset) {
    parser->header.feed = &feed_collect_bits;
    memset(parser->collect_bits.samples, 0, sizeof(parser->collect_bits.samples));
    parser->collect_bits.first = true;
    parser->collect_bits.offset = offset;
    parser->collect_bits.bit_at = 0;
}

static bool feed_find_second(dcf77_parser* parser, const bool* samples) {
    for (size_t i = 0; i < DCF77_PARSER_SAMPLES_PER_SECOND; ++i) {
        parser->find_second.samples[i] += samples[i];
    }
    parser->find_second.seconds += 1;
    if (parser->find_second.seconds == 5) {
        size_t maximum_index = 0;
        unsigned char maximum = 0;
        for (size_t i = 0; i < DCF77_PARSER_SAMPLES_PER_SECOND; ++i) {
            if (parser->find_second.samples[i] >= maximum) {
                maximum = parser->find_second.samples[i];
                maximum_index = i;
            }
        }
        switch_to_collect_bits(parser, (maximum_index + 1) % DCF77_PARSER_SAMPLES_PER_SECOND);
    }
    return false;
}

static void switch_to_find_second(dcf77_parser* parser) {
    parser->header.feed = &feed_find_second;
    memset(parser->find_second.samples, 0, sizeof(parser->find_second.samples));
    parser->find_second.seconds = 0;
}

void dcf77_parser_init(dcf77_parser* parser) {
    switch_to_find_second(parser);
}

bool dcf77_parser_feed(dcf77_parser* parser, const bool* samples) {
    return parser->header.feed(parser, samples);
}
