#ifndef CAR_BUTTON_H
#define CAR_BUTTON_H

#include <stdint.h>

void button_init();
void button_update(); // blocks 20 ms if button is touched, otherwise immediate
void button_ignore_next();
uint8_t button_is_down(); // no debounce (immediate)
uint8_t button_was_pressed(); // debounced (needs button update)
uint8_t button_was_released(); // debounced (needs button update)

#endif
