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
#include "setup.h"

typedef enum { STOPPING, STOPPED, DRIFT } recover_state_t;

static recover_state_t state = STOPPING;
static uint8_t update_count = 0;

void state_recover_init()
{
	motor_set_power(0);
	motor_set_enabled(1);
	motor_set_direction(MOTOR_BACKWARDS);
	
	steering_set_direction(0);
	steering_set_enabled(1);
	
	lcd_draw_header("RECOVER");
	measurer_print_info();
	
	state = STOPPING;
	update_count = 0;
}

void state_recover_update_fixed()
{
	if (button_was_released())
	{
		manager_set_state(STATE_IDLE);
		return;
	}
	
	irsens_update();
	tacho_update();
	measurer_update();
	
	if (state == STOPPING)
	{
		controller_motor_set_target_speed(0);
		controller_motor_update_pid();
		
		if (tacho_is_accelerating())
		{
			motor_set_power(0);
			state = STOPPED;
		}
	}
	
	if (state == DRIFT)
	{
		++update_count;
		
		if (update_count >= (0 * CONTROL_FREQ) && update_count <= (8 * CONTROL_FREQ))
		{
			steering_set_direction(127);
		}
		else
			steering_set_direction(0);
		
		if (update_count >= (1 * CONTROL_FREQ) && update_count <= (8 * CONTROL_FREQ))
		{
			motor_set_power_nolimit(672);
		}
		else
			motor_set_power_nolimit(0);
			
		if (update_count > (8 * CONTROL_FREQ))
		{
			state = STOPPED;
			update_count = 0;
		}
	}
}

void state_recover_update_fast()
{
	irsens_read_sensor();
}
