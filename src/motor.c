#include <avr/io.h>

#include "iomap.h"
#include "lcd.h"

/* OCR must be at least about 70 for the wheels to start turning from
 * idle position
 */

static uint8_t over = 0;

static void update_screen(void) {
	lcd_printf(2, "MOTOR %s %u %s",
		   (MOTORCTL_PORT & MOTORCTL_INA) ? "ON " : "OFF",
		   MOTORPWM_OCRA,
		   over ? "(Over!)" : "");
}

void init_motor(void) {
	// Enable output pin
	MOTORPWM_PORT &= ~MOTORPWM0;
	MOTORPWM_DDR |= MOTORPWM0;

	// Fast PWM, no divisor
	MOTORPWM_TCCRA = BIT(WGM11);
	MOTORPWM_TCCRB = BIT(WGM12) | BIT(WGM13) | BIT(CS10);

	// PWM frequency 20kHz
	MOTORPWM_ICR = 800;
	MOTORPWM_OCRA = 0;

	// Set up controlpins
	MOTORCTL_DDR |= MOTORCTL_INA | MOTORCTL_INB; // INx as outputs
	MOTORCTL_DDR &= ~(MOTORCTL_ENA | MOTORCTL_ENB); // ENx as inputs
	MOTORCTL_PORT &= ~(MOTORCTL_INA | MOTORCTL_INB); // stop to GND
	MOTORCTL_PORT |= MOTORCTL_ENA | MOTORCTL_ENB; // enable pull-ups

	update_screen();
}

void motor_set_enabled(uint8_t en) {
	if (en) {
		MOTORPWM_TCCRA |= BIT(COM4A1); // PWM on (non-inverted, i.e normally high)
		MOTORCTL_PORT |= MOTORCTL_INA; // motor on CW
	} else {
		MOTORPWM_TCCRA &= ~BIT(COM4A1); // PWM disabled
		MOTORCTL_PORT &= ~MOTORCTL_INA; // stop to GND
	}
	
	update_screen();
}

// Input range in 0..800
void motor_set_duty_cycle(uint16_t dc) {
	if (dc > 267) {
		// Over 1/3 power specified, limiting
		dc = 267;
		over = 1;
	} else {
		over = 0;
	}
	
	MOTORPWM_OCRA = dc;
	update_screen();
}

// Input range 0..255, motor should start spinning immediately
void motor_set_duty_cycle2(uint8_t dc) {
	motor_set_duty_cycle(70 + dc);
}
