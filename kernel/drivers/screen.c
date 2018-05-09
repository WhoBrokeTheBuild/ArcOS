#include <kernel/drivers/screen.h>
#include <kernel/cpu/ports.h>

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

#define SCR_VIDEO_ADDRESS 0xB8000

#define SCR_WHITE_ON_BLACK 0x0F

int _scr_x = 0, _scr_y = 0;
uint8_t _flags = SCR_WHITE_ON_BLACK;

int _scr_get_offset(int x, int y) {
    return 2 * (y * SCR_WIDTH + x);
}

int _scr_get_cursor_offset() {
    return _scr_get_offset(_scr_x, _scr_y);
}

void scr_set_flags(uint8_t flags) {
    _flags = flags;
}

void scr_clear() {
    char * vid_mem_ptr = (char *)SCR_VIDEO_ADDRESS;

    for (int i = 0; i < SCR_HEIGHT * SCR_WIDTH * 2; i += 2) {
        vid_mem_ptr[i + 0] = ' ';
        vid_mem_ptr[i + 1] = _flags;
    }

    scr_set_cursor(0, 0);
}

void scr_putc(char c) {
    if (c == '\n') {
        scr_newline_cursor();
        return;
    }

    char * vid_mem_ptr = (char*)SCR_VIDEO_ADDRESS;
    int offset = _scr_get_cursor_offset();

    vid_mem_ptr[offset++] = c;
    vid_mem_ptr[offset++] = _flags;

    scr_shift_cursor();
}

void scr_putc_at(char c, int x, int y) {
    scr_set_cursor(x, y);
    scr_putc(c);
}

void scr_print(char * s) {
    while (*s) {
        scr_putc(*s);
        ++s;
    }
}

void scr_print_at(char * s, int x, int y) {
    scr_set_cursor(x, y);
    scr_print(s);
}

void scr_set_cursor(int x, int y) {
    _scr_x = x;
    _scr_y = y;

    int offset = _scr_get_cursor_offset() / 2;
    write_port_byte(REG_SCREEN_CTRL, 14);
    write_port_byte(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    write_port_byte(REG_SCREEN_CTRL, 15);
    write_port_byte(REG_SCREEN_DATA, (unsigned char)(offset & 0xFF));
}

void scr_shift_cursor() {
    ++_scr_x;
    if (_scr_x >= SCR_WIDTH) {
        scr_newline_cursor();
        return;
    }

    scr_set_cursor(_scr_x, _scr_y);
}

void scr_newline_cursor() {
    _scr_x = 0;
    _scr_y++;
    if (_scr_y >= SCR_HEIGHT) {
        _scr_y = 0;
    }

    scr_set_cursor(_scr_x, _scr_y);
}