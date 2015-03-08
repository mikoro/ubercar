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

void state_error_init()
{
	motor_set_power(0);
	motor_set_enabled(0);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_clear();
	lcd_set_bg_color(255, 0, 0);
	lcd_printf(0, 1, 3, 255, 255, 255, "ERROR");
	
	_delay_ms(1000);
	
	motor_set_enabled(1);
	motor_set_direction(MOTOR_FORWARDS);
}

void state_error_update_fixed()
{
}

void state_error_update_fast()
{
}
