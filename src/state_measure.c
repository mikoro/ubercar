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

void state_measure_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_clear();
	lcd_printf(0, 1, 3, 255, 255, 255, "MEASURE");
}

void state_measure_update_fixed()
{
	if (button_was_released())
		manager_set_state(STATE_IDLE);
}

void state_measure_update_fast()
{
}
