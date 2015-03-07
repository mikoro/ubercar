#ifndef CAR_STEERING_H
#define CAR_STEERING_H

#include <stdint.h>

void steering_init(void);
void steering_set_enabled(uint8_t en);
void steering_set_direction(int8_t dir); // -127 .. 127, 0 is neutral/straight

#endif
