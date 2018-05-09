#include <kernel/drivers/screen.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/cpu/isr.h>
#include <kernel/cpu/timer.h>

void kernel_main() {
    scr_set_flags(0xF0);
    scr_clear();

    isr_init();
    __asm__ __volatile__("sti");
    timer_init(1);
    key_init();

    int i;

    for (i = 1; i < SCR_WIDTH - 1; ++i) {
        scr_putc_at('\xCD', i, SCR_HEIGHT - 1);
        scr_putc_at('\xCD', i, 0);
    }

    for (i = 1; i < SCR_WIDTH - 1; ++i) {
        scr_putc_at('\xBA', 0, i);
        scr_putc_at('\xBA', SCR_WIDTH - 1, i);
    }

    scr_putc_at('\xC9', 0,             0);
    scr_putc_at('\xBB', SCR_WIDTH - 1, 0);
    scr_putc_at('\xC8', 0,             SCR_HEIGHT - 1);
    scr_putc_at('\xBC', SCR_WIDTH - 1, SCR_HEIGHT - 1);

    scr_putc_at('\xC7', 0, 17);
    scr_putc_at('\xB6', SCR_WIDTH - 1, 17);

    for (i = 1; i < SCR_WIDTH - 1; ++i) {
        scr_putc_at('\xC4', i, 17);
    }

    scr_putc_at('\xB5', 5, 0);
    scr_putc_at('\xC6', 15, 0);

    scr_print_at("Flashlight . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1", 3, 2);
    scr_print_at("Batteries  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 4", 3, 3);
    scr_print_at("Sword  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1", 3, 4);
    
    scr_set_flags(0xF2);
    scr_print_at("Inventory", 6, 0);
    
    scr_set_flags(0xF4);
    scr_print_at("Rations  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 0", 3, 5);

    scr_set_flags(0xF1);
    scr_print_at("What item would you like to interact with?", 3, 19);

    scr_set_flags(0xF2);
    scr_putc_at('>', 3, 21);
    scr_set_cursor(5, 21);

}
