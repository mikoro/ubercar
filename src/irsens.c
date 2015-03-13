#include <stdlib.h>
#include <avr/io.h>

#include "irsens.h"
#include "iomap.h"
#include "setup.h"

static const int8_t location_map[8] = IRSENS_LOCATION_MAP;
static const int8_t relative_location_map[8] = IRSENS_RELATIVE_LOCATION_MAP;
static int8_t location = 0;
static uint8_t relative_location = 0;

static int8_t sensor_value = 0;
static int8_t previous_sensor_value = -127;
static uint8_t previous_bit_index = 0;
static side_t change_direction = NONE;
static side_t last_edge = NONE;
static uint8_t same_value_count = 0;

static uint8_t is_at_start_line = 0;
static uint8_t is_at_edge = 0;
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

/*
static uint8_t get_left_bit_index(uint8_t value)
{
	for (int8_t i = 7; i >= 0; --i)
	{
		if (value & BIT(i))
			return i;
	}
	
	return 0;
}
*/

void irsens_init()
{
	IRSENS_DDR = 0x00; // port as input
	IRSENS_PORT = 0x00; // no pull-ups
}

void irsens_reset()
{
	location = 0;
	relative_location = 0;

	sensor_value = 0;
	previous_sensor_value = -127;
	previous_bit_index = 0;
	change_direction = NONE;
	last_edge = NONE;
	same_value_count = 0;

	is_at_start_line = 0;
	is_at_edge = 0;
	is_stuck = 0;
}

void irsens_read_sensor()
{
	uint8_t new_sensor_value = ~IRSENS_PIN;
	
	if (new_sensor_value != 0)
	{
		sensor_value = new_sensor_value;
		is_at_start_line = (get_bit_count(sensor_value) >= 6);
	}
}

void irsens_update()
{
	// detect if we are stuck somewhere and the sensor value never changes
	if (sensor_value == previous_sensor_value)
	{
		++same_value_count;
		
		if (same_value_count > (IRSENS_STUCK_DURATION * CONTROL_FREQ))
		{
			same_value_count = 0;
			is_stuck = 1;
		}
		
		return;
	}
	else
		same_value_count = 0;
	
	previous_sensor_value = sensor_value;
	
	uint8_t bit_count = get_bit_count(sensor_value);
	
	if (bit_count == 0 || bit_count > 1)
		return;
		
	uint8_t bit_index = get_right_bit_index(sensor_value);
	int8_t bit_index_diff = (int8_t)previous_bit_index - (int8_t)bit_index;
	
	// used to eliminate the start line "jumping"
	// TODO needs a counter to reset after x ms
	if (abs(bit_index_diff) > 2)
		return;
		
	previous_bit_index = bit_index;
	location = location_map[bit_index];
	relative_location = relative_location_map[bit_index];
	
	// detect which direction the detections are going
	if (bit_index_diff <= -1)
		change_direction = LEFT;
	else if (bit_index_diff >= 1)
		change_direction = RIGHT;
	else
		change_direction = NONE;
	
	// detect if we are at the edges
	is_at_edge = (bit_index == 7 || bit_index == 0);
	
	// detect on which side of the sensor we are
	if (bit_index <= 3)
		last_edge = RIGHT;
	else
		last_edge = LEFT;
}

int8_t irsens_get_location()
{
	return location;
}

uint8_t irsens_get_relative_location()
{
	return relative_location;
}

uint8_t irsens_is_at_start_line()
{
	return is_at_start_line;
}

void irsens_reset_is_at_start_line()
{
	is_at_start_line = 0;
}

uint8_t irsens_is_at_edge()
{
	return is_at_edge;
}

uint8_t irsens_is_stuck()
{
	return is_stuck;
}

side_t irsens_get_last_edge()
{
	return last_edge;
}
