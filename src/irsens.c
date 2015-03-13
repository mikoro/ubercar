#include <stdlib.h>
#include <avr/io.h>

#include "irsens.h"
#include "iomap.h"
#include "setup.h"

static const int8_t location_map[8] = IRSENS_LOCATION_MAP;
static const int8_t speed_decrease_map[8] = IRSENS_SPEED_DECREASE_MAP;
static int8_t location = 0;
static uint8_t speed_decrease = 0;

static int8_t sensor_value = 0;
static int8_t previous_sensor_value = -127;
static uint8_t previous_bit_index = 0;
static side_t change_direction = NONE;
static side_t last_edge = NONE;
static uint8_t same_value_count = 0;
static uint8_t start_line_count = 0;

static uint8_t has_crossed_start_line = 0;
static uint8_t is_sensing = 0;
static uint8_t is_at_center = 0;
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
	speed_decrease = 0;

	sensor_value = 0;
	previous_sensor_value = -127;
	previous_bit_index = 0;
	change_direction = NONE;
	last_edge = NONE;
	same_value_count = 0;
	start_line_count = 0;
	
	has_crossed_start_line = 0;
	is_sensing = 0;
	is_at_center = 0;
	is_at_edge = 0;
	is_stuck = 0;
}

void irsens_read_sensor()
{
	uint8_t new_sensor_value = ~IRSENS_PIN;
	
	// update the sensor value only if it is not zero (i.e stick with the latest non-zero reading)
	// if set to zero, the update function (which is called less frequently) will miss sensor readings
	if (new_sensor_value != 0)
	{
		sensor_value = new_sensor_value;
		is_sensing = 1;
		
		// TODO needs testing for the right bit amount
		if (get_bit_count(sensor_value) >= 3)
		{
			has_crossed_start_line = 1;
			start_line_count = 0;
		}
	}
	else
		is_sensing = 0;
}

void irsens_update()
{
	// keep the steering constant for 200 ms after entering the start line
	// this will prevent the sudden steerings when crossing the start line with a little angle
	// TODO test if actually works and tune timing
	if (start_line_count < (200 / (uint8_t)TIME_STEP_MS))
	{
		++start_line_count;
		sensor_value = 0x08;
	}
	
	// detect if we are stuck somewhere and the sensor value never changes
	if (sensor_value == previous_sensor_value)
	{
		++same_value_count;
		
		if (same_value_count > (IRSENS_STUCK_DURATION * CONTROL_FREQ))
		{
			same_value_count = 0;
			is_stuck = 1;
		}
	}
	else
		same_value_count = 0;
	
	previous_sensor_value = sensor_value;
	
	uint8_t bit_count = get_bit_count(sensor_value);
	
	// skip situations where no good data can be calculated (location value will stay the same)
	if (bit_count == 0 || bit_count > 1)
		return;
		
	uint8_t bit_index = get_right_bit_index(sensor_value);
	int8_t bit_index_diff = (int8_t)previous_bit_index - (int8_t)bit_index;
		
	previous_bit_index = bit_index;
	location = location_map[bit_index];
	speed_decrease = speed_decrease_map[bit_index];
	
	// detect which direction the detections are going
	if (bit_index_diff <= -1)
		change_direction = LEFT;
	else if (bit_index_diff >= 1)
		change_direction = RIGHT;
	else
		change_direction = NONE;
		
	// detect if we are at the center
	is_at_center = (bit_index == 4 || bit_index == 3);
	
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

uint8_t irsens_get_speed_decrease()
{
	return speed_decrease;
}

uint8_t irsens_has_crossed_start_line()
{
	return has_crossed_start_line;
}

void irsens_reset_has_crossed_start_line()
{
	has_crossed_start_line = 0;
}

uint8_t irsens_is_sensing()
{
	return is_sensing;
}

uint8_t irsens_is_at_center()
{
	return is_at_center;
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
