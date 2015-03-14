#ifndef CAR_MEASURER_H
#define CAR_MEASURER_H

#include <stdint.h>

void measurer_reset();
void measurer_update();
void measurer_print_info();
uint8_t measurer_get_lap_count();

#endif
