#ifndef CAR_MEASURER_H
#define CAR_MEASURER_H

#include <stdint.h>

void measurer_reset();

void measurer_measure_update();
uint8_t measurer_is_finished();

void measurer_race_update();
uint8_t measurer_is_on_straight();

#endif
