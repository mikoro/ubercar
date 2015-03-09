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

void state_idle_init()
{
	motor_set_power(0);
	motor_set_enabled(0);
	motor_set_direction(MOTOR_FORWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_draw_header("IDLE");
	
	//lcd_draw_rectangle(20, 110, 220, 170, 15, 32, 15);
	//lcd_draw_rectangle(20, 180, 220, 240, 15, 32, 15);
	//lcd_draw_rectangle(20, 250, 220, 310, 15, 32, 15);
	
	lcd_draw_button(1, 30, 120, 0, 0, 31, 3, 31, 63, 31, 2, 2, " DRIVE ");
	lcd_draw_button(1, 30, 190, 0, 0, 31, 3, 31, 63, 31, 2, 2, "MEASURE");
	lcd_draw_button(1, 30, 260, 0, 0, 31, 3, 31, 63, 31, 2, 2, "RECOVER");
}

void state_idle_update_fixed()
{
	lcd_set_touch_region(20, 110, 220, 170);
	
	if (button_was_released() || lcd_is_touched())
	{
		manager_set_state(STATE_DRIVE);
		return;
	}
	
	lcd_set_touch_region(20, 180, 220, 240);
	
	if (lcd_is_touched())
	{
		manager_set_state(STATE_MEASURE);
		return;
	}
	
	lcd_set_touch_region(20, 250, 220, 310);
	
	if (lcd_is_touched())
	{
		manager_set_state(STATE_RECOVER);
		return;
	}
}

void state_idle_update_fast()
{

}

