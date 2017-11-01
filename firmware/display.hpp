#pragma once
#include <cstdint>
#include <array>
#include <initializer_list>
#include <tuple>
#include <stm32l073xx.h>

namespace hw {

enum class display_digit : int8_t {
    zero = 0,
    one = 1,
    two = 2,
    three = 3,
    four = 4,
    five = 5,
    six = 6,
    seven = 7,
    eight = 8,
    nine = 9,
    minus = 10,
    none = 11,
};

class display {
public:
    using pins = std::initializer_list<std::tuple<GPIO_TypeDef*, std::initializer_list<size_t>>>;

    display(LCD_TypeDef* lcd, pins pins);

    display(const display&) = delete;
    display(display&&) = delete;
    display& operator=(const display&) = delete;
    display& operator=(display&&) = delete;

    void set_digit(size_t i, display_digit value) { m_digits[i] = value; m_need_refresh = true; }
    void set_dot(size_t i, bool visible) { m_dots[i] = visible; m_need_refresh = true; }
    void set_colon(bool visible) { m_colon = visible; m_need_refresh = true; }

    void refresh(bool force = false);

private:
    void render_digit(display_digit digit, size_t pos);

    LCD_TypeDef* m_lcd;
    bool m_need_refresh = false;
    std::array<display_digit, 6> m_digits;
    std::array<bool, 4> m_dots;
    bool m_colon = false;
};

}

