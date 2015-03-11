#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "tacho.h"
#include "iomap.h"

#define AVG_AMOUNT 3

static volatile uint16_t distance = 0;
static volatile uint16_t values[AVG_AMOUNT] = {0};
static volatile uint8_t value_index = 0;

void tacho_init()
{
	// Set clock pin to input
	TACHO_DDR &= ~TACHO0;
	TACHO_PORT |= TACHO0; // pull-up
	
	// External counter
	// Counter mode, external clock, clock on falling edge
	TACHO_TCCRB = BIT(CS51) | BIT(CS52);
	TACHO_TCNT = 0;
	
	// Interrupt reference
	TACHOREF_TCCRB = BIT(WGM32); // CTC mode
	TACHOREF_TCCRB |= BIT(CS31) | BIT(CS30); // divide by 64
	TACHOREF_TIMSK = BIT(OCIE3A); // enable compare interrupt
	TACHOREF_OCRA = 12500; // 50 ms 20 Hz
	TACHOREF_TCNT = 0;
}

ISR(TACHOREF_COMPA_vect)
{
	uint16_t tachos = TACHO_TCNT;
	values[++value_index % AVG_AMOUNT] = tachos;
	distance += tachos;
	
	TACHO_TCNT = 0;
	TACHOREF_TCNT = 0;
}

uint8_t tacho_get_speed()
{
	uint16_t speed_sum = 0;
		
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (uint8_t i = 0; i < AVG_AMOUNT; ++i)
			speed_sum += values[i];
	}
	
	if (speed_sum > 255)
		speed_sum = 255;
		
	return speed_sum;
}

uint16_t tacho_get_distance()
{
	uint16_t temp_distance;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		temp_distance = distance;
	}
	
	return temp_distance;
}

void tacho_reset_distance()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		distance = 0;
	}
}

