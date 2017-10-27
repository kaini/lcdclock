#include "dcf77_parser.h"
#include "utils.h"
#include <string.h>

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
    /* DCF77_BIT_UNKNOWN */     { DCF77_BIT_UNKNOWN,     DCF77_BIT_ZERO,    DCF77_BIT_ONE,     DCF77_BIT_UNKNOWN },
    /* DCF77_BIT_ZERO */        { DCF77_BIT_ZERO,        DCF77_BIT_ZERO,    DCF77_BIT_UNKNOWN, DCF77_BIT_ZERO    },
    /* DCF77_BIT_ONE */         { DCF77_BIT_ONE,         DCF77_BIT_UNKNOWN, DCF77_BIT_ONE,     DCF77_BIT_ONE     },
    /* DCF77_BIT_MINUTE_MARK */ { DCF77_BIT_UNKNOWN,     DCF77_BIT_ZERO,    DCF77_BIT_ONE,     DCF77_BIT_UNKNOWN },
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
    ASSERT(0 <= minimum_value && minimum_value <= maximum_value);

    if (maximum_value < 10) {
        // If the value is beween 0 and 9 the tens do not need to be increased.
    } else if (minimum_value == 10 && maximum_value == 10) {
        // If the value is 10 the tens need to be incremented and the ones need
        // to be reset.
        parser->result.minute_ones[0] = DCF77_BIT_ZERO;
        parser->result.minute_ones[1] = DCF77_BIT_ZERO;
        parser->result.minute_ones[2] = DCF77_BIT_ZERO;
        parser->result.minute_ones[3] = DCF77_BIT_ZERO;
        ripple_add(parser->result.minute_tens, COUNTOF(parser->result.minute_tens), DCF77_BIT_ONE);
    } else if (minimum_value > 10) {
        // Something is wrong if the minimum value is greater than 10.
        // The field is out of range. Forget everything we know about the ones
        // and possibly increment the tens.
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
            // xor
            if (parser->result.minute_parity == DCF77_BIT_ZERO) {
                parser->result.minute_parity = DCF77_BIT_ONE;
            } else {
                parser->result.minute_parity = DCF77_BIT_ZERO;
            }
        } else {
            // unknown
            parser->result.minute_parity = DCF77_BIT_UNKNOWN;
            break;
        }
    }

    for (int i = 0; i < DCF77_PARSER_BITS_PER_MINUTE - 1; ++i) {
        int source_i = i + parser->minute_mark + 1;
        if (source_i >= DCF77_PARSER_BITS_PER_MINUTE) {
            source_i -= DCF77_PARSER_BITS_PER_MINUTE;
        }
        parser->result.bits[i] = COMBINE_TABLE[parser->result.bits[i]][parser->new_bits[source_i]];
    }

    // This value is hardcoded here, because its position is decided by handle_minute_mark.
    parser->result.bits[DCF77_PARSER_BITS_PER_MINUTE - 1] = DCF77_BIT_MINUTE_MARK;

    parser->frame_number += 1;
}

bool dcf77_parser_feed(dcf77_parser* parser, const bool* samples) {
	bool has_new_frame = false;

    second_sync(parser, samples);

    if (parser->second_start != -1) {
        dcf77_bit bit = parse_bit(parser, samples);
        parser->new_bits[parser->bit_at] = bit;

        if (bit == DCF77_BIT_MINUTE_MARK) {
            handle_minute_mark(parser);
        }

        if (parser->bit_at == parser->minute_mark) {
            parse_dataframe(parser);
            has_new_frame = true;
        }

        parser->bit_at += 1;
        if (parser->bit_at >= DCF77_PARSER_BITS_PER_MINUTE) {
            parser->bit_at = 0;
        }
    }

    return has_new_frame;
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

bool dcf77_parser_result(const dcf77_parser* parser, struct tm* result) {
    // Note: Slightly out of bounds values do not lead to errors. The time functions
    // handle "overflows" gracefully by adding the overflowing values to the real value.
    // This "just" leads to wrong data.

	bool layout_valid = parser->result.zero == DCF77_BIT_ZERO &&
                        parser->result.one == DCF77_BIT_ONE &&
                        parser->result.minute_mark == DCF77_BIT_MINUTE_MARK &&
						(
						    (parser->result.in_dst == DCF77_BIT_ONE && parser->result.not_in_dst == DCF77_BIT_ZERO) ||
						    (parser->result.in_dst == DCF77_BIT_ZERO && parser->result.not_in_dst == DCF77_BIT_ONE)
						);
	if (!layout_valid) {
		return false;
	}

    bool minute_valid = is_parity_valid(parser->result.minute_parity,
                                        parser->result.minute_bits,
                                        COUNTOF(parser->result.minute_bits));
    if (!minute_valid) {
    	return false;
    }
    result->tm_min = parse_valid_number(parser->result.minute_ones, COUNTOF(parser->result.minute_ones)) +
                     parse_valid_number(parser->result.minute_tens, COUNTOF(parser->result.minute_tens)) * 10;
    if (result->tm_min < 0 || result->tm_min > 59) {
    	return false;
    }

    bool hour_valid = is_parity_valid(parser->result.hour_parity,
                                      parser->result.hour_bits,
                                      COUNTOF(parser->result.hour_bits));
    if (!hour_valid) {
    	return false;
    }
    result->tm_hour = parse_valid_number(parser->result.hour_ones, COUNTOF(parser->result.hour_ones)) +
                      parse_valid_number(parser->result.hour_tens, COUNTOF(parser->result.hour_tens)) * 10;
    if (result->tm_hour < 0 || result->tm_hour > 23) {
        return false;
    }

    bool date_valid = is_parity_valid(parser->result.date_parity,
                                      parser->result.date_bits,
                                      COUNTOF(parser->result.date_bits));
    if (!date_valid) {
        return false;
    }
    result->tm_mday = parse_valid_number(parser->result.day_ones, COUNTOF(parser->result.day_ones)) +
                      parse_valid_number(parser->result.day_tens, COUNTOF(parser->result.day_tens)) * 10;
    result->tm_mon = parse_valid_number(parser->result.month_ones, COUNTOF(parser->result.month_ones)) +
                     parse_valid_number(parser->result.month_tens, COUNTOF(parser->result.month_tens)) * 10;
    result->tm_year = 100 +
                      parse_valid_number(parser->result.year_ones, COUNTOF(parser->result.year_ones)) +
                      parse_valid_number(parser->result.year_tens, COUNTOF(parser->result.year_tens)) * 10;
    if (result->tm_mon < 1 || result->tm_mon > 12) {
        return false;
    }
    if (result->tm_mday < 1 || result->tm_mday > 31) {
        return false;
    }

    result->tm_isdst = parser->result.in_dst == DCF77_BIT_ONE;

	result->tm_sec = 0;

	return mktime(result) != (time_t)(-1);
}
