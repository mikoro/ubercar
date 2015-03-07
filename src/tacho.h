#ifndef CAR_TACHO_H
#define CAR_TACHO_H

#include <stdint.h>

void tacho_init();
uint16_t tacho_get_speed();
uint16_t tacho_get_distance();
void tacho_reset_distance();

#endif
