#ifndef CAR_IRSENS_H
#define CAR_IRSENS_H

#include <stdint.h>

void irsens_init();
void irsens_update();
uint8_t irsens_get_direction();

#endif
