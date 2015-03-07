#include "led.h"
#include "iomap.h"

void init_led()
{
	// leds to output
	LED_DDR = LED0 | LED1;
	
	// turn off
	LED_PORT &= ~(LED0 | LED1);
}

void set_led0(uint8_t state)
{
	if (state)
		LED_PORT |= LED0;
	else
		LED_PORT &= ~LED0;
}

void set_led1(uint8_t state)
{
	if (state)
		LED_PORT |= LED1;
	else
		LED_PORT &= ~LED1;
}

void toggle_led0()
{
	LED_PORT ^= LED0;
}

void toggle_led1()
{
	LED_PORT ^= LED1;
}
