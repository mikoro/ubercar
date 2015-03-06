#include <avr/io.h>
#include <avr/interrupt.h>

#include "iomap.h"
#include "lcd.h"

#include "fixed/fix8.h"

static fix8_t avg = F8(0);

static const fix8_t alpha = F8(0.25);
static const fix8_t oneminusalpha = F8(0.75);

static void update_screen(void) {
	static uint16_t last_ref = 0;
	static uint16_t last_tacho = 0;

#define TACHO_UPDATE_INTERVAL 15625

	uint16_t elapsed = REFCLK_TCNT - last_ref;
	if (elapsed >= TACHO_UPDATE_INTERVAL) {
		uint16_t tachos = TACHO_TCNT - last_tacho;
		last_ref = REFCLK_TCNT;
		last_tacho = TACHO_TCNT;
		int8_t x = (tachos * 62500) / elapsed;

		fix8_t xf = fix8_from_int(x);

		avg = fix8_add(fix8_mul(alpha, xf), fix8_mul(oneminusalpha, avg));

		int8_t val = fix8_to_int(avg);

		lcd_printf(4, "TACHO %d r/s  ", val);
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

uint8_t tacho_get_speed() {
	return fix8_to_int(avg);
}

