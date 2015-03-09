#include <avr/io.h>

#include "irsens.h"
#include "iomap.h"
#include "lcd.h"
#include "manager.h"

typedef enum { LEFT_EDGE, RIGHT_EDGE } edge_t;
	
static int8_t location = 0;
static int8_t previous_value = 0;
static uint8_t is_at_edge = 0;
static edge_t last_edge_type = LEFT_EDGE;
static uint8_t stuck_detection_enabled = 1;
static uint8_t same_value_count = 0;

static uint8_t get_bit_count(uint8_t value)
{
	uint8_t bit_count = 0;
	
	for (uint8_t i = 0; i <= 7; ++i)
	{
		if (value & BIT(i))
			++bit_count;
	}
	
	return bit_count;
}

static uint8_t get_right_bit_index(uint8_t value)
{
	for (uint8_t i = 0; i <= 7; ++i)
	{
		if (value & BIT(i))
			return i;
	}
	
	return 0;
}

static uint8_t get_left_bit_index(uint8_t value)
{
	for (int8_t i = 7; i >= 0; --i)
	{
		if (value & BIT(i))
			return i;
	}
	
	return 0;
}

void irsens_init() {
	IRSENS_DDR = 0x00; // port as input
	IRSENS_PORT = 0x00; // no pull-ups
}

void irsens_set_stuck_detection(uint8_t value)
{
	stuck_detection_enabled = value;
}

void irsens_update()
{
	static const int8_t mapping[8] = {127, 91, 54, 18, -18, -54, -91, -127};
	
	uint8_t value = ~IRSENS_PIN;
	
	// detect if we are stuck somewhere and the sensor value never changes
	if (stuck_detection_enabled && value == previous_value)
	{
		++same_value_count;
		
		// 5 s with 20 Hz
		if (same_value_count > 100)
		{
			same_value_count = 0;
			manager_set_state(STATE_RECOVER);
			
			return;
		}
	}
	else
		same_value_count = 0;
		
	previous_value = value;
	
	uint8_t bit_count = get_bit_count(value);
	uint8_t left_bit_index = get_left_bit_index(value);
	uint8_t right_bit_index = get_right_bit_index(value);
	
	// either out of track or tape between sensors, keep using the previous value
	if (bit_count == 0)
	{
		return;
	}
	// tape under single sensor
	else if (bit_count == 1)
	{
		location = mapping[left_bit_index];
		
		// detect if we are at the extremes for later use
		is_at_edge = (left_bit_index == 7 || right_bit_index == 0);
		
		// detect on which side of the sensor we are for later use
		if (left_bit_index <= 3)
			last_edge_type = RIGHT_EDGE;
		else
			last_edge_type = LEFT_EDGE;
	}
	// tape overlapping two sensors
	else if (bit_count == 2)
	{
		int16_t left_value = mapping[left_bit_index];
		int16_t right_value = mapping[right_bit_index];
		
		location = (int8_t)((left_value + right_value) / 2);
	}
	// car is completely misaligned
	// get the rightmost or leftmost value, preferring the side which has a reading closer to the edge
	else if (bit_count <= 4) // skip the start line situation
	{
		if (7 - left_bit_index < right_bit_index)
			location = mapping[left_bit_index];
		else
			location = mapping[right_bit_index];
	}
}

int8_t irsens_get_location() {
	return location;
}

uint8_t irsens_is_at_start_line()
{
	uint8_t value = ~IRSENS_PIN;
	
	return (get_bit_count(value) >= 6);
}

uint8_t irsens_is_at_edge()
{
	return is_at_edge;
}
