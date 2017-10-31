#pragma once
#include <stm32l073xx.h>

namespace hw {

class critical_section {
public:
    /** Begins a critical section. */
    critical_section() : m_primask(__get_PRIMASK()) { __disable_irq(); }
    /** Ends the critical section. */
    ~critical_section() { __set_PRIMASK(m_primask); }

    critical_section(const critical_section&) = delete;
    critical_section(critical_section&&) = delete;
    critical_section& operator=(const critical_section&) = delete;
    critical_section& operator=(critical_section&&) = delete;

private:
    uint32_t m_primask;
};

}
