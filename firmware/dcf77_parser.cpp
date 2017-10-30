#include "dcf77_parser.hpp"
#include "utils.hpp"
#include <algorithm>

dcf77_parser::dcf77_parser() {
    std::fill(m_result.bits.begin(), m_result.bits.end(), bit::unknown);
}

std::experimental::optional<datetime> dcf77_parser::feed(const bool* samples) {
    bool has_new_frame = false;

    feed_second_sync(samples);

    if (m_second_start != -1) {
        bit bit = parse_bit(samples);
        m_new_bits[m_bit_at] = bit;

        if (bit == bit::minute_mark) {
            feed_minute_mark();
        }

        if (m_bit_at == m_minute_mark) {
            parse_dataframe();
            has_new_frame = true;
        }

        m_bit_at += 1;
        if (m_bit_at >= bits_per_minute) {
            m_bit_at = 0;
        }
    }

    if (has_new_frame) {
        return get_result();
    } else {
        return {};
    }
}

void dcf77_parser::feed_second_sync(const bool* samples) {
    for (int i = 0; i < samples_per_second; ++i) {
        m_second_sync_samples[i] += samples[i];
    }
    m_second_sync_count += 1;

    if (m_second_sync_count >= 10) {
        // Find the 100 ms band with the lowest values. The beginning of this
        // band is the second start. The applied kernel is therefore
        // [>1<; 1; 1; 1; 1; 1; 1; 1; 1; 1]
        int current_value =
            m_second_sync_samples[0] +
            m_second_sync_samples[1] +
            m_second_sync_samples[2] +
            m_second_sync_samples[3] +
            m_second_sync_samples[4] +
            m_second_sync_samples[5] +
            m_second_sync_samples[6] +
            m_second_sync_samples[7] +
            m_second_sync_samples[8] +
            m_second_sync_samples[9];
        int min_value = current_value;
        int min_index = 0;
        for (int i = 1; i < samples_per_second; ++i) {
            // Subtract the old leftmost value.
            current_value -= m_second_sync_samples[i - 1];

            // Add the new rightmost value
            int right_index = i + 9;
            if (right_index >= samples_per_second) {
                right_index -= samples_per_second;
            }
            current_value += m_second_sync_samples[right_index];

            if (current_value < min_value) {
                min_value = current_value;
                min_index = i;
            }
        }

        std::fill(m_second_sync_samples.begin(), m_second_sync_samples.end(), 0);
        m_second_sync_count = 0;
        m_second_start = min_index;
    }
}

dcf77_parser::bit dcf77_parser::parse_bit(const bool* samples) const {
    // Count the time low in the first 200 ms.
    int low_ms = 0;
    for (int i = 0; i < samples_per_second / 5; ++i) {
        int index = i + m_second_start;
        if (index > samples_per_second) {
            index -= samples_per_second;
        }
        if (!samples[index]) {
            low_ms += 1000 / samples_per_second;
        }
    }

    if (low_ms <= 20) {
        return bit::minute_mark;
    } else if (60 <= low_ms && low_ms <= 120) {
        return bit::zero;
    } else if (180 <= low_ms) {
        return bit::one;
    } else {
        return bit::unknown;
    }
}

void dcf77_parser::feed_minute_mark() {
    m_minute_marks[m_bit_at] += 1;

    // The counters have to be clamped every now and then to avoid that it takes
    // a too long time to resync the minute mark if it changes after a leap
    // second. In this setup it takes at most 11 minutes to resync to a new
    // minute mark after a leap second.
    if (m_minute_marks[m_bit_at] >= 10) {
        for (auto& count : m_minute_marks) {
            if (count >= 5) {
                count -= 5;
            }
        }
    }

    // Set a new minute mark if this is new new maximum.
    if (m_bit_at != m_minute_mark) {
        if (m_minute_mark == -1 || m_minute_marks[m_bit_at] >= m_minute_marks[m_minute_mark]) {
            m_minute_mark = m_bit_at;
        }
    }
}

