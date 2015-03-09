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

static int8_t irsens_location_ref = -18;
static uint8_t tacho_speed_ref = NORMAL_SPEED;
static uint8_t button_down_count = 0;

void state_drive_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	pid_steering_reset();
	pid_motor_reset();
	
	irsens_set_stuck_detection(ENABLE_STUCK_DETECTION);
	
	lcd_draw_header("DRIVE");
}

void state_drive_update_fixed()
{
	if (button_was_released())
	{
		manager_set_state(STATE_MEASURE);
		return;
	}
	
	if (button_is_down())
	{
		// 1 second
		if (++button_down_count >= 20)
		{
			manager_set_state(STATE_IDLE);
			button_ignore_next();
			button_down_count = 0;
			
			return;
		}
	}
	else
		button_down_count = 0;
	
	irsens_update();
	
	int8_t irsens_location = irsens_get_location();
	uint8_t tacho_speed = tacho_get_speed();
	
	//int8_t steering_direction = pid_steering_calculate(irsens_location_ref, irsens_location);
	uint8_t motor_power = pid_motor_calculate(tacho_speed_ref, tacho_speed);
	
	steering_set_direction(irsens_location);
	motor_set_power(motor_power);
}

void state_drive_update_fast()
{
}
