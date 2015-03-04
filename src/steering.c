#include <avr/io.h>

#include "iomap.h"

void init_steering(void) {
	// Enable output pin
	STEER_PORT &= ~STEER0;
	STEER_DDR |= STEER0;

	// 10-bit fast PWM, divide clock by 8
	STEER_TCRA = BIT(WGM11);
	STEER_TCRB = BIT(WGM12) | BIT(WGM13) | BIT(CS11);

	// Set MAX attained in 20ms with clock divider 8
	STEER_ICR = 40000;
}

void steering_set_enabled(uint8_t en) {
	if (en)
		STEER_TCRA |= BIT(COM1A1);
	else
		STEER_TCRA &= ~BIT(COM1A1);
}

void steering_set_duty_cycle(uint16_t dc) {
	STEER_OCRA = dc;
}
