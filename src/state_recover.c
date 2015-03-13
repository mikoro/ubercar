#include "states.h"
#include "manager.h"
#include "iomap.h"
#include "led.h"
#include "button.h"
#include "lcd.h"
#include "motor.h"
#include "steering.h"
#include "tacho.h"
#include "irsens.h"
#include "measurer.h"
#include "pid.h"
#include "setup.h"

typedef enum { STOPPING, STOPPED, DRIFT } recover_state_t;

static recover_state_t state = STOPPING;
static uint16_t timer = 0;

void state_recover_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_BACKWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_draw_header("RECOVER");
	lcd_printg(45, 130, 3, 0, 31, 31, 0, 12, 12, "!");
	
	state = STOPPING;
	timer = 0;
}

void state_recover_update_fixed()
{
	if (button_was_released())
	{
		manager_set_state(STATE_IDLE);
		return;
	}
	
	irsens_update();
	tacho_update();
	
	int8_t irsens_location = irsens_get_location();
	uint8_t tacho_speed = tacho_get_speed();
	
	if (state == STOPPING)
	{
		uint8_t motor_power = pid_motor_calculate(0, tacho_speed);
		motor_set_power(motor_power);
		
		if (tacho_is_accelerating() || tacho_speed < 2)
		{
			motor_set_power(0);
			state = STOPPED;
		}
	}
	
	if (state == DRIFT)
	{
		++timer;
		
		if (timer >= (0 * CONTROL_FREQ) && timer <= (8 * CONTROL_FREQ))
		{
			steering_set_direction(127);
		}
		
		if (timer >= (1 * CONTROL_FREQ) && timer <= (8 * CONTROL_FREQ))
		{
			motor_set_power_nolimit(672);
		}
		else
			motor_set_power_nolimit(0);
			
		if (timer > (8 * CONTROL_FREQ))
		{
			steering_set_direction(0);
			state = STOPPED;
			timer = 0;
		}
	}
}

void state_recover_update_fast()
{
	irsens_read_sensor();
}
