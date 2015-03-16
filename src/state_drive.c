#include <stdio.h>

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
#include "controller.h"
#include "setup.h"

void state_drive_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	controller_steering_reset();
	controller_motor_reset();
	
	irsens_reset();
	tacho_reset();
	measurer_reset();
	
	lcd_draw_header("DRIVE");
	measurer_print_info();
}

void state_drive_update_fixed()
{
	if (button_was_released())
	{
		manager_set_state(STATE_RECOVER);
		return;
	}
	
	if ((IRSENS_ENABLE_STUCK_DETECTION && irsens_is_stuck()) || (TACHO_ENABLE_STOP_DETECTION && tacho_has_stopped()))
	{
		manager_set_state(STATE_RECOVER);
		return;
	}
	
	irsens_update();
	tacho_update();
	measurer_update();
	
	controller_motor_set_target_speed(TARGET_SPEED - irsens_get_speed_decrease());
	controller_motor_update_pid();
	controller_steering_update_pid();
	//controller_steering_update_fixed();
}

void state_drive_update_fast()
{
	irsens_read_sensor();
}
