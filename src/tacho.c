#include <avr/io.h>
#include <avr/interrupt.h>

#include "iomap.h"

static uint16_t tachos = 0;

void init_tacho()
{
	// Set clock pin to input
	TACHO_DDR &= ~TACHO0;
	TACHO_PORT |= TACHO0; // pull-up
	
	// External counter
	// Counter mode, external clock, clock on falling edge
	TACHO_TCRB = BIT(CS51) | BIT(CS52);
	TACHO_TCNT = 0;
	
	// Interrupt reference
	TACHOREF_TCCRB = BIT(WGM32); // CTC mode
	TACHOREF_TCCRB |= BIT(CS31) | BIT(CS30); // divide by 64
	TACHOREF_TIMSK = BIT(OCIE3A); // enable compare interrupt
	TACHOREF_OCRA = 15625; // 250 ms
	TACHOREF_TCNT = 0;
}

ISR(TACHOREF_COMPA_vect)
{
	tachos = TACHO_TCNT;
	TACHO_TCNT = 0;
	TACHOREF_TCNT = 0;
}

uint8_t tacho_get_speed()
{
	return 0;
}
