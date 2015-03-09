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

void state_recover_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_draw_header("RECOVER");
}

void state_recover_update_fixed()
{
	if (button_was_released())
	{
		manager_set_state(STATE_IDLE);
		return;
	}
}

void state_recover_update_fast()
{
}
