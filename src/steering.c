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
 
void steering_init(void) {
	// Enable output pin
	STEER_DDR |= STEER0;
	STEER_PORT &= ~STEER0;

	// Fast PWM, divide clock by 1
	STEER_TCCRA = BIT(WGM11);
	STEER_TCCRB = BIT(WGM12) | BIT(WGM13) | BIT(CS10);

	// PWM period 4ms (250 Hz), set TOP accordingly
	STEER_ICR = 64000;
	STEER_OCRA = 40000; // neutral
}

void steering_set_enabled(uint8_t en) {

	if (en) {
		STEER_TCCRA |= BIT(COM1A1) | BIT(COM1A0); // PWM on (inverted, i.e normally low)
	} else {
		STEER_TCCRA &= ~(BIT(COM1A1) | BIT(COM1A0)); // PWM disabled
	}
}

// Input in range -127 .. 127, 0 is neutral, -127 is left
void steering_set_direction(int8_t dir) {
	// Mapping is not perfect, factor should be 39.216
	STEER_OCRA = 40000 + 39 * dir;
}
