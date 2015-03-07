#ifndef CAR_STEERING_H
#define CAR_STEERING_H

#include <stdint.h>

void steering_init(void);
void steering_set_enabled(uint8_t en);
void steering_set_duty_cycle(uint16_t dc);
void steering_set_direction(int16_t dir);
void steering_set_direction2(uint8_t dir);

#endif
