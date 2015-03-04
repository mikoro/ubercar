#include <avr/io.h>

#include "iomap.h"
#include "lcd.h"

/* OCR must be at least about 70 for the wheels to start turning from
 * idle position
 */

static uint8_t over = 0;

static void update_screen(void) {
	lcd_printf(2, "MOTOR %s %d %s",
		   (MOTORCTL_PORT & MOTORCTL_INA) ? "ON " : "OFF",
		   MOTORPWM_OCRA,
		   over ? "(Over!)" : "");
}

void init_motor(void) {
	// Enable output pin
	MOTORPWM_PORT &= ~MOTORPWM0;
	MOTORPWM_DDR |= MOTORPWM0;

	// 10-bit fast PWM, no divisor
	MOTORPWM_TCRA = BIT(WGM11);
	MOTORPWM_TCRB = BIT(WGM12) | BIT(WGM13) | BIT(CS10);

	// PWM frequency 20kHz
	MOTORPWM_ICR = 800;
	MOTORPWM_OCRA = 0;

	// Set up controlpins
	MOTORCTL_PORT &= ~(MOTORCTL_INA | MOTORCTL_INB);
	MOTORCTL_DDR |= MOTORCTL_INA | MOTORCTL_INB;
	MOTORCTL_DDR &= ~(MOTORCTL_ENA | MOTORCTL_ENB);

	update_screen();
}

void motor_set_enabled(uint8_t en) {
	if (en) {
		MOTORPWM_TCRA |= BIT(COM4A1);
		MOTORCTL_PORT |= MOTORCTL_INA;
	} else {
		MOTORPWM_TCRA &= ~BIT(COM4A1);
		MOTORCTL_PORT &= ~MOTORCTL_INA;
	}
	update_screen();
}

// 0..800
void motor_set_duty_cycle(uint16_t dc) {
	if (dc > 266) {
		// Over 1/3 power specified, limiting
		dc = 266;
		over = 1;
	} else {
		over = 0;
	}
	MOTORPWM_OCRA = dc;
	update_screen();
}
