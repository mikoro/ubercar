#include <avr/io.h>

#include "motor.h"
#include "iomap.h"
#include "lcd.h"
#include "setup.h"

void motor_init(void) {
	// Enable output pin
	MOTORPWM_DDR |= MOTORPWM0;
	MOTORPWM_PORT &= ~MOTORPWM0;

	// Fast PWM, no divisor
	MOTORPWM_TCCRA = BIT(WGM11);
	MOTORPWM_TCCRB = BIT(WGM12) | BIT(WGM13) | BIT(CS10);

	// PWM frequency 20kHz
	MOTORPWM_ICR = 800;
	MOTORPWM_OCRA = 0;

	// Set up control pins
	MOTORCTL_DDR |= MOTORCTL_INA | MOTORCTL_INB; // INx as outputs
	MOTORCTL_DDR &= ~(MOTORCTL_ENA | MOTORCTL_ENB); // ENx as inputs
	MOTORCTL_DDR &= ~MOTORCTL_CS; // CS as input
	MOTORCTL_PORT &= ~(MOTORCTL_INA | MOTORCTL_INB); // stop to GND
	MOTORCTL_PORT |= MOTORCTL_ENA | MOTORCTL_ENB; // enable pull-ups
	MOTORCTL_PORT &= ~MOTORCTL_CS; // disable pull-up
	
	// set up ADC
	ADMUX |= BIT(REFS1) | BIT(REFS0); // internal 2.56 V reference
	ADMUX |= BIT(MUX2); // select ADC12 as input
	ADCSRB |= BIT(MUX5);
	ADMUX |= BIT(ADLAR); // left adjust (-> 8-bit resolution)
	ADCSRA |= BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0); // prescaler 128 -> 125 kHz
	ADCSRA |= BIT(ADATE); // enable auto trigger
	ADCSRA |= BIT(ADEN); // enable ADC
}

void motor_set_enabled(uint8_t en) {
	if (en) {
		MOTORPWM_TCCRA |= BIT(COM4A1); // PWM on (non-inverted, i.e normally high)
	} else {
		MOTORPWM_TCCRA &= ~BIT(COM4A1); // PWM disabled
		MOTORCTL_PORT &= ~(MOTORCTL_INA | MOTORCTL_INB); // stop to GND
	}
}

void motor_set_direction(motor_dir_t dir)
{
	MOTORCTL_PORT &= ~(MOTORCTL_INA | MOTORCTL_INB); // stop to GND
	
	if (dir == MOTOR_FORWARDS)
		MOTORCTL_PORT |= MOTORCTL_INA; // motor on CW
	else
		MOTORCTL_PORT |= MOTORCTL_INB; // motor on CCW
}

// Input range 0 .. 255
void motor_set_power(uint8_t power)
{
	uint16_t adjusted_power = (uint16_t)power + MINIMUM_POWER;
	
	// full duty cycle range is 0 .. 800
	// limit to 1/3 of full power
	if (adjusted_power > 267)
		adjusted_power = 267;
		
	MOTORPWM_OCRA = adjusted_power;
}

// Input range 0 .. 672
void motor_set_power_nolimit(uint16_t power)
{
	uint16_t adjusted_power = power + MINIMUM_POWER;
	
	// full duty cycle range is 0 .. 800
	// allow for 8 us PWM off period to avoid short to battery detection
	if (adjusted_power > 672)
		adjusted_power = 672;
	
	MOTORPWM_OCRA = adjusted_power;
}

// Returns 0 if everything is ok
// 1 if half bridge A is in error
// 2 if half bridge B is in error
// 3 if both half bridges are in error
uint8_t motor_get_status()
{
	uint8_t status = ~MOTORCTL_PIN;
	uint8_t result = (status & MOTORCTL_ENA) ? 0x01 : 0x00;
	result |= (status & MOTORCTL_ENB) ? 0x02 : 0x00;
	
	return result;
}

uint8_t motor_get_current()
{
	// ADCH = (Vin * 256) / Vref
	return ADCH;
}
