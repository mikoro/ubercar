#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "iomap.h"
#include "led.h"

/*
  LCD size:
  width: 240 px
  height: 320 px
*/

static void send_byte(uint8_t b) {
	for (;;) {
		uint8_t v = LCD_UCSRA;
		if (v & BIT(UDRE1)) break;
	}

	LCD_UDR = b;
}

static uint8_t read_byte() {
	for (;;) {
		uint8_t v = LCD_UCSRA;
		if (v & BIT(RXC1)) break;
	}

	return LCD_UDR;
}

static void send_str(const char *s) {
	for (; *s; ++s)
		send_byte(*s);
}

static void usart_flush_rx()
{
	uint8_t temp;
	
	while (LCD_UCSRA & BIT(RXC1))
		temp = LCD_UDR;
}

static void check_ack()
{
	if (read_byte() != 0x06)
		led_error_loop();
}

// r: 0 .. 31
// g: 0 .. 63
// b: 0 .. 31
static void convert_color(uint8_t r, uint8_t g, uint8_t b, uint8_t* c1, uint8_t* c2)
{
	if (r > 31)
		r = 31;
	
	if (g > 63)
		g = 63;
	
	if (b > 31)
		b = 31;
	
	*c1 = (r << 3) | (g >> 3);
	*c2 = (g << 5) | (b & 0x1f);
}

void lcd_init() {
	// wait for LCD controller to be ready
	_delay_ms(500);
	
	#define BAUDRATE1 ((F_CPU)/(9600*16UL)-1)
	LCD_UBRRH = (uint8_t)(BAUDRATE1 >> 8);
	LCD_UBRRL = (uint8_t)BAUDRATE1;

	LCD_UCSRC = BIT(UCSZ11) | BIT(UCSZ10); // 8 bits, no parity, 1 stop bit
	LCD_UCSRB = BIT(TXEN1) | BIT(RXEN1); // enable transmitter and receiver

	// send Auto-Baud command with 9600 baud rate
	send_byte(0x55);
	check_ack();

	/*
	// set new baud rate
	send_byte(0x51);
	send_byte(0x07); // 14400
	
	#define BAUDRATE2 ((F_CPU)/(14400*16UL)-1)
	LCD_UBRRH = (uint8_t)(BAUDRATE2 >> 8);
	LCD_UBRRL = (uint8_t)BAUDRATE2;

	_delay_ms(100);
	usart_flush_rx();
	*/
	
	// back light on
	send_byte(0x59);
	send_byte(0x00);
	send_byte(0x01);
	check_ack();
	
	// set contrast
	send_byte(0x59);
	send_byte(0x02);
	send_byte(0x0F);
	check_ack();
	
	// enable touch
	send_byte(0x59);
	send_byte(0x05);
	send_byte(0x00);
	check_ack();
	
	// Set opaque text
	send_byte(0x4f);
	send_byte(0x01);
	check_ack();
}

void lcd_clear()
{
	send_byte(0x45);
	check_ack();
}

void lcd_print(uint8_t col, uint8_t row, uint8_t font, uint8_t r, uint8_t g, uint8_t b, const char *str) {
	
	if (font > 0x03)
		font = 0x03;
		
	uint8_t c1; uint8_t c2;
	convert_color(r, g, b, &c1, &c2);
	
	send_byte(0x73);
	send_byte(col);
	send_byte(row);
	send_byte(font);
	send_byte(c1);
	send_byte(c2);
	send_str(str);
	send_byte(0x00);
	check_ack();
}

void lcd_printf(uint8_t col, uint8_t row, uint8_t font, uint8_t r, uint8_t g, uint8_t b, const char *fmt, ...) {
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	uint8_t written = vsnprintf(buffer, 256, fmt, args);
	buffer[written] = 0;
	lcd_print(col, row, font, r, g, b, buffer);
	va_end(args);
}

