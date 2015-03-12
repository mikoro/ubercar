#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "tacho.h"
#include "iomap.h"
#include "setup.h"

static volatile uint16_t distance = 0;
static volatile uint16_t values[TACHO_AVG_AMOUNT] = {0};
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
	TACHOREF_TCCRB |= BIT(CS32); // divide by 256
	TACHOREF_TIMSK = BIT(OCIE3A); // enable compare interrupt
	TACHOREF_OCRA = 62500 / TACHO_UPDATE_FREQ; // 62500 is one second
	TACHOREF_TCNT = 0;
}

ISR(TACHOREF_COMPA_vect)
{
	uint16_t tachos = TACHO_TCNT;
	values[++value_index % TACHO_AVG_AMOUNT] = tachos;
	distance += tachos;
	
	TACHO_TCNT = 0;
	TACHOREF_TCNT = 0;
}

uint8_t tacho_get_speed()
{
	uint16_t speed_sum = 0;
		
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (uint8_t i = 0; i < TACHO_AVG_AMOUNT; ++i)
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

uint16_t tacho_get_distance_m()
{
	// 73 mm per one tacho reading
	// 1 m = 1000 mm has 1000/73 = 13.7 tachos
	// estimate with with divider = 14
	// TODO needs f16.16 for better accuracy
	
	uint16_t temp_distance;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		temp_distance = distance;
	}
	
	return temp_distance / 14;
}

void tacho_reset_distance()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		distance = 0;
	}
}
