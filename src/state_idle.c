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

void state_idle_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_set_touch_region(0, 0, 240, 320);
	
	lcd_draw_header("IDLE");
	measurer_print_info();
	
	lcd_set_transparent_font(1);
	lcd_draw_button(1, 30, 190, 0, 0, 31, 3, 31, 63, 31, 2, 2, " DRIVE ");
	lcd_set_transparent_font(0);
}

void state_idle_update_fixed()
{
	if (button_was_released() || lcd_is_touched())
	{
		manager_set_state(STATE_DRIVE);
		return;
	}
}

void state_idle_update_fast()
{

}
