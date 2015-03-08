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

static int8_t steering_angle_ref = 0;
static uint8_t tacho_speed_ref = 30;

void state_drive_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_clear();
	lcd_printf(0, 1, 3, 255, 255, 255, "DRIVE");
	
	pid_steering_reset();
	pid_motor_reset();
	
	pid_steering_set_kp(F8(1.0));
	pid_steering_set_ki(F8(1.0));
	pid_steering_set_kd(F8(1.0));
	
	pid_motor_set_kp(F8(1.0));
	pid_motor_set_ki(F8(1.0));
	pid_motor_set_kd(F8(1.0));
}

void state_drive_update_fixed()
{
	if (button_was_released())
		manager_set_state(STATE_IDLE);
		
	int8_t irsens_location = irsens_get_location();
	uint8_t tacho_speed = tacho_get_speed();
	
	int8_t steering_angle = pid_steering_calculate(steering_angle_ref, irsens_location);
	uint8_t motor_power = pid_motor_calculate(tacho_speed_ref, tacho_speed);
	
	steering_set_direction(steering_angle);
	motor_set_power(motor_power);
}

void state_drive_update_fast()
{
}
