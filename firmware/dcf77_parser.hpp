// Parses a DCF77 signal. Inspired by http://www.gjlay.de/software/dcf77/konzept.html.
#pragma once
#include "datetime.hpp"
#include <array>
#include <cstdint>
#include <experimental/optional>

namespace dcf77 {

constexpr size_t samples_per_second = 100;
constexpr size_t bits_per_minute = 60;

enum class bit : int8_t {
    unknown, zero, one, minute_mark,
};

/** A class to synchronize the start of a DCF77 bit. Returns an index from 0 to samples_per_second or -1. */
class bit_sync {
public:
    bit_sync();
    void feed(const bool samples[samples_per_second]);
    size_t bit_start() const { return m_bit_start; }

private:
    std::array<int8_t, samples_per_second> m_histogram;
    int m_count = 0;
    size_t m_bit_start = size_t(-1);
};

/** A class to synchronize the start of a DCF77 frame. Returns an index from 0 to bits_per_minute or -1. */
class frame_sync {
public:
    frame_sync();
    void feed(bit bit);
    size_t frame_start() const { return m_frame_start; }

private:
    std::array<int8_t, bits_per_minute> m_histogram;
    size_t m_at = 0;
    size_t m_frame_start = size_t(-1);
};

class parser {
public:
    parser();

    /** Feeds one second of samples into the parser. Returns true once a new frame is ready. */
    bool feed(const bool* samples);
    std::experimental::optional<datetime> get_result() const;

private:
    void consume_dataframe();

    bit_sync m_bit_sync;
    frame_sync m_frame_sync;
    size_t m_at = 0;
    std::array<bit, bits_per_minute> m_new_bits;
    union {
        std::array<bit, bits_per_minute> bits;
        struct {
            bit zero;
            std::array<bit, 14 + 1 + 1> ignored0; // weather data + callbit + DST in next hour
            bit in_dst;
            bit not_in_dst;
            std::array<bit, 1> ignored1; // leap second bit
            bit one;
            union {
                std::array<bit, 4 + 3> minute_bits;
                struct {
                    std::array<bit, 4> minute_ones;
                    std::array<bit, 3> minute_tens;
                };
            };
            bit minute_parity;
            union {
                std::array<bit, 4 + 2> hour_bits;
                struct {
                    std::array<bit, 4> hour_ones;
                    std::array<bit, 2> hour_tens;
                };
            };
            bit hour_parity;
            union {
                std::array<bit, 4 + 2 + 3 + 4 + 1 + 4 + 4> date_bits;
                struct {
                    std::array<bit, 4> day_ones;
                    std::array<bit, 2> day_tens;
                    std::array<bit, 3> weekday_ones;
                    std::array<bit, 4> month_ones;
                    std::array<bit, 1> month_tens;
                    std::array<bit, 4> year_ones;
                    std::array<bit, 4> year_tens;
                };
            };
            bit date_parity;
            bit minute_mark;
        };
    } m_result;
};

}

