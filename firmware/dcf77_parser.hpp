// Parses a DCF77 signal. Inspired by http://www.gjlay.de/software/dcf77/konzept.html.
#pragma once
#include "datetime.hpp"
#include <array>
#include <cstdint>
#include <experimental/optional>

class dcf77_parser {
public:
    static constexpr int samples_per_second = 100;
    static constexpr int bits_per_minute = 60;

    dcf77_parser();

    /** Feeds one second of samples into the parser. Returns true once a new frame is ready. */
    std::experimental::optional<datetime> feed(const bool* samples);

private:
    enum class bit : int8_t {
        unknown, zero, one, minute_mark,
    };

    template <typename Container>
    static bit ripple_add(Container& bits, bit carray);

    template <typename Container>
    static bool is_parity_valid(const Container& bits, bit parity);

    template <typename Container>
    static int parse_valid_number(const Container& bits);

    void feed_second_sync(const bool* samples);
    bit parse_bit(const bool* samples) const;
    void feed_minute_mark();
    void parse_dataframe();
    std::experimental::optional<datetime> get_result() const;

    std::array<int8_t, samples_per_second> m_second_sync_samples;
    int m_second_sync_count = 0;
    int m_second_start = -1;

    int m_bit_at = 0;

    std::array<int8_t, bits_per_minute> m_minute_marks;
    int m_minute_mark = -1;

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
    int m_frame_number = 0;
};

