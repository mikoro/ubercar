#include <avr/io.h>
#include <avr/interrupt.h>

#include "iomap.h"
#include "lcd.h"
#include "steering.h"
#include "motor.h"
#include "tacho.h"
#include "irsens.h"
#include "state_manager.h"

int main(void)
{
	cli();
	
	// Leds to output
	LED_DDR = LED0 | LED1;
	LED_PORT = 0x00;

	// Buttons to input
	BTN_DDR = 0x00;
/*
	init_lcd();
	init_steering();
	init_motor();
	init_tacho();
	init_irsens();
	init_state_manager();
	*/
	sei();
	
	state_manager_set_state(STATE_IDLE);
	state_manager_run();
}
