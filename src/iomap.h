#ifndef CAR_IOMAP_H
#define CAR_IOMAP_H

#include <avr/io.h>

#define BIT(x)			(1<<(x))

#define LED_PORT		PORTC
#define LED_DDR			DDRC
#define LED0			BIT(0)
#define LED1			BIT(1)

#define BTN_PIN			PINE
#define BTN_DDR			DDRE
#define BTN0			BIT(5)

#define SENS_PIN		PINA
#define SENS_DDR		DDRA

#define STEER_PORT		PORTB
#define STEER_DDR		DDRB
#define STEER_TCRA		TCCR1A
#define STEER_TCRB		TCCR1B
#define STEER_TCRC		TCCR1C
#define STEER_ICR		ICR1
#define STEER_OCRA		OCR1A
#define STEER0			BIT(5)

#define MOTORPWM_PORT		PORTH
#define MOTORPWM_DDR		DDRH
#define MOTORPWM_TCRA		TCCR4A
#define MOTORPWM_TCRB		TCCR4B
#define MOTORPWM_TCRC		TCCR4C
#define MOTORPWM_ICR		ICR4
#define MOTORPWM_OCRA		OCR4A

#endif
