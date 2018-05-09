#ifndef CPU_TIMER_H
#define CPU_TIMER_H

#include <stdint.h>

void timer_init(uint32_t freq);

uint32_t get_timer();

#endif // CPU_TIMER_H