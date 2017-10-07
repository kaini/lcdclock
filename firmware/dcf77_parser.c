#include "dcf77_parser.h"
#include "countof.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

void dcf77_parser_init(dcf77_parser* parser) {
    memset(parser->second_sync_samples, 0, sizeof(parser->second_sync_samples));
    parser->second_sync_count = 0;
    parser->second_start = -1;

    parser->bit_at = 0;

    memset(parser->minute_marks, 0, sizeof(parser->minute_marks));
    parser->minute_mark = -1;

    memset(parser->new_bits, 0, sizeof(parser->new_bits));
    memset(parser->result.bits, 0, sizeof(parser->result.bits));
    parser->frame_number = 0;
}

static void second_sync(dcf77_parser* parser, const bool* samples) {
    for (int i = 0; i < DCF77_PARSER_SAMPLES_PER_SECOND; ++i) {
        parser->second_sync_samples[i] += samples[i];
    }
    parser->second_sync_count += 1;

    if (parser->second_sync_count == 10) {
        // Find the 100 ms band with the lowest values. The beginning of this
        // band is the second start. The applied kernel is therefore
        // [>1<; 1; 1; 1; 1; 1; 1; 1; 1; 1]
        int current_value = parser->second_sync_samples[0] +
                            parser->second_sync_samples[1] +
                            parser->second_sync_samples[2] +
                            parser->second_sync_samples[3] +
                            parser->second_sync_samples[4] +
                            parser->second_sync_samples[5] +
                            parser->second_sync_samples[6] +
                            parser->second_sync_samples[7] +
                            parser->second_sync_samples[8] +
                            parser->second_sync_samples[9];
        int min_value = current_value;
        int min_index = 0;
        for (int i = 1; i < DCF77_PARSER_SAMPLES_PER_SECOND; ++i) {
            // Subtract the old leftmost value.
            current_value -= parser->second_sync_samples[i - 1];

            // Add the new rightmost value
            int right_index = i + 9;
            if (right_index >= DCF77_PARSER_SAMPLES_PER_SECOND) {
                right_index -= DCF77_PARSER_SAMPLES_PER_SECOND;
            }
            current_value += parser->second_sync_samples[right_index];

            if (current_value < min_value) {
                min_value = current_value;
                min_index = i;
            }
        }

        memset(parser->second_sync_samples, 0, sizeof(parser->second_sync_samples));
        parser->second_sync_count = 0;
        parser->second_start = min_index;
    }
}

static dcf77_bit parse_bit(const dcf77_parser* parser, const bool* samples) {
    // Count the time low in the first 200 ms.
    int low_ms = 0;
    for (int i = 0; i < DCF77_PARSER_SAMPLES_PER_SECOND / 5; ++i) {
        int index = i + parser->second_start;
        if (index > DCF77_PARSER_SAMPLES_PER_SECOND) {
            index -= DCF77_PARSER_SAMPLES_PER_SECOND;
        }
        if (!samples[index]) {
            low_ms += 1000 / DCF77_PARSER_SAMPLES_PER_SECOND;
        }
    }

    if (low_ms <= 20) {
        return DCF77_BIT_MINUTE_MARK;
    } else if (60 <= low_ms && low_ms <= 120) {
        return DCF77_BIT_ZERO;
    } else if (180 <= low_ms) {
        return DCF77_BIT_ONE;
    } else {
        return DCF77_BIT_UNKNOWN;
    }
}

static void handle_minute_mark(dcf77_parser* parser) {
    parser->minute_marks[parser->bit_at] += 1;

    // The counters have to be clamped every now and then to avoid that it takes
    // a too long time to resync the minute mark if it changes after a leap
    // second. In this setup it takes at most 11 minutes to resync to a new
    // minute mark after a leap second.
    if (parser->minute_marks[parser->bit_at] == 10) {
        for (int i = 0; i < DCF77_PARSER_BITS_PER_MINUTE; ++i) {
            parser->minute_marks[i] -= 5;
            if (parser->minute_marks[i] < 0) {
                parser->minute_marks[i] = 0;
            }
        }
    }

    // Set a new minute mark if this is new new maximum.
    if (parser->bit_at != parser->minute_mark) {
        if (parser->minute_mark == -1 || parser->minute_marks[parser->bit_at] >= parser->minute_marks[parser->minute_mark]) {
            parser->minute_mark = parser->bit_at;
        }
    }
}

static const dcf77_bit ADD_RESULT_TABLE[4][4] = {
    /*                            DCF77_BIT_UNKNOWN, DCF77_BIT_ZERO,    DCF77_BIT_ONE,     DCF77_BIT_MINUTE_MARK */
    /* DCF77_BIT_UNKNOWN */     { DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN },
    /* DCF77_BIT_ZERO */        { DCF77_BIT_UNKNOWN, DCF77_BIT_ZERO,    DCF77_BIT_ONE,     DCF77_BIT_UNKNOWN },
    /* DCF77_BIT_ONE */         { DCF77_BIT_UNKNOWN, DCF77_BIT_ONE,     DCF77_BIT_ZERO,    DCF77_BIT_UNKNOWN },
    /* DCF77_BIT_MINUTE_MARK */ { DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN },
};

