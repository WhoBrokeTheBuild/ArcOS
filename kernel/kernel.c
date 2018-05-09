#include <kernel/drivers/screen.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/cpu/isr.h>
#include <kernel/cpu/timer.h>
#include <string.h>

void prompt() {
    scr_print("$ ");
}

void on_input() {
    scr_clear_line();
    prompt();
    scr_print(key_get_input());
}

void print_reg(int reg) {
    int i;
    char s[32];

    itoa(reg, s, 16);
    for (i = strlen(s); i < 8; ++i) {
        scr_putc('0');
    }
    for (i = 0; i < (int)strlen(s); ++i) {
        scr_putc(s[i]);
    }
    scr_print("\n");
}

void on_enter() {
    scr_cursor_newline();

    if (key_get_input()[0] == '\0') {

    }
    if (strcmp(key_get_input(), "HELLO") == 0) {
        scr_set_flags(0x02);
        scr_print("Hello, World!\n");
        scr_set_flags(0x0F);
    }
    else if (strcmp(key_get_input(), "REG") == 0) {
        registers_t reg;

        __asm__("movl %%eax,%0" : "=r"(reg.eax));
        __asm__("movl %%ebx,%0" : "=r"(reg.ebx));
        __asm__("movl %%ecx,%0" : "=r"(reg.ecx));
        __asm__("movl %%edx,%0" : "=r"(reg.edx));
        __asm__("movl %%esp,%0" : "=r"(reg.esp));
        __asm__("movl %%ebp,%0" : "=r"(reg.ebp));
        __asm__("movl %%esi,%0" : "=r"(reg.esi));
        __asm__("movl %%edi,%0" : "=r"(reg.edi));

        scr_print("eax    ");
        print_reg(reg.eax);

        scr_print("ebx    ");
        print_reg(reg.ebx);

        scr_print("ecx    ");
        print_reg(reg.ecx);

        scr_print("edx    ");
        print_reg(reg.edx);

        scr_print("esp    ");
        print_reg(reg.esp);

        scr_print("ebp    ");
        print_reg(reg.ebp);

        scr_print("esi    ");
        print_reg(reg.esi);

        scr_print("edi    ");
        print_reg(reg.edi);
    }
    else if (strcmp(key_get_input(), "CLEAR") == 0) {
        scr_clear();
    }
    else {
        scr_set_flags(0x04);
        scr_print("Command not found '");
        scr_print(key_get_input());
        scr_print("'\n");
        scr_set_flags(0x0F);
    }

    prompt();
}

void shell() {
    scr_clear();
    scr_print("Welcome to ArcOS v0.1.0\n\n");

    prompt();
}

void kernel_main() {
    __asm__ __volatile__("sti");

    isr_init();
    timer_init(1);
    key_init();

    key_set_input_callback(on_input);
    key_set_enter_callback(on_enter);

    shell();
}
