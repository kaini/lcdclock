#include "display.hpp"
#include "utils.hpp"
#include <initializer_list>
#include <algorithm>
#include <stm32l0xx.h>

hw::display::display(LCD_TypeDef* lcd, pins pins) : m_lcd(lcd) {
    std::fill(m_digits.begin(), m_digits.end(), display_digit::none);
    std::fill(m_dots.begin(), m_dots.end(), false);

    for (const auto& pin_group : pins) {
        GPIO_TypeDef* gpio = std::get<0>(pin_group);
        for (const auto pin : std::get<1>(pin_group)) {
            // alternative function 1 (LCD)
            MODIFY_REG(gpio->AFR[GPIO_AFR_Idx(pin)], GPIO_AFR_AFSEL_Msk(pin), (0b0001 << GPIO_AFR_AFSEL_Pos(pin)));
            // no pullup/pulldown
            MODIFY_REG(gpio->PUPDR, GPIO_PUPDR_PUPD_Msk(pin), (0b00 << GPIO_PUPDR_PUPD_Pos(pin)));
            // high output speed
            MODIFY_REG(gpio->OSPEEDR, GPIO_OSPEEDER_OSPEED_Msk(pin), (0b10 << GPIO_OSPEEDER_OSPEED_Pos(pin)));
            // push/pull
            CLEAR_BIT(gpio->OTYPER, GPIO_OTYPER_OT(pin));
            // alternative function mode
            MODIFY_REG(gpio->MODER, GPIO_MODER_MODE_Msk(pin), (0b10 << GPIO_MODER_MODE_Pos(pin)));
        }
    }

    // 1/3 bias, 1/4 duty, external voltage
    MODIFY_REG(m_lcd->CR, LCD_CR_BIAS_Msk | LCD_CR_DUTY_Msk, (0b10 << LCD_CR_BIAS_Pos) | (0b011 << LCD_CR_DUTY_Pos) | LCD_CR_VSEL);

    // 32768 rtc / 2^3 prescaler / 16 divider = 256 Hz; 256 Hz / 4 duty -> 64 Hz
    MODIFY_REG(
            m_lcd->FCR,
            LCD_FCR_PS_Msk | LCD_FCR_DIV_Msk | LCD_FCR_PON_Msk | LCD_FCR_DEAD_Msk,
            (3 << LCD_FCR_PS_Pos) | (0b0000 << LCD_FCR_DIV_Pos) | (0b001 << LCD_FCR_PON_Pos) | (0b011 << LCD_FCR_DEAD_Pos) | LCD_FCR_HD);

    // enable the LCD controller
    SET_BIT(m_lcd->CR, LCD_CR_LCDEN);

    refresh(true);
}

void hw::display::refresh(bool force) {
    if (!force && !m_need_refresh) {
        return;
    }
    m_need_refresh = false;

	while (READ_BIT(m_lcd->SR, LCD_SR_UDR))
		;

	for (size_t i = 0; i < sizeof(m_lcd->RAM) / sizeof(m_lcd->RAM[0]); ++i) {
		WRITE_REG(m_lcd->RAM[i], 0);
	}

	for (size_t i = 0; i < m_digits.size(); ++i) {
	    render_digit(m_digits[i], i);
	}

	for (size_t i = 0; i < m_dots.size(); ++i) {
		if (m_dots[i]) {
			SET_BIT(m_lcd->RAM[4], 1 << (1 + 2 * i));
		}
	}

	if (m_colon) {
		SET_BIT(m_lcd->RAM[4], (1 << 9) | (1 << 11));
	}

	SET_BIT(m_lcd->SR, LCD_SR_UDR);
}

void hw::display::render_digit(display_digit digit, size_t pos) {
    static constexpr size_t segment_pin_offsets[] = { 1, 1, 1, 0, 0, 0, 0 };
    static constexpr size_t segment_coms[] = { 3, 0, 1, 2, 1, 3, 0 };
    static constexpr uint8_t digit_segments[] = {
    //    GFEDCBA <- segments active
        0b0111111,  // 0
        0b0000110,  // 1
        0b1011011,  // 2
        0b1001111,  // 3
        0b1100110,  // 4
        0b1101101,  // 5
        0b1111101,  // 6
        0b0000111,  // 7
        0b1111111,  // 8
        0b1101111,  // 9
        0b1000000,  // -
        0b0000000,  // (empty)
    };

    for (size_t segment = 0; segment < 7; ++segment) {
        if (digit_segments[static_cast<size_t>(digit)] & (1 << segment)) {
            size_t pin = 2 * pos + segment_pin_offsets[segment];
            SET_BIT(m_lcd->RAM[2 * segment_coms[segment]], 1 << pin);
        }
    }
}
