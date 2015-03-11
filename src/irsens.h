#ifndef CAR_IRSENS_H
#define CAR_IRSENS_H

#include <stdint.h>

typedef enum { LEFT, RIGHT, NONE } side_t;

void irsens_init();
void irsens_reset();
void irsens_read_sensor();
void irsens_update();
int8_t irsens_get_location();
uint8_t irsens_get_relative_location();
uint8_t irsens_is_at_start_line();
void irsens_reset_is_at_start_line();
uint8_t irsens_is_at_edge();
uint8_t irsens_is_stuck();
side_t irsens_get_last_edge();

#endif
