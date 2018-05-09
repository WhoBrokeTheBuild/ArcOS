#include <kernel/cpu/timer.h>
#include <kernel/cpu/isr.h>
#include <kernel/cpu/ports.h>
#include <stdlib.h>

#define CLOCK_HZ 1193182

uint32_t _tick = 0;

static void _timer_handler() {
    ++_tick;
}

uint32_t get_timer() {
    return _tick;
}

void timer_init(uint32_t freq) {
    isr_register(IRQ0, _timer_handler);

    uint32_t divisor = CLOCK_HZ / freq;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    write_port_byte(0x43, 0x36);
    write_port_byte(0x40, low);
    write_port_byte(0x40, high);
}