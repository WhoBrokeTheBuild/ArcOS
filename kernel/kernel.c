#include <kernel/drivers/screen.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/cpu/isr.h>
#include <kernel/cpu/timer.h>
#include <string.h>
#include <stdbool.h>

bool _is_zork = false;

void zork() {
    _is_zork = true;
    int i;

    scr_set_flags(0xF0);
    scr_clear();

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
    scr_print_at(key_get_input(), 5, 21);
}

void prompt() {
    scr_print("$ ");
}

void on_input() {
    if (_is_zork) {
        zork();
        return;
    }

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
    scr_set_flags(0x0F);

    if (_is_zork) {
        _is_zork = false;
        scr_clear();
        prompt();
        return;
    }

    scr_cursor_newline();

    char * input = key_get_input();

    if (input[0] == '\0') {

    }
    if (strcmp(input, "HELLO") == 0) {
        scr_set_flags(0x02);
        scr_print("Hello, World!\n");
        scr_set_flags(0x0F);
    }
    else if (strcmp(input, "REG") == 0) {
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
    else if (strcmp(input, "CLEAR") == 0) {
        scr_clear();
    }
    else if (strcmp(input, "EXIT") == 0) {
        __asm__ __volatile__ ("hlt");
    }
    else if (strcmp(input, "UNAME") == 0) {
        scr_print("ArcOS 0.1.0-arcos SLW/ArcOS\n");
    }
    else if (strcmp(input, "DEFINITELY NOT ZORK") == 0) {
        input[0] = '\0';
        zork();
        return;
    }
    else if (strcmp(input, "HELP") == 0) {
        scr_print("Commands: HELP, CLEAR, REG, ECHO, UNAME, HELLO, DEFINITELY NOT ZORK\n");
    }
    else if (input[0] == 'E' && input[1] == 'C' && input[2] == 'H' && input[3] == 'O') {
        scr_print(input + 5);
        scr_putc('\n');
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
