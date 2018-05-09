#include <kernel/drivers/keyboard.h>
#include <kernel/cpu/ports.h>
#include <kernel/cpu/isr.h>

int _index = 0;
char _input[256];

keycb_t _input_cb = 0;
keycb_t _enter_cb = 0;

const char ASCII_SCANCODES[] = {
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

#define KEY_ENTER       0x1C
#define KEY_BACKSPACE   0x0E

static void _key_handler() {
    uint8_t scancode = read_port_byte(0x60);

    if (scancode == KEY_ENTER) {
        if (_enter_cb) {
            _enter_cb();
        }

        _index = 0;
        _input[_index] = '\0';

        return;
    }

    if (scancode == KEY_BACKSPACE) {
        if (_index > 0) --_index;
        _input[_index] = '\0';

        if (_input_cb) {
            _input_cb();
        }

        return;
    }

    if (scancode > sizeof(ASCII_SCANCODES)) return;
    if (ASCII_SCANCODES[scancode] == '?') return;

    _input[_index++] = ASCII_SCANCODES[scancode];
    _input[_index] = '\0';

    if (_input_cb) {
        _input_cb();
    }
}

void key_init() {
    _input[0] = '\0';

    isr_register(IRQ1, _key_handler);
}

void key_set_input_callback(keycb_t cb) {
    _input_cb = cb;
}

void key_set_enter_callback(keycb_t cb) {
    _enter_cb = cb;
}

char * key_get_input() {
    return _input;
}
