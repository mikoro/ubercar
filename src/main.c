#include <inttypes.h>
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "iomap.h"
#include "steering.h"

#include "fixed/fix8.h"

int main(void)
{
	// Leds to output
	LED_DDR = 0xff;
	LED_PORT = 0x00;

	// Buttons to input
	BTN_DDR = 0x00;

	fix8_t a = F8(1.5);
	fix8_t b = F8(2.5);
	fix8_t c = fix8_mul(a, b);
	(void)c;

	init_steering();

	for (;;) {
	}

	return 0;
}
