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

	sei();
	
	//demo_loop();
	
	int16_t targetPower = 0;

	for (;;) {
		tacho_update();
		irsens_update();

		steering_set_direction2(irsens_get_direction());
		
		uint8_t currentSpeed = tacho_get_speed();
		int16_t speedDelta = 20 - currentSpeed;
		targetPower += speedDelta / 10;
		
		if (targetPower < 0)
			targetPower = 0;
			
		if (targetPower > 255)
			targetPower = 255;
			
		motor_set_duty_cycle2(targetPower);

		/*
		uint8_t pressed = !(BTN_PIN & BTN0);
		if (pressed && !was_pressed) {
			was_pressed = 1;
		}
		if (!pressed)
			was_pressed = 0;
		*/
		
		_delay_ms(10);
	}
}

void demo_loop()
{
	uint8_t dir = 127;
	int8_t dirInc = 1;
	
	uint8_t power = 0;
	int8_t powerInc = 1;
	
	uint8_t divider = 0;
	
	for (;;) {
		tacho_update();
		irsens_update();
		
		if (dir == 255)
			dirInc = -1;
			
		if (dir == 0)
			dirInc = 1;
			
		if (power == 255)
			powerInc = -1;
			
		if (power == 0)
			powerInc = 1;
		
		dir += dirInc;
		
		if (++divider % 3 == 0)
		{
			power += powerInc;
		}
		
		steering_set_direction2(dir);
		motor_set_duty_cycle2(power);
		
		_delay_ms(2);
	}
}
