#ifndef CAR_BUTTON_H
#define CAR_BUTTON_H

#include <stdint.h>

void button_init();
void button_update();
uint8_t button_is_down(); // no debounce (immediate)
uint8_t button_was_pressed(); // debounced (possible 20 ms delay)
uint8_t button_was_released(); // debounced (possible 20 ms delay)

#endif
