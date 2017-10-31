#include "dcf77.hpp"
#include "utils.hpp"
#include "dcf77_parser.hpp"
#include <stm32l0xx.h>

#define VCC_GPIO GPIOB
#define VCC_PIN 6

#define SIGNAL_GPIO GPIOB
#define SIGNAL_PIN 7

static volatile bool samples_a[dcf77::samples_per_second] = {};
static volatile bool samples_b[dcf77::samples_per_second] = {};
static volatile const bool* volatile samples_read = NULL;
static volatile bool* volatile samples_write = NULL;
static volatile int write_at = 0;

void dcf77_init(void) {
	// VCC Pin: no pullup/pulldown, low speed, push/pull, output mode
	MODIFY_REG(VCC_GPIO->PUPDR, GPIO_PUPDR_PUPD_Msk(VCC_PIN), 0b00 << GPIO_PUPDR_PUPD_Pos(VCC_PIN));
	MODIFY_REG(VCC_GPIO->OSPEEDR, GPIO_OSPEEDER_OSPEED_Msk(VCC_PIN), 0b00 << GPIO_OSPEEDER_OSPEED_Pos(VCC_PIN));
	CLEAR_BIT(VCC_GPIO->OTYPER, GPIO_OTYPER_OT(VCC_PIN));
	MODIFY_REG(VCC_GPIO->MODER, GPIO_MODER_MODE_Msk(VCC_PIN), 0b01 << GPIO_MODER_MODE_Pos(VCC_PIN));

	// Use the 2^15 Hz clock from the LSE as clock source.
	// To get an exact 10 ms clock, I use a nice idea from https://www.mikrocontroller.net/topic/23135#172414
	// Solve for x and y:
	//   x * floor(2^15 / 100) + y * ceil(2^15 / 100) = 2^15
	//   x + y = 100
	// See https://www.wolframalpha.com/input/?i=solve+x+*+floor(2%5E15+%2F+100)+%2B+y+*+ceil(2%5E15+%2F+100)+%3D+2%5E15;+x+%2B+y+%3D+100+for+x,+y
	//   x = 32 (count 32 times to 327) and y = 68 (count 68 times to 328)
	SET_BIT(TIM2->SMCR, TIM_SMCR_ECE);
	MODIFY_REG(TIM2->OR, TIM2_OR_ETR_RMP_Msk, 0b101 << TIM2_OR_ETR_RMP_Pos);  // Connect to LSE
	SET_BIT(TIM2->DIER, TIM_DIER_UIE);
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 20);

	dcf77_disable();
}

void dcf77_enable(void) {
	SET_BIT(VCC_GPIO->ODR, GPIO_ODR_OD(VCC_PIN));

	// Signal Pin: pullup, input mode
	MODIFY_REG(SIGNAL_GPIO->PUPDR, GPIO_PUPDR_PUPD_Msk(SIGNAL_PIN), 0b01 << GPIO_PUPDR_PUPD_Pos(SIGNAL_PIN));
	MODIFY_REG(SIGNAL_GPIO->MODER, GPIO_MODER_MODE_Msk(SIGNAL_PIN), 0b00 << GPIO_MODER_MODE_Pos(SIGNAL_PIN));

	write_at = 0;
	samples_read = NULL;
	samples_write = samples_a;
	WRITE_REG(TIM2->CNT, 0);
	WRITE_REG(TIM2->ARR, 326);
	SET_BIT(TIM2->CR1, TIM_CR1_CEN);
}

void dcf77_disable(void) {
	CLEAR_BIT(TIM2->CR1, TIM_CR1_CEN);

	// Signal pin: no pullup/pulldown, analog mode
	MODIFY_REG(SIGNAL_GPIO->PUPDR, GPIO_PUPDR_PUPD_Msk(SIGNAL_PIN), 0b00 << GPIO_PUPDR_PUPD_Pos(SIGNAL_PIN));
	MODIFY_REG(SIGNAL_GPIO->MODER, GPIO_MODER_MODE_Msk(SIGNAL_PIN), 0b11 << GPIO_MODER_MODE_Pos(SIGNAL_PIN));

	CLEAR_BIT(VCC_GPIO->ODR, GPIO_ODR_OD(VCC_PIN));
}

bool dcf77_samples_pending() {
	return samples_read != NULL;
}

void dcf77_clear_samples_pending(bool output_samples[dcf77::samples_per_second]) {
	volatile const bool* current_samples = samples_read;
	samples_read = NULL;
	if (output_samples != NULL) {
		for (int i = 0; i < dcf77::samples_per_second; ++i) {
			output_samples[i] = current_samples[i];
		}
	}
}

extern "C" void TIM2_IRQHandler(void) {
	if (READ_BIT(TIM2->SR, TIM_SR_UIF)) {
		CLEAR_BIT(TIM2->SR, TIM_SR_UIF);

		int at = write_at;
		volatile bool* samples = samples_write;

		samples[at++] = READ_BIT(SIGNAL_GPIO->IDR, GPIO_IDR_ID(SIGNAL_PIN));

		if (at >= dcf77::samples_per_second) {
			samples_read = samples;
			samples_write = (samples == samples_a) ? samples_b : samples_a;
			at = 0;
		}

		if (at < 32) {
			WRITE_REG(TIM2->ARR, 326);
		} else {
			WRITE_REG(TIM2->ARR, 327);
		}

		write_at = at;
	}
}
