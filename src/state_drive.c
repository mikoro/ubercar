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

void state_drive_init()
{
	motor_set_duty_cycle2(0);
	motor_set_enabled(1);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_printf(0, "DRIVE");
}

void state_drive_update_fixed()
{
	if (button_was_released())
		manager_set_state(STATE_MEASURE);
}

void state_drive_update_fast()
{
}
