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

void state_drive_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	pid_steering_reset();
	pid_motor_reset();
	
	irsens_reset();
	tacho_reset();
	
	lcd_draw_header("DRIVE");
}

void state_drive_update_fixed()
{
	if (button_was_released())
	{
		manager_set_state(STATE_MEASURE);
		return;
	}
	
	irsens_update();
	tacho_update();
	
	if ((IRSENS_ENABLE_STUCK_DETECTION && irsens_is_stuck()) || (TACHO_ENABLE_STOP_DETECTION && tacho_has_stopped()))
	{
		manager_set_state(STATE_RECOVER);
		return;
	}
	
	int8_t irsens_location = irsens_get_location();
	uint8_t tacho_speed = tacho_get_speed();
	
	int8_t steering_direction = pid_steering_calculate(irsens_location);
	uint8_t motor_power = pid_motor_calculate(TARGET_SPEED, tacho_speed);
	
	steering_set_direction(steering_direction);
	motor_set_power(motor_power);
}

void state_drive_update_fast()
{
	irsens_read_sensor();
}
