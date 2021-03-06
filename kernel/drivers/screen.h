#ifndef DRV_SCREEN_H
#define DRV_SCREEN_H

#include <stdint.h>

#define SCR_WIDTH  80
#define SCR_HEIGHT 25

void scr_set_flags(uint8_t flags);
void scr_clear();
void scr_clear_line();

void scr_putc(char c);
void scr_putc_at(char c, int x, int y);

void scr_print(char * s);
void scr_print_at(char * s, int x, int y);

void scr_set_cursor(int x, int y);

void scr_cursor_shift(int x);
void scr_cursor_newline();

#endif // DRV_SCREEN_H
