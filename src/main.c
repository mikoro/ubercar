#include <inttypes.h>
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "iomap.h"
#include "steering.h"

int main(void)
{
	// Leds to output
	LED_DDR = 0xff;
	LED_PORT = 0x00;

	// Buttons to input
	BTN_DDR = 0x00;

	init_steering();

	for (;;) {
	}

	return 0;
}
