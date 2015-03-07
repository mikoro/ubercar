#include <avr/interrupt.h>

#include "iomap.h"
#include "led.h"
#include "button.h"
#include "lcd.h"
#include "steering.h"
#include "motor.h"
#include "tacho.h"
#include "irsens.h"
#include "state_manager.h"

int main(void)
{
	cli();
	
	init_led();
	init_button();
	//init_lcd();
	init_steering();
	init_motor();
	init_tacho();
	init_irsens();
	init_state_manager();
	
	sei();
	
	state_manager_set_state(STATE_IDLE);
	state_manager_run();
}
