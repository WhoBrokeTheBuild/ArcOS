#ifndef DRV_KEYBOARD_H
#define DRV_KEYBOARD_H

#include <kernel/cpu/isr.h>

typedef void (*keycb_t)();

void key_init();

void key_set_input_callback(keycb_t cb);
void key_set_enter_callback(keycb_t cb);

char * key_get_input();

#endif // DRV_KEYBOARD_H
