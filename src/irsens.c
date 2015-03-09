#include <avr/io.h>

#include "irsens.h"
#include "iomap.h"
#include "setup.h"

static const int8_t sensor_to_location_map[8] = IRSENS_MAPPING;
static int8_t location = 0;
static int8_t previous_sensor_value = 0;
static uint8_t is_at_edge = 0;
static edge_t last_edge = LEFT_EDGE;
static uint8_t same_value_count = 0;
static uint8_t is_stuck = 0;

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

void irsens_update()
{
	uint8_t sensor_value = ~IRSENS_PIN;
	
	// detect if we are stuck somewhere and the sensor value never changes
	if (sensor_value == previous_sensor_value)
	{
		++same_value_count;
		
		if (same_value_count > (STUCK_DURATION * 20))
		{
			same_value_count = 0;
			is_stuck = 1;
		}
	}
	else
		same_value_count = 0;
		
	previous_sensor_value = sensor_value;
	
	uint8_t bit_count = get_bit_count(sensor_value);
	uint8_t left_bit_index = get_left_bit_index(sensor_value);
	uint8_t right_bit_index = get_right_bit_index(sensor_value);
	
	// tape under single sensor
	if (bit_count == 1)
	{
		location = sensor_to_location_map[left_bit_index];
		
		// detect if we are at the extremes for later use
		is_at_edge = (left_bit_index == 7 || right_bit_index == 0);
		
		// detect on which side of the sensor we are for later use
		if (left_bit_index <= 3)
			last_edge = RIGHT_EDGE;
		else
			last_edge = LEFT_EDGE;
	}
	// tape overlapping two sensors
	else if (bit_count == 2)
	{
		int16_t left_value = sensor_to_location_map[left_bit_index];
		int16_t right_value = sensor_to_location_map[right_bit_index];
		
		location = (int8_t)((left_value + right_value) / 2);
	}
	
	// 0 bits set: out of track or tape between two sensors -> keep the previous value
	// >2 bits set: over the start line or car is completely misaligned -> keep the previous value
}

void irsens_reset()
{
	location = 0;
	previous_sensor_value = 0;
	is_at_edge = 0;
	last_edge = LEFT_EDGE;
	same_value_count = 0;
	is_stuck = 0;
}

uint8_t irsens_is_stuck()
{
	return is_stuck;
}

int8_t irsens_get_location()
{
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

edge_t irsens_get_last_edge()
{
	return last_edge;
}
