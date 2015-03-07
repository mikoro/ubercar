#ifndef CAR_LCD_H
#define CAR_LCD_H

#include <stdint.h>

void init_lcd();
void print_str(uint8_t line, const char *str);
void lcd_printf(uint8_t line, const char *fmt, ...);

#endif
