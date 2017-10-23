#pragma once
#include "dcf77_parser.h"
#include <stdbool.h>

void dcf77_init(void);
void dcf77_enable(void);
void dcf77_disable(void);

bool dcf77_samples_pending(void);
void dcf77_clear_samples_pending(bool samples[DCF77_PARSER_SAMPLES_PER_SECOND]);
