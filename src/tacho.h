#ifndef CAR_TACHO_H
#define CAR_TACHO_H

#include <stdint.h>

void init_tacho(void);
void tacho_update();
uint8_t tacho_get_speed();

#endif

