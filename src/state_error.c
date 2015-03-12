#include <util/delay.h>

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

void state_error_init()
{
	motor_set_power(0);
	motor_set_enabled(0);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_draw_header("ERROR");
	
	uint8_t status = motor_get_status();
	
	switch (status)
	{
		case 1: LCD_PRINTF(7, "Half-bridge A error"); break;
		case 2: LCD_PRINTF(7, "Half-bridge B error"); break;
		case 3: LCD_PRINTF(7, "Half-bridge A & B error"); break;
		default: LCD_PRINTF(7, "Unknown error"); break;
	}
}

void state_error_update_fixed()
{
	if (button_was_released())
	{
		manager_set_state(STATE_IDLE);
		return;
	}
}

void state_error_update_fast()
{
}
