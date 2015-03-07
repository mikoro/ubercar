#ifndef CAR_LCD_H
#define CAR_LCD_H

#include <stdint.h>

void lcd_init();
void lcd_print_str(uint8_t line, const char *str);
void lcd_printf(uint8_t line, const char *fmt, ...);

#endif
