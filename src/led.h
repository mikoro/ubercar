#ifndef CAR_LED_H
#define CAR_LED_H

#include <stdint.h>

void init_led();
void set_led0(uint8_t state);
void set_led1(uint8_t state);
void toggle_led0();
void toggle_led1();

#endif
