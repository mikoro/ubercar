#include <avr/io.h>

#include "iomap.h"
#include "lcd.h"

/* Steering servo: about
 * Full right = 2300
 * Neutral    = 3000 (verified)
 * Full left  = 3600
 *
 * Values outside range will cause bad noises
 */

static void update_screen(void) {
	lcd_printf(1, "STEER %s %d",
		   (STEER_TCRA & BIT(COM1A1)) ? "ON " : "OFF",
		   STEER_OCRA);
}

void init_steering(void) {
	// Enable output pin
	STEER_PORT &= ~STEER0;
	STEER_DDR |= STEER0;

	// 10-bit fast PWM, divide clock by 8
	STEER_TCRA = BIT(WGM11);
	STEER_TCRB = BIT(WGM12) | BIT(WGM13) | BIT(CS11);

	// Set MAX attained in 5ms with clock divider 8
	STEER_ICR = 10000;

	update_screen();
}

void steering_set_enabled(uint8_t en) {
	if (en) {
		STEER_TCRA |= BIT(COM1A1);
	} else {
		STEER_TCRA &= ~BIT(COM1A1);
	}

	update_screen();
}

void steering_set_duty_cycle(uint16_t dc) {
	STEER_OCRA = dc;

	update_screen();
}

// Input in range -600 .. 600, 127 is neutral, -600 is left
// Output in range 2400..3600
void steering_set_direction(int16_t dir) {
	steering_set_duty_cycle(3000 - dir);
}
