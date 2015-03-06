#include <inttypes.h>
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "iomap.h"
#include "steering.h"
#include "motor.h"
#include "lcd.h"
#include "tacho.h"

#include "fixed/fix8.h"

#define PWM_PORT		PORTB
#define PWM_DDR			DDRB

#define TIM_CRA			TCCR1A
#define TIM_CRB			TCCR1B
#define TIM_CRC			TCCR1C

int main(void)
{
	// Leds to output
	LED_DDR = LED0 | LED1;
	LED_PORT = 0x00;

	// Buttons to input
	BTN_DDR = 0x00;

	cli();

	init_lcd();

	init_steering();
	steering_set_direction2(127);
	steering_set_enabled(1);

	init_motor();
	motor_set_duty_cycle2(0);
	motor_set_enabled(1);

	init_tacho();

	uint8_t state = 0;
	uint8_t dir = 0;
	uint8_t dc = 0;

	sei();

	/* Revert to neutral steering position, motor off on button press */
	for (;;) {
		tacho_update();

		uint8_t pressed = !(BTN_PIN & BTN0);

		if (pressed) {

			steering_set_direction2(dir);
			dir += 10;

			motor_set_duty_cycle2(dc);
			dc++;
		}
	}
}
