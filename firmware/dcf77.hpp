#pragma once
#include "dcf77_parser.hpp"

void dcf77_init();
void dcf77_enable();
void dcf77_disable();

bool dcf77_samples_pending();
void dcf77_clear_samples_pending(bool samples[dcf77::samples_per_second]);
