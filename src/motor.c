#include <avr/io.h>

#include "motor.h"
#include "iomap.h"
#include "lcd.h"

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
// Actual full duty cycle range is 0 .. 800
// 1/3 power is 267, so this should never go too high
void motor_set_power(uint8_t power)
{
	MOTORPWM_OCRA = power;
	
	// sanity check anyway
	if (MOTORPWM_OCRA > 267)
		MOTORPWM_OCRA = 267;
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

uint16_t motor_get_current()
{
	//uint16_t result = ADCL;
	//result |= (ADCH << 8);
	
	// ADC = (Vin * 1024) / Vref
	return ADC;
}
