#include <kernel/drivers/keyboard.h>
#include <kernel/cpu/ports.h>
#include <kernel/cpu/isr.h>

const char sc_ascii[] = {
    '?',
    '?',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    '?',
    '?',
    'Q',
    'W',
    'E',
    'R',
    'T',
    'Y',
    'U',
    'I',
    'O',
    'P',
    '[',
    ']',
    '\n',
    '?',
    'A',
    'S',
    'D',
    'F',
    'G',
    'H',
    'J',
    'K',
    'L',
    ';',
    '\'',
    '`',
    '?',
    '\\',
    'Z',
    'X',
    'C',
    'V',
    'B',
    'N',
    'M',
    ',',
    '.',
    '/',
    '?',
    '?',
    '?',
    ' '
};

#define SC_MAX 57

static void _key_handler() {
    uint8_t scancode = read_port_byte(0x60);

    if (scancode > SC_MAX) return;

    scr_putc(sc_ascii[scancode]);
}

void key_init() {
    char s[10];
    itoa(_key_handler, s, 10);
    scr_print_at(s, 0, 0);

    isr_register(IRQ1, _key_handler);
}
