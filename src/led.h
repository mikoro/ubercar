#ifndef CAR_LED_H
#define CAR_LED_H

#include <stdint.h>

void led_init();
void led_set0(uint8_t state);
void led_set1(uint8_t state);
void led_toggle0();
void led_toggle1();

#endif