static const dcf77_bit ADD_CARRY_TABLE[4][4] = {
    /*                            DCF77_BIT_UNKNOWN, DCF77_BIT_ZERO,    DCF77_BIT_ONE,     DCF77_BIT_MINUTE_MARK */
    /* DCF77_BIT_UNKNOWN */     { DCF77_BIT_UNKNOWN, DCF77_BIT_ZERO,    DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN },
    /* DCF77_BIT_ZERO */        { DCF77_BIT_ZERO,    DCF77_BIT_ZERO,    DCF77_BIT_ZERO,    DCF77_BIT_ZERO    },
    /* DCF77_BIT_ONE */         { DCF77_BIT_UNKNOWN, DCF77_BIT_ZERO,    DCF77_BIT_ONE,     DCF77_BIT_UNKNOWN },
    /* DCF77_BIT_MINUTE_MARK */ { DCF77_BIT_UNKNOWN, DCF77_BIT_ZERO,    DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN },
};

static const dcf77_bit COMBINE_TABLE[4][4] = {
    /*                            DCF77_BIT_UNKNOWN,     DCF77_BIT_ZERO,    DCF77_BIT_ONE,     DCF77_BIT_MINUTE_MARK */
    /* DCF77_BIT_UNKNOWN */     { DCF77_BIT_UNKNOWN,     DCF77_BIT_ZERO,    DCF77_BIT_ONE,     DCF77_BIT_MINUTE_MARK },
    /* DCF77_BIT_ZERO */        { DCF77_BIT_ZERO,        DCF77_BIT_ZERO,    DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN     },
    /* DCF77_BIT_ONE */         { DCF77_BIT_ONE,         DCF77_BIT_UNKNOWN, DCF77_BIT_ONE,     DCF77_BIT_UNKNOWN     },
    /* DCF77_BIT_MINUTE_MARK */ { DCF77_BIT_MINUTE_MARK, DCF77_BIT_UNKNOWN, DCF77_BIT_UNKNOWN, DCF77_BIT_MINUTE_MARK },
};

static dcf77_bit ripple_add(dcf77_bit* bits, int length, dcf77_bit carry) {
    for (int i = 0; i < length; ++i) {
        dcf77_bit value = bits[i];
        bits[i] = ADD_RESULT_TABLE[value][carry];
        carry = ADD_CARRY_TABLE[value][carry];
    }
    return carry;
}

static void parse_dataframe(dcf77_parser* parser) {
    // First simulate an increment on the existing minute
    dcf77_bit carry = ripple_add(parser->result.minute_ones, COUNTOF(parser->result.minute_ones), DCF77_BIT_ONE);
    int minimum_value = 16 * (carry == DCF77_BIT_ONE) +
                         8 * (parser->result.minute_ones[3] == DCF77_BIT_ONE) +
                         4 * (parser->result.minute_ones[2] == DCF77_BIT_ONE) +
                         2 * (parser->result.minute_ones[1] == DCF77_BIT_ONE) +
                         1 * (parser->result.minute_ones[0] == DCF77_BIT_ONE);
    int maximum_value = 16 * (carry != DCF77_BIT_ZERO) +
                         8 * (parser->result.minute_ones[3] != DCF77_BIT_ZERO) +
                         4 * (parser->result.minute_ones[2] != DCF77_BIT_ZERO) +
                         2 * (parser->result.minute_ones[1] != DCF77_BIT_ZERO) +
                         1 * (parser->result.minute_ones[0] != DCF77_BIT_ZERO);
    assert(minimum_value <= maximum_value);

    if (maximum_value < 10) {
        // If the value is beween 0 and 9 the tens do not need to be increased.
    } else if (maximum_value == minimum_value && maximum_value == 10) {
        // If the value is 10 the tens need to be incremented and the ones need
        // to be reset.
        parser->result.minute_ones[0] = DCF77_BIT_ZERO;
        parser->result.minute_ones[1] = DCF77_BIT_ZERO;
        parser->result.minute_ones[2] = DCF77_BIT_ZERO;
        parser->result.minute_ones[3] = DCF77_BIT_ZERO;
        ripple_add(parser->result.minute_tens, COUNTOF(parser->result.minute_tens), DCF77_BIT_ONE);
    } else if (minimum_value >= 10) {
        // Something is wrong if the minimum value is >= 10, the field is out of
        // range. Forget everything we know about the ones and possibly increment
        // the tens.
        parser->result.minute_ones[0] = DCF77_BIT_UNKNOWN;
        parser->result.minute_ones[1] = DCF77_BIT_UNKNOWN;
        parser->result.minute_ones[2] = DCF77_BIT_UNKNOWN;
        parser->result.minute_ones[3] = DCF77_BIT_UNKNOWN;
        ripple_add(parser->result.minute_tens, COUNTOF(parser->result.minute_tens), DCF77_BIT_UNKNOWN);
    } else if (maximum_value >= 10) {
        // It might be required to flip the tens if the maximum might be bigger or
        // equal to 10.
        ripple_add(parser->result.minute_tens, COUNTOF(parser->result.minute_tens), DCF77_BIT_UNKNOWN);
    }

    // Recalculate the parity. If one bit is unkown the whole parity is unkown.
    parser->result.minute_parity = DCF77_BIT_ZERO;
    for (int i = 0; i < COUNTOF(parser->result.minute_bits); ++i) {
        if (parser->result.minute_bits[i] == DCF77_BIT_ZERO) {
            // no-op
        } else if (parser->result.minute_bits[i] == DCF77_BIT_ONE) {
            if (parser->result.minute_parity == DCF77_BIT_ZERO) {
                parser->result.minute_parity = DCF77_BIT_ONE;
            } else {
                parser->result.minute_parity = DCF77_BIT_ZERO;
            }
        } else {
            parser->result.minute_parity = DCF77_BIT_UNKNOWN;
            break;
        }
    }

    for (int i = 0; i < DCF77_PARSER_BITS_PER_MINUTE; ++i) {
        int source_i = i + parser->minute_mark + 1;
        if (source_i >= DCF77_PARSER_BITS_PER_MINUTE) {
            source_i -= DCF77_PARSER_BITS_PER_MINUTE;
        }
        parser->result.bits[i] = COMBINE_TABLE[parser->result.bits[i]][parser->new_bits[source_i]];
    }

    parser->frame_number += 1;
}

