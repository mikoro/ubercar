#include <avr/io.h>
#include <avr/interrupt.h>

#include "iomap.h"
#include "lcd.h"

static void update_screen(void) {
	static uint16_t last_ref = 0;
	static uint16_t last_tacho = 0;

#define TACHO_UPDATE_INTERVAL 62500

	uint16_t elapsed = REFCLK_TCNT - last_ref;
	if (elapsed >= TACHO_UPDATE_INTERVAL) {
		uint16_t tachos = TACHO_TCNT - last_tacho;
		last_ref = REFCLK_TCNT;
		last_tacho = TACHO_TCNT;
		uint16_t x = (tachos * 62500) / elapsed;
		lcd_printf(4, "TACHO %u r/s  ", x);
	}
}

void init_tacho(void) {
	// Set clock pin to input
	TACHO_DDR &= ~TACHO0;

	// Counter mode, external clock, clock on falling edge
	OCR5A = 1;
	TACHO_TCRA = 0x00;
	TACHO_TCRB = BIT(CS51) | BIT(CS52);

	// Base clock
	REFCLK_TCRA = 0x00;
	REFCLK_TCRB = BIT(CS32); // 256 divider

	update_screen();
}

void tacho_update() {
	update_screen();
}
