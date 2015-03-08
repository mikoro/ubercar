#ifndef CAR_IRSENS_H
#define CAR_IRSENS_H

#include <stdint.h>

void irsens_init();
void irsens_set_stuck_detection(uint8_t value);
void irsens_update();
int8_t irsens_get_location();
uint8_t irsens_is_at_start_line();
uint8_t irsens_is_at_edge();

#endif
