#ifndef CAR_TACHO_H
#define CAR_TACHO_H

#include <stdint.h>

#include "fix8.h"

void tacho_init();
uint8_t tacho_get_speed();
uint16_t tacho_get_distance();
uint16_t tacho_get_distance_m();
void tacho_reset_distance();

#endif
