#include <util/delay.h>

#include "led.h"
#include "iomap.h"

void led_init()
{
	// set led pins to output
	LED_DDR = LED0 | LED1;
	
	// turn off
	LED_PORT &= ~(LED0 | LED1);
}

void led_set0(uint8_t state)
{
	if (state)
		LED_PORT |= LED0;
	else
		LED_PORT &= ~LED0;
}

void led_set1(uint8_t state)
{
	if (state)
		LED_PORT |= LED1;
	else
		LED_PORT &= ~LED1;
}

void led_toggle0()
{
	LED_PORT ^= LED0;
}

void led_toggle1()
{
	LED_PORT ^= LED1;
}

void led_error_loop()
{
	led_set0(0);
	led_set1(0);
	
	for (;;)
	{
		led_toggle0();
		led_toggle1();
		_delay_ms(500);
	}
}