template <typename Container>
dcf77_parser::bit dcf77_parser::ripple_add(Container& bits, bit carry) {
    static constexpr bit add_result_table[4][4] = {
        /*                       bit::unknown, bit::zero,    bit::one,     bit::minute_mark */
        /* bit::unknown */     { bit::unknown, bit::unknown, bit::unknown, bit::unknown },
        /* bit::zero */        { bit::unknown, bit::zero,    bit::one,     bit::unknown },
        /* bit::one */         { bit::unknown, bit::one,     bit::zero,    bit::unknown },
        /* bit::minute_mark */ { bit::unknown, bit::unknown, bit::unknown, bit::unknown },
    };

    static constexpr bit add_carry_table[4][4] = {
        /*                       bit::unknown, bit::zero,    bit::one,     bit::minute_mark */
        /* bit::unknown */     { bit::unknown, bit::zero,    bit::unknown, bit::unknown },
        /* bit::zero */        { bit::zero,    bit::zero,    bit::zero,    bit::zero    },
        /* bit::one */         { bit::unknown, bit::zero,    bit::one,     bit::unknown },
        /* bit::minute_mark */ { bit::unknown, bit::zero,    bit::unknown, bit::unknown },
    };

    for (size_t i = 0; i < bits.size(); ++i) {
        bit value = bits[i];
        bits[i] = add_result_table[static_cast<size_t>(value)][static_cast<size_t>(carry)];
        carry = add_carry_table[static_cast<size_t>(value)][static_cast<size_t>(carry)];
    }
    return carry;
}

void dcf77_parser::parse_dataframe() {
    // First simulate an increment on the existing minute
    bit carry = ripple_add(m_result.minute_ones, bit::one);
    int minimum_value = 16 * (carry == bit::one) +
                         8 * (m_result.minute_ones[3] == bit::one) +
                         4 * (m_result.minute_ones[2] == bit::one) +
                         2 * (m_result.minute_ones[1] == bit::one) +
                         1 * (m_result.minute_ones[0] == bit::one);
    int maximum_value = 16 * (carry != bit::zero) +
                         8 * (m_result.minute_ones[3] != bit::zero) +
                         4 * (m_result.minute_ones[2] != bit::zero) +
                         2 * (m_result.minute_ones[1] != bit::zero) +
                         1 * (m_result.minute_ones[0] != bit::zero);
    ASSERT(0 <= minimum_value && minimum_value <= maximum_value);

    if (maximum_value < 10) {
        // If the value is beween 0 and 9 the tens do not need to be increased.
    } else if (minimum_value == 10 && maximum_value == 10) {
        // If the value is 10 the tens need to be incremented and the ones need
        // to be reset.
        m_result.minute_ones[0] = bit::zero;
        m_result.minute_ones[1] = bit::zero;
        m_result.minute_ones[2] = bit::zero;
        m_result.minute_ones[3] = bit::zero;
        ripple_add(m_result.minute_tens, bit::one);
    } else if (minimum_value > 10) {
        // Something is wrong if the minimum value is greater than 10.
        // The field is out of range. Forget everything we know about the ones
        // and possibly increment the tens.
        m_result.minute_ones[0] = bit::unknown;
        m_result.minute_ones[1] = bit::unknown;
        m_result.minute_ones[2] = bit::unknown;
        m_result.minute_ones[3] = bit::unknown;
        ripple_add(m_result.minute_tens, bit::unknown);
    } else if (maximum_value >= 10) {
        // It might be required to flip the tens if the maximum might be bigger or
        // equal to 10.
        ripple_add(m_result.minute_tens, bit::unknown);
    }

    // Recalculate the parity. If one bit is unkown the whole parity is unkown.
    m_result.minute_parity = bit::zero;
    for (size_t i = 0; i < m_result.minute_bits.size(); ++i) {
        if (m_result.minute_bits[i] == bit::zero) {
            // no-op
        } else if (m_result.minute_bits[i] == bit::one) {
            // xor
            if (m_result.minute_parity == bit::zero) {
                m_result.minute_parity = bit::one;
            } else {
                m_result.minute_parity = bit::zero;
            }
        } else {
            // unknown
            m_result.minute_parity = bit::unknown;
            break;
        }
    }

    static constexpr bit combine_table[4][4] = {
        /*                       bit::unknown,     bit::zero,    bit::one,     bit::minute_mark */
        /* bit::unknown */     { bit::unknown,     bit::zero,    bit::one,     bit::unknown },
        /* bit::zero */        { bit::zero,        bit::zero,    bit::unknown, bit::zero    },
        /* bit::one */         { bit::one,         bit::unknown, bit::one,     bit::one     },
        /* bit::minute_mark */ { bit::unknown,     bit::zero,    bit::one,     bit::unknown },
    };
    for (int i = 0; i < bits_per_minute - 1; ++i) {
        int source_i = i + m_minute_mark + 1;
        if (source_i >= bits_per_minute) {
            source_i -= bits_per_minute;
        }
        m_result.bits[i] = combine_table[static_cast<size_t>(m_result.bits[i])][static_cast<size_t>(m_new_bits[source_i])];
    }

    // This value is hardcoded here, because its position is decided by handle_minute_mark.
    m_result.bits.back() = bit::minute_mark;

    m_frame_number += 1;
}

