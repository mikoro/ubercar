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
	
	led_init();
	button_init();
	//lcd_init();
	steering_init();
	motor_init();
	tacho_init();
	irsens_init();
	state_manager_init();
	
	sei();
	
	state_manager_set_state(STATE_IDLE);
	state_manager_run();
}
