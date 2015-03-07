#include "state_manager.h"
#include "iomap.h"
#include "led.h"
#include "button.h"
#include "lcd.h"
#include "motor.h"
#include "steering.h"
#include "tacho.h"
#include "irsens.h"

void state_measure_start()
{
	motor_set_duty_cycle2(0);
	motor_set_enabled(1);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_printf(0, "MEASURE");
}

void state_measure_update()
{
	if (button_was_released())
		state_manager_set_state(STATE_RECOVER);
}