template <typename Container>
bool dcf77_parser::is_parity_valid(const Container& bits, bit parity) {
    if (parity == bit::unknown || parity == bit::minute_mark) {
        return false;
    } else {
        bool result = false;
        for (size_t i = 0; i < bits.size(); ++i) {
            if (bits[i] == bit::zero) {
                // no-op
            } else if (bits[i] == bit::one) {
                result = !result;
            } else {
                return false;
            }
        }
        return result ? (parity == bit::one) : (parity == bit::zero);
    }
}

template <typename Container>
int dcf77_parser::parse_valid_number(const Container& bits) {
    int result = 0;
    for (size_t i = 0; i < bits.size(); ++i) {
        result += (1 << i) * (bits[i] == bit::one);
    }
    return result;
}

std::experimental::optional<datetime> dcf77_parser::get_result() const {
	bool layout_valid =
	    m_result.zero == bit::zero &&
	    m_result.one == bit::one &&
	    m_result.minute_mark == bit::minute_mark &&
        (
            (m_result.in_dst == bit::one && m_result.not_in_dst == bit::zero) ||
            (m_result.in_dst == bit::zero && m_result.not_in_dst == bit::one)
        );
	if (!layout_valid) {
		return {};
	}

    if (!is_parity_valid(m_result.minute_bits, m_result.minute_parity)) {
    	return {};
    }
    int minute = parse_valid_number(m_result.minute_ones) + parse_valid_number(m_result.minute_tens) * 10;

    if (!is_parity_valid(m_result.hour_bits, m_result.hour_parity)) {
    	return {};
    }
    int hour = parse_valid_number(m_result.hour_ones) + parse_valid_number(m_result.hour_tens) * 10;

    if (!is_parity_valid(m_result.date_bits, m_result.date_parity)) {
        return {};
    }
    int day = parse_valid_number(m_result.day_ones) + parse_valid_number(m_result.day_tens) * 10;
    int month = parse_valid_number(m_result.month_ones) + parse_valid_number(m_result.month_tens) * 10;
    int year = 2000 + parse_valid_number(m_result.year_ones) + parse_valid_number(m_result.year_tens) * 10;

    bool isdst = m_result.in_dst == bit::one;

    datetime result(year, month, day, hour, minute, 0);
    if (isdst) {
        result.add_hours(-2);
    } else {
        result.add_hours(-1);
    }
    return result;
}
