#ifndef CAR_IRSENS_H
#define CAR_IRSENS_H

#include <stdint.h>

typedef enum { LEFT, RIGHT, NONE } side_t;

void irsens_init();
void irsens_reset();
void irsens_read_sensor();
void irsens_update();
int8_t irsens_get_location_fixed();
int8_t irsens_get_location_pid();
uint8_t irsens_get_speed_decrease();
uint8_t irsens_has_crossed_start_line();
void irsens_reset_has_crossed_start_line();
uint8_t irsens_is_sensing();
uint8_t irsens_is_at_center();
uint8_t irsens_is_at_edge();
uint8_t irsens_is_stuck();
side_t irsens_get_last_edge();

#endif