void lcd_printg(uint16_t x, uint16_t y, uint8_t font, uint8_t proportional, uint8_t r, uint8_t g, uint8_t b, uint8_t width, uint8_t height, const char *str)
{
	if (font > 0x03)
		font = 0x03;
	
	uint8_t c1; uint8_t c2;
	convert_color(r, g, b, &c1, &c2);
	
	send_byte(0x53);
	send_byte(x >> 8);
	send_byte(x & 0x00ff);
	send_byte(y >> 8);
	send_byte(y & 0x00ff);
	send_byte(font | (proportional ? 0x10 : 0x00));
	send_byte(c1);
	send_byte(c2);
	send_byte(width);
	send_byte(height);
	send_str(str);
	send_byte(0x00);
	check_ack();
}

void lcd_printgf(uint16_t x, uint16_t y, uint8_t font, uint8_t proportional, uint8_t r, uint8_t g, uint8_t b, uint8_t width, uint8_t height, const char *fmt, ...)
{
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	uint8_t written = vsnprintf(buffer, 256, fmt, args);
	buffer[written] = 0;
	lcd_printg(x, y, font, proportional, r, g, b, width, height, buffer);
	va_end(args);
}

void lcd_set_transparent_font(uint8_t state)
{
	send_byte(0x4F);
	send_byte(state ? 0x00 : 0x01);
	check_ack();
}

void lcd_set_bg_color(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t c1; uint8_t c2;
	convert_color(r, g, b, &c1, &c2);
	
	send_byte(0x42);
	send_byte(c1);
	send_byte(c2);
	check_ack();
}

void lcd_set_draw_solid(uint8_t state)
{
	send_byte(0x70);
	send_byte(state ? 0x00 : 0x01);
	check_ack();
}

void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t c1; uint8_t c2;
	convert_color(r, g, b, &c1, &c2);
	
	send_byte(0x72);
	send_byte(x1 >> 8);
	send_byte(x1 & 0x00ff);
	send_byte(y1 >> 8);
	send_byte(y1 & 0x00ff);
	send_byte(x2 >> 8);
	send_byte(x2 & 0x00ff);
	send_byte(y2 >> 8);
	send_byte(y2 & 0x00ff);
	send_byte(c1);
	send_byte(c2);
	check_ack();
}

void lcd_draw_button(uint8_t state, uint16_t x, uint16_t y, uint8_t rb, uint8_t gb, uint8_t bb, uint8_t font, uint8_t rs, uint8_t gs, uint8_t bs, uint8_t width, uint8_t height, const char *str)
{
	if (font > 0x03)
		font = 0x03;
	
	uint8_t cb1; uint8_t cb2;
	uint8_t cs1; uint8_t cs2;
	convert_color(rb, gb, bb, &cb1, &cb2);
	convert_color(rs, gs, bs, &cs1, &cs2);
	
	send_byte(0x62);
	send_byte(state);
	send_byte(x >> 8);
	send_byte(x & 0x00ff);
	send_byte(y >> 8);
	send_byte(y & 0x00ff);
	send_byte(cb1);
	send_byte(cb2);
	send_byte(font);
	send_byte(cs1);
	send_byte(cs2);
	send_byte(width);
	send_byte(height);
	send_str(str);
	send_byte(0x00);
	check_ack();
}

void lcd_set_touch_region(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	send_byte(0x75);
	send_byte(x1 >> 8);
	send_byte(x1 & 0x00ff);
	send_byte(y1 >> 8);
	send_byte(y1 & 0x00ff);
	send_byte(x2 >> 8);
	send_byte(x2 & 0x00ff);
	send_byte(y2 >> 8);
	send_byte(y2 & 0x00ff);
	check_ack();
}

uint8_t lcd_is_touched()
{
	send_byte(0x6f);
	send_byte(0x04);
	
	read_byte();
	uint8_t response = read_byte();
	read_byte();
	read_byte();
	
	return (response == 0x01);
}

void lcd_draw_header(const char* mode_str)
{
	size_t length = strlen(mode_str);
	uint8_t odd = length % 2;
	
	lcd_clear();
	lcd_draw_rectangle(0, 0, 240, 50, 31, 0, 0);
	lcd_set_transparent_font(1);
	lcd_printg(35, 10, 3, 0, 31, 63, 31, 2, 2, "ubercar");
	lcd_printg(120 - (length / 2) * 25 - (odd ? 13 : 0), 70, 3, 0, 31, 63, 0, 2, 2, mode_str);
}
