#include <util/delay.h>

#include "button.h"
#include "iomap.h"

static uint8_t last_state = 0;
static uint8_t was_pressed = 0;
static uint8_t was_released = 0;
static uint8_t ignore_next = 0;

void button_init()
{
	// button to input
	BTN_DDR &= ~BTN0;
	
	// enable pull-up
	BTN_PORT |= BTN0;
}

void button_update()
{
	uint8_t new_state = !(BTN_PIN & BTN0);
	uint8_t state_changed = 0;
	
	if (new_state != last_state)
	{
		// debounce
		_delay_ms(20);
		new_state = !(BTN_PIN & BTN0);
		state_changed = (new_state != last_state);
	}
	
	if (state_changed)
	{
		if (ignore_next)
			ignore_next = 0;
		else
		{
			was_pressed = (new_state && !last_state);
			was_released = !was_pressed;
		}
	}
	else
	{
		was_pressed = 0;
		was_released = 0;
	}
	
	last_state = new_state;
}

void button_ignore_next()
{
	ignore_next = 1;
}

uint8_t button_is_down()
{
	return !(BTN_PIN & BTN0);
}

uint8_t button_was_pressed()
{
	return was_pressed;
}

uint8_t button_was_released()
{
	return was_released;
}
