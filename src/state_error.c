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
#include "controller.h"

void state_error_init()
{
	motor_set_power(0);
	motor_set_enabled(0);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_draw_header("ERROR");
	lcd_printg(45, 130, 3, 0, 31, 0, 0, 12, 12, "!");
	
	uint8_t status = motor_get_status();
	char* msg;
	
	switch (status)
	{
		case 1: msg = "Half-bridge A error"; break;
		case 2: msg = "Half-bridge B error"; break;
		case 3: msg = "Half-bridge A & B error"; break;
		default: msg = "Unknown error"; break;
	}
	
	lcd_printg(5, 305, 2, 0, 31, 63, 31, 1, 1, msg);
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