void dcf77_parser_feed(dcf77_parser* parser, const bool* samples) {
    second_sync(parser, samples);

    if (parser->second_start != -1) {
        dcf77_bit bit = parse_bit(parser, samples);
        parser->new_bits[parser->bit_at] = bit;

        if (bit == DCF77_BIT_MINUTE_MARK) {
            handle_minute_mark(parser);
        }

        if (parser->bit_at == parser->minute_mark) {
            parse_dataframe(parser);
        }

        parser->bit_at += 1;
        if (parser->bit_at >= DCF77_PARSER_BITS_PER_MINUTE) {
            parser->bit_at = 0;
        }
    }
}

static bool is_parity_valid(dcf77_bit parity, const dcf77_bit* bits, int count) {
    if (parity == DCF77_BIT_UNKNOWN || parity == DCF77_BIT_MINUTE_MARK) {
        return false;
    } else {
        bool result = false;
        for (int i = 0; i < count; ++i) {
            if (bits[i] == DCF77_BIT_ZERO) {
                // no-op
            } else if (bits[i] == DCF77_BIT_ONE) {
                result = !result;
            } else {
                return false;
            }
        }
        return result ? (parity == DCF77_BIT_ONE) : (parity == DCF77_BIT_ZERO);
    }
}

static int parse_valid_number(const dcf77_bit* bits, int count) {
    int result = 0;
    for (int i = 0; i < count; ++i) {
        result += (1 << i) * (bits[i] == DCF77_BIT_ONE);
    }
    return result;
}

dcf77_result dcf77_parser_result(const dcf77_parser* parser) {
    dcf77_result result;
    result.frame_number = parser->frame_number;
    
    result.layout_valid = parser->result.zero == DCF77_BIT_ZERO &&
                          parser->result.one == DCF77_BIT_ONE &&
                          parser->result.minute_mark == DCF77_BIT_MINUTE_MARK;
    result.minute_valid = is_parity_valid(parser->result.minute_parity,
                                        parser->result.minute_bits,
                                        COUNTOF(parser->result.minute_bits));
    result.hour_valid = is_parity_valid(parser->result.hour_parity,
                                        parser->result.hour_bits,
                                        COUNTOF(parser->result.hour_bits));
    result.date_valid = is_parity_valid(parser->result.date_parity,
                                        parser->result.date_bits,
                                        COUNTOF(parser->result.date_bits));

    if (result.minute_valid) {
        result.minute = parse_valid_number(parser->result.minute_ones, COUNTOF(parser->result.minute_ones)) +
                        parse_valid_number(parser->result.minute_tens, COUNTOF(parser->result.minute_tens)) * 10;
    } else {
        result.minute = -1;
    }

    if (result.hour_valid) {
        result.hour = parse_valid_number(parser->result.hour_ones, COUNTOF(parser->result.hour_ones)) +
                      parse_valid_number(parser->result.hour_tens, COUNTOF(parser->result.hour_tens)) * 10;
    } else {
        result.hour = -1;
    }

    if (result.date_valid) {
        result.day = parse_valid_number(parser->result.day_ones, COUNTOF(parser->result.day_ones)) +
                     parse_valid_number(parser->result.day_tens, COUNTOF(parser->result.day_tens)) * 10;
        result.month = parse_valid_number(parser->result.month_ones, COUNTOF(parser->result.month_ones)) +
                       parse_valid_number(parser->result.month_tens, COUNTOF(parser->result.month_tens)) * 10;
        result.year = parse_valid_number(parser->result.year_ones, COUNTOF(parser->result.year_ones)) +
                      parse_valid_number(parser->result.year_tens, COUNTOF(parser->result.year_tens)) * 10;
    } else {
        result.day = -1;
        result.month = -1;
        result.year = -1;
    }

    return result;
}
