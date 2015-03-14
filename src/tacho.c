#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "tacho.h"
#include "iomap.h"
#include "setup.h"

typedef enum { STARTING, RUNNING } tacho_state_t;
	
static uint8_t speed = 0;
static uint8_t previous_speed = 0;
static int8_t speed_diff = 0;
static volatile uint16_t distance = 0;
static volatile uint16_t values[TACHO_AVG_AMOUNT] = {0};
static volatile uint8_t value_index = 0;
static tacho_state_t state = STARTING;
static uint8_t has_stopped = 0;

ISR(TACHOREF_COMPA_vect)
{
	uint16_t tachos = TACHO_TCNT;
	values[++value_index % TACHO_AVG_AMOUNT] = tachos;
	distance += tachos;
	
	TACHO_TCNT = 0;
	TACHOREF_TCNT = 0;
}

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

void tacho_reset()
{
	speed = 0;
	previous_speed = 0;
	speed_diff = 0;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		distance = 0;
		
		for (uint8_t i = 0; i < TACHO_AVG_AMOUNT; ++i)
			values[i] = 0;
			
		value_index = 0;
	}
	
	state = STARTING;
	has_stopped = 0;
}

void tacho_reset_distance()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		distance = 0;
	}
}

void tacho_update()
{
	uint16_t speed_sum = 0;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (uint8_t i = 0; i < TACHO_AVG_AMOUNT; ++i)
			speed_sum += values[i];
	}
	
	if (speed_sum > 255)
		speed_sum = 255;
	
	speed = speed_sum;
	speed_diff = (int8_t)((int16_t)speed - (int16_t)previous_speed);
	previous_speed = speed;
	
	if (state == STARTING)
	{
		if (speed > TACHO_STOP_DETECTION_THRESHOLD)
			state = RUNNING;
	}
	else if (state == RUNNING)
	{
		if (speed <= TACHO_STOP_DETECTION_THRESHOLD)
			has_stopped = 1;
	}
}

uint8_t tacho_get_speed()
{
	return speed;
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
	
	return tacho_get_distance() / 14;
}

uint8_t tacho_has_stopped()
{
	return has_stopped;
}

uint8_t tacho_is_accelerating()
{
	return (speed_diff > 0);
}

uint8_t tacho_is_decelerating()
{
	return (speed_diff < 0);
}

uint8_t tacho_is_steady()
{
	return (speed_diff == 0);
}
