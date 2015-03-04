#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdarg.h>

#include "iomap.h"

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

void print_str(uint8_t line, const char *str) {
	send_byte(0x73);
	send_byte(0x00);
	send_byte(line);
	send_byte(0x03);
	send_byte(0xff);
	send_byte(0xff);
	send_str(str);
	send_byte(0x00);
	if (read_byte() != 0x06) {
		LED_PORT = LED0|LED1;
		for (;;);
	}
}

void lcd_printf(uint8_t line, const char *fmt, ...) {
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	uint8_t written = vsnprintf(buffer, 256, fmt, args);
	buffer[written] = 0;
	print_str(line, buffer);
	va_end(args);
}

void print_ch(uint8_t col, uint8_t row, uint8_t b) {
	send_byte(0x54);
	send_byte(b);
	send_byte(col);
	send_byte(row);
	send_byte(0xff);
	send_byte(0xff);
	if (read_byte() != 0x06) {
		LED_PORT = LED0|LED1;
		for (;;);
	}
}

void init_lcd() {
	// 8 bits, no parity, 1 stop bit, 9600 baud

	// wait for LCD controller to be ready
	_delay_ms(500);

	LCD_PORT = 0x00;
	LCD_DDR = BIT(PD3);

	LCD_UBRRL = 103;
	LCD_UBRRH = 0;
	LCD_UCSRB = BIT(TXEN1) | BIT(RXEN1);
	LCD_UCSRC = BIT(UCSZ11) | BIT(UCSZ10);

	// Send Auto-baud command
	send_byte(0x55);
	if (read_byte() != 0x06) {
		LED_PORT = LED0|LED1;
		for (;;);
	}

	// Set opaque text
	send_byte(0x4f);
	send_byte(0x01);
	if (read_byte() != 0x06) {
		LED_PORT = LED0|LED1;
		for (;;);
	}

	lcd_printf(0, "LCD   OK");
}
