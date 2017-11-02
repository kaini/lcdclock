#pragma once
#include "dcf77_parser.hpp"
#include <stm32l073xx.h>
#include <array>

namespace hw {

class dcf77 {
public:
    // Both GPIOs must be normal I/O pins. The timer must be equivalent to STM32L0x3 TIM2 (especially the OR register).
    // The LSE is expected to be clocked 2^15 Hz.
    dcf77(GPIO_TypeDef* vcc_gpio, size_t vcc_pin, GPIO_TypeDef* sig_gpio, size_t sig_pin, TIM_TypeDef* timer);

    dcf77(const dcf77&) = delete;
    dcf77(dcf77&&) = delete;
    dcf77& operator=(const dcf77&) = delete;
    dcf77& operator=(dcf77&&) = delete;

    // Call this in the timer interrupt.
    void irq_timer();

    void enable();
    void disable();

    bool samples_pending() const;
    std::array<bool, ::dcf77::samples_per_second> clear_pending_samples();

private:
    GPIO_TypeDef* m_vcc_gpio;
    size_t m_vcc_pin;
    GPIO_TypeDef* m_sig_gpio;
    size_t m_sig_pin;
    TIM_TypeDef* m_timer;

    std::array<std::array<volatile bool, ::dcf77::samples_per_second>, 2> m_samples;
    volatile size_t m_read_index = size_t(-1);
    volatile size_t m_write_index = 0;
    volatile size_t m_write_at = 0;
};

}

