#include <avr/io.h>

#include "iomap.h"
#include "lcd.h"

static void update_screen(void) {
	static uint16_t last_ref = 0;
	static uint16_t last_tacho = 0;

	uint16_t elapsed = REFCLK_TCNT - last_ref;
	if (elapsed >= 31250) {
		// at least 0.5 sec elapsed
		uint16_t tachos = TACHO_TCNT - last_tacho;
		last_ref = REFCLK_TCNT;
		last_tacho = TACHO_TCNT;
		lcd_printf(4, "TACHO %d/hs", tachos);
	}
}

void init_tacho(void) {
	// Set clock pin to input
	TACHO_DDR &= ~TACHO0;

	// Counter mode, external clock, clock on falling edge
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
