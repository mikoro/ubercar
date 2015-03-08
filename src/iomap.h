#ifndef CAR_IOMAP_H
#define CAR_IOMAP_H

#include <avr/io.h>

#define BIT(x)			(1<<(x))

#define LED_PORT		PORTC
#define LED_PIN			PINC
#define LED_DDR			DDRC
#define LED0			BIT(PC0)
#define LED1			BIT(PC1)

#define BTN_PORT		PORTE
#define BTN_PIN			PINE
#define BTN_DDR			DDRE
#define BTN0			BIT(PE5)

#define IRSENS_PORT		PORTA
#define IRSENS_PIN		PINA
#define IRSENS_DDR		DDRA

// 16-bit Timer/Counter1 (PWM)
#define STEER_PORT		PORTB
#define STEER_DDR		DDRB
#define STEER_TCCRA		TCCR1A
#define STEER_TCCRB		TCCR1B
#define STEER_TCCRC		TCCR1C
#define STEER_ICR		ICR1
#define STEER_OCRA		OCR1A
#define STEER0			BIT(PB5)

// 16-bit Timer/Counter4 (PWM)
#define MOTORPWM_PORT	PORTH
#define MOTORPWM_DDR	DDRH
#define MOTORPWM_TCCRA	TCCR4A
#define MOTORPWM_TCCRB	TCCR4B
#define MOTORPWM_TCCRC	TCCR4C
#define MOTORPWM_ICR	ICR4
#define MOTORPWM_OCRA	OCR4A
#define MOTORPWM0		BIT(PH3)

#define MOTORCTL_PORT	PORTK
#define MOTORCTL_PIN	PINK
#define MOTORCTL_DDR	DDRK
#define MOTORCTL_INA	BIT(PK0)
#define MOTORCTL_INB	BIT(PK1)
#define MOTORCTL_ENA	BIT(PK2)
#define MOTORCTL_ENB	BIT(PK3)
#define MOTORCTL_CS		BIT(PK4)

#define LCD_PORT		PORTD
#define LCD_DDR			DDRD
#define LCD_UBRRL		UBRR1L
#define LCD_UBRRH		UBRR1H
#define LCD_UCSRA		UCSR1A
#define LCD_UCSRB		UCSR1B
#define LCD_UCSRC		UCSR1C
#define LCD_UDR			UDR1

// 16-bit Timer/Counter5 (Counter)
#define TACHO_PORT		PORTL
#define TACHO_DDR		DDRL
#define TACHO_TCCRA		TCCR5A
#define TACHO_TCCRB		TCCR5B
#define TACHO_TCCRC		TCCR5C
#define TACHO_ICR		ICR5
#define TACHO_OCRA		OCR5A
#define TACHO_TCNT		TCNT5
#define TACHO0			BIT(PL2)

// 16-bit Timer/Counter3 (Interrupt reference)
#define TACHOREF_TCCRA		TCCR3A
#define TACHOREF_TCCRB		TCCR3B
#define TACHOREF_TCCRC		TCCR3C
#define TACHOREF_OCRA		OCR3A
#define TACHOREF_TCNT		TCNT3
#define TACHOREF_TIMSK		TIMSK3
#define TACHOREF_COMPA_vect	TIMER3_COMPA_vect

// 8-bit Timer/Counter0 (Interrupt reference)
#define STATEMANREF_TCCRA		TCCR0A
#define STATEMANREF_TCCRB		TCCR0B
#define STATEMANREF_OCRA		OCR0A
#define STATEMANREF_TCNT		TCNT0
#define STATEMANREF_TIMSK		TIMSK0
#define STATEMANREF_COMPA_vect	TIMER0_COMPA_vect

#endif
