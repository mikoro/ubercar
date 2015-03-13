#ifndef CAR_TACHO_H
#define CAR_TACHO_H

#include <stdint.h>

#include "fix8.h"

void tacho_init();
void tacho_reset();
void tacho_reset_distance();
void tacho_update();
uint8_t tacho_get_speed();
uint16_t tacho_get_distance();
uint16_t tacho_get_distance_m();
uint8_t tacho_has_stopped();
uint8_t tacho_is_accelerating();
uint8_t tacho_is_decelerating();
uint8_t tacho_is_steady();

#endif
