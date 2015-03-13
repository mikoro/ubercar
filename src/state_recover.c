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

typedef enum { STOPPING, STOPPED } recover_state_t;

static recover_state_t state = STOPPING;

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
}

void state_recover_update_fast()
{
	irsens_read_sensor();
}
