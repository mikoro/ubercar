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
	
	lcd_clear();
	lcd_printf(0, 1, 3, 255, 255, 255, "IDLE");
	
	
	//lcd_set_touch_region(0, 0, 240, 320);
	
	//lcd_draw_button(uint8_t state, uint16_t x, uint16_t y, uint8_t rb, uint8_t gb, uint8_t bb, uint8_t font, uint8_t rs, uint8_t gs, uint8_t bs, uint8_t width, uint8_t height, const char *str)
	//lcd_draw_button(1, 10, 10, 31, 0, 0, 3, 31, 63, 31, 220, 100, "press me");
	//lcd_draw_rectangle(10, 10, 220, 100, 15, 32, 15);
	//void lcd_printg(uint16_t x, uint16_t y, uint8_t font, uint8_t proportional, uint8_t r, uint8_t g, uint8_t b, uint8_t width, uint8_t height, const char *str);
	//lcd_printg(30, 20, 3, 1, 31, 63, 31, 3, 3, "DRIVE");
}

void state_idle_update_fixed()
{
	//if (lcd_is_touched())
		//led_toggle1();
		
	if (button_was_released())
	{
		manager_set_state(STATE_DRIVE);
		return;
	}
}

void state_idle_update_fast()
{

}

