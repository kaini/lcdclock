#include "dcf77.hpp"
#include "utils.hpp"
#include "dcf77_parser.hpp"
#include <stm32l0xx.h>

hw::dcf77::dcf77(GPIO_TypeDef* vcc_gpio, size_t vcc_pin, GPIO_TypeDef* sig_gpio, size_t sig_pin, TIM_TypeDef* timer) :
        m_vcc_gpio(vcc_gpio), m_vcc_pin(vcc_pin), m_sig_gpio(sig_gpio), m_sig_pin(sig_pin), m_timer(timer) {

    // VCC Pin: no pullup/pulldown, low speed, push/pull, output mode
    MODIFY_REG(m_vcc_gpio->PUPDR, GPIO_PUPDR_PUPD_Msk(m_vcc_pin), 0b00 << GPIO_PUPDR_PUPD_Pos(m_vcc_pin));
    MODIFY_REG(m_vcc_gpio->OSPEEDR, GPIO_OSPEEDER_OSPEED_Msk(m_vcc_pin), 0b00 << GPIO_OSPEEDER_OSPEED_Pos(m_vcc_pin));
    CLEAR_BIT(m_vcc_gpio->OTYPER, GPIO_OTYPER_OT(m_vcc_pin));
    MODIFY_REG(m_vcc_gpio->MODER, GPIO_MODER_MODE_Msk(m_vcc_pin), 0b01 << GPIO_MODER_MODE_Pos(m_vcc_pin));

    // Use the 2^15 Hz clock from the LSE as clock source.
    // To get an exact 10 ms clock, I use a nice idea from https://www.mikrocontroller.net/topic/23135#172414
    // Solve for x and y:
    //   x * floor(2^15 / 100) + y * ceil(2^15 / 100) = 2^15
    //   x + y = 100
    // See https://www.wolframalpha.com/input/?i=solve+x+*+floor(2%5E15+%2F+100)+%2B+y+*+ceil(2%5E15+%2F+100)+%3D+2%5E15;+x+%2B+y+%3D+100+for+x,+y
    //   x = 32 (count 32 times to 327) and y = 68 (count 68 times to 328)
    SET_BIT(m_timer->SMCR, TIM_SMCR_ECE);
    MODIFY_REG(m_timer->OR, TIM2_OR_ETR_RMP_Msk, 0b101 << TIM2_OR_ETR_RMP_Pos);  // Connect to LSE
    SET_BIT(m_timer->DIER, TIM_DIER_UIE);

    disable();
}

void hw::dcf77::irq_timer() {
    size_t at = m_write_at;
    size_t write_index = m_write_index;

    m_samples[write_index][at++] = READ_BIT(m_sig_gpio->IDR, GPIO_IDR_ID(m_sig_pin));

    if (at >= m_samples[write_index].size()) {
        m_read_index = write_index;
        write_index += 1;
        if (write_index >= m_samples.size()) {
            write_index = 0;
        }
        m_write_index = write_index;
        at = 0;
    }

    if (at < 32) {
        WRITE_REG(m_timer->ARR, 326);
    } else {
        WRITE_REG(m_timer->ARR, 327);
    }

    m_write_at = at;
}

void hw::dcf77::enable() {
	SET_BIT(m_vcc_gpio->ODR, GPIO_ODR_OD(m_vcc_pin));

	// Signal Pin: pullup, input mode
	MODIFY_REG(m_sig_gpio->PUPDR, GPIO_PUPDR_PUPD_Msk(m_sig_pin), 0b01 << GPIO_PUPDR_PUPD_Pos(m_sig_pin));
	MODIFY_REG(m_sig_gpio->MODER, GPIO_MODER_MODE_Msk(m_sig_pin), 0b00 << GPIO_MODER_MODE_Pos(m_sig_pin));

	m_write_at = 0;
	m_read_index = size_t(-1);
	m_write_index = 0;
	WRITE_REG(m_timer->CNT, 0);
	WRITE_REG(m_timer->ARR, 326);
	SET_BIT(m_timer->CR1, TIM_CR1_CEN);
}

void hw::dcf77::disable() {
	CLEAR_BIT(m_timer->CR1, TIM_CR1_CEN);

	// Signal pin: no pullup/pulldown, analog mode
	MODIFY_REG(m_sig_gpio->PUPDR, GPIO_PUPDR_PUPD_Msk(m_sig_pin), 0b00 << GPIO_PUPDR_PUPD_Pos(m_sig_pin));
	MODIFY_REG(m_sig_gpio->MODER, GPIO_MODER_MODE_Msk(m_sig_pin), 0b11 << GPIO_MODER_MODE_Pos(m_sig_pin));

	CLEAR_BIT(m_vcc_gpio->ODR, GPIO_ODR_OD(m_vcc_pin));
}

bool hw::dcf77::samples_pending() const {
	return m_read_index != size_t(-1);
}

std::array<bool, ::dcf77::samples_per_second> hw::dcf77::clear_pending_samples() {
    std::array<bool, ::dcf77::samples_per_second> result;
    size_t read_index = m_read_index;
	if (read_index != size_t(-1)) {
	    m_read_index = size_t(-1);
	    std::copy(m_samples[read_index].begin(), m_samples[read_index].end(), result.begin());
	}
	return result;
}
