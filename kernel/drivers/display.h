#ifndef DRV_DISPLAY_H
#define DRV_DISPLAY_H

#include <stdint.h>

void dis_init();

void dis_clear(uint8_t color);

void dis_draw_pixel(uint16_t x, uint16_t y, uint8_t color);
void dis_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void dis_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void dis_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);


#endif // DRV_DISPLAY_H
