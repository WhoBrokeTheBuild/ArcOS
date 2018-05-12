#include <kernel/drivers/screen.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/display.h>
#include <kernel/cpu/isr.h>
#include <kernel/cpu/timer.h>
#include <string.h>
#include <stdbool.h>

void on_input() {

}

void on_enter() {

}


void kernel_main() {
    __asm__ __volatile__("sti");

    scr_clear();
    scr_print("ArcOS 0.1.0");

    isr_init();
    timer_init(1);
    key_init();
    dis_init();

    key_set_input_callback(on_input);
    key_set_enter_callback(on_enter);

    //dis_draw_pixel(100, 100, 0xFF);
    dis_draw_line(0, 0, 100, 100, 0xFF);
}
