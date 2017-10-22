#pragma once
#include <stdbool.h>

enum display_digit {
	DISPLAY_DIGIT_0 = 0,
	DISPLAY_DIGIT_1 = 1,
	DISPLAY_DIGIT_2 = 2,
	DISPLAY_DIGIT_3 = 3,
	DISPLAY_DIGIT_4 = 4,
	DISPLAY_DIGIT_5 = 5,
	DISPLAY_DIGIT_6 = 6,
	DISPLAY_DIGIT_7 = 7,
	DISPLAY_DIGIT_8 = 8,
	DISPLAY_DIGIT_9 = 9,
	DISPLAY_DIGIT_MINUS = 10,
	DISPLAY_DIGIT_NONE = 11,
};

struct display_content {
	enum display_digit digits[6];
	bool dots[4];
	bool colon;
};

// The current state of the display.
extern struct display_content display_content;

// Initializes the hardware. Must not be interrupted.
void display_init(void);

// Queue an update of the display content.
void display_refresh(void);
