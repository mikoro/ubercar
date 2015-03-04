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

	init_lcd();

	//lcd_printf(3, "42 hey");

	init_steering();
	steering_set_direction(-600);
	steering_set_enabled(1);

	init_motor();
	motor_set_duty_cycle(100);
	motor_set_enabled(1);

	init_tacho();

	uint8_t state = 0;

	/* Revert to neutral steering position on button press */
	for (;;) {
		tacho_update();

		uint8_t pressed = !(BTN_PIN & BTN0);

		if (pressed) {
			steering_set_direction(0);
			motor_set_enabled(0);
			LED_PORT = LED0;
		}
	}
}
