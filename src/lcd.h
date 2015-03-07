#ifndef CAR_LCD_H
#define CAR_LCD_H

#include <stdint.h>

void lcd_init();
void lcd_clear();
void lcd_set_font_size(uint8_t size);
void lcd_set_transparent_font(uint8_t state);
void lcd_print(uint8_t col, uint8_t row, uint8_t font, uint8_t r, uint8_t g, uint8_t b, const char *str);
void lcd_printf(uint8_t col, uint8_t row, uint8_t font, uint8_t r, uint8_t g, uint8_t b, const char *fmt, ...);
void lcd_printg(uint16_t x, uint16_t y, uint8_t font, uint8_t proportional, uint8_t r, uint8_t g, uint8_t b, uint8_t width, uint8_t height, const char *str);
void lcd_printgf(uint16_t x, uint16_t y, uint8_t font, uint8_t proportional, uint8_t r, uint8_t g, uint8_t b, uint8_t width, uint8_t height, const char *fmt, ...);
void lcd_set_bg_color(uint8_t r, uint8_t g, uint8_t b);
void lcd_set_draw_solid(uint8_t state);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t r, uint8_t g, uint8_t b);
void lcd_draw_button(uint8_t state, uint16_t x, uint16_t y, uint8_t rb, uint8_t gb, uint8_t bb, uint8_t font, uint8_t rs, uint8_t gs, uint8_t bs, uint8_t width, uint8_t height, const char *str);
void lcd_set_touch_region(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
uint8_t lcd_is_touched();

#endif
