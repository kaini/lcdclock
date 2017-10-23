#include "display.h"
#include "stm32l0xx.h"
#include "utils.h"

struct display_content display_content = {
	.digits = { DISPLAY_DIGIT_NONE, DISPLAY_DIGIT_NONE, DISPLAY_DIGIT_NONE, DISPLAY_DIGIT_NONE, DISPLAY_DIGIT_NONE, DISPLAY_DIGIT_NONE },
	.dots = { false, false, false, false },
	.colon = false,
};

static void init_pins(GPIO_TypeDef* gpio, const int* pins, int count) {
	for (int p = 0; p < count; ++p) {
		int pin = pins[p];
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

void display_init(void) {
	static const int pins_a[] = { 1, 2, 3, 6, 7, 8, 9, 10 };
	static const int pins_b[] = { 0, 1, 3, 4, 5, 9, 10, 11 };

	init_pins(GPIOA, pins_a, COUNTOF(pins_a));
	init_pins(GPIOB, pins_b, COUNTOF(pins_b));

	// 1/3 bias, 1/4 duty, external voltage
	MODIFY_REG(LCD->CR, LCD_CR_BIAS_Msk | LCD_CR_DUTY_Msk, (0b10 << LCD_CR_BIAS_Pos) | (0b011 << LCD_CR_DUTY_Pos) | LCD_CR_VSEL);

	// 32768 rtc / 2^3 prescaler / 16 divider = 256 Hz; 256 Hz / 4 duty -> 64 Hz
	MODIFY_REG(
			LCD->FCR,
			LCD_FCR_PS_Msk | LCD_FCR_DIV_Msk | LCD_FCR_PON_Msk | LCD_FCR_DEAD_Msk,
			(3 << LCD_FCR_PS_Pos) | (0b0000 << LCD_FCR_DIV_Pos) | (0b001 << LCD_FCR_PON_Pos) | (0b011 << LCD_FCR_DEAD_Pos) | LCD_FCR_HD);

	// enable the LCD controller
	SET_BIT(LCD->CR, LCD_CR_LCDEN);

	display_refresh();
}

static void display_digit(int digit, int pos) {
	static const int segment_pin_offsets[] = { 1, 1, 1, 0, 0, 0, 0 };
	static const int segment_coms[] = { 3, 0, 1, 2, 1, 3, 0 };
	static const unsigned int digit_segments[] = {
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

	for (int segment = 0; segment < 7; ++segment) {
		if (digit_segments[digit] & (1 << segment)) {
			int pin = 2 * pos + segment_pin_offsets[segment];
			SET_BIT(LCD->RAM[2 * segment_coms[segment]], 1 << pin);
		}
	}
}

void display_refresh(void) {
	while (READ_BIT(LCD->SR, LCD_SR_UDR))
		;

	for (int i = 0; i < COUNTOF(LCD->RAM); ++i) {
		WRITE_REG(LCD->RAM[i], 0);
	}

	for (int i = 0; i < COUNTOF(display_content.digits); ++i) {
		display_digit(display_content.digits[i], i);
	}

	for (int i = 0; i < COUNTOF(display_content.dots); ++i) {
		if (display_content.dots[i]) {
			SET_BIT(LCD->RAM[4], 1 << (1 + 2 * i));
		}
	}

	if (display_content.colon) {
		SET_BIT(LCD->RAM[4], (1 << 9) | (1 << 11));
	}

	SET_BIT(LCD->SR, LCD_SR_UDR);
}
