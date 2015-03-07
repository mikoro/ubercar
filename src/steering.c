#include <avr/io.h>

#include "iomap.h"
#include "lcd.h"

/* Steering servo:
 *
 * Full right = 45000
 * Neutral    = 40000
 * Full left  = 35000
 *
 * Values outside range will cause bad noises
 */
 
static void update_screen(void) {
	lcd_printf(1, "STEER %s %u",
		   (STEER_TCCRA & BIT(COM1A1)) ? "ON " : "OFF",
		   STEER_OCRA);
}

void steering_init(void) {
	// Enable output pin
	STEER_PORT &= ~STEER0;
	STEER_DDR |= STEER0;

	// Fast PWM, divide clock by 1
	STEER_TCCRA = BIT(WGM11);
	STEER_TCCRB = BIT(WGM12) | BIT(WGM13) | BIT(CS10);

	// PWM period 4ms (250 Hz), set TOP accordingly
	STEER_ICR = 64000;
	STEER_OCRA = 40000;
	
	update_screen();
}

void steering_set_enabled(uint8_t en) {

	if (en) { // PWM on (inverted, i.e normally low)
		STEER_TCCRA |= BIT(COM1A1) | BIT(COM1A0);
	} else { // PWM disabled
		STEER_TCCRA &= ~(BIT(COM1A1) | BIT(COM1A0));
	}

	update_screen();
}

void steering_set_duty_cycle(uint16_t dc) {
	STEER_OCRA = dc;

	update_screen();
}
 
// Input in range -5000 .. 5000, 0 is neutral, -5000 is left
void steering_set_direction(int16_t dir) {
	steering_set_duty_cycle(40000 + dir);
}

// Input in range 0 .. 255, 127 is neutral, 0 is left
void steering_set_direction2(uint8_t dir) {
	// Mapping is not perfect, factor should be 39.216 (could this be done with f16.16?)
	uint16_t dc = 39 * dir + 35000;
	steering_set_duty_cycle(dc);
}
