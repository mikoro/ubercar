#ifndef CAR_IRSENS_H
#define CAR_IRSENS_H

#include <stdint.h>

typedef enum { LEFT_EDGE, RIGHT_EDGE } edge_t;

void irsens_init();
void irsens_update();
void irsens_reset();
uint8_t irsens_is_stuck();
int8_t irsens_get_location();
uint8_t irsens_is_at_start_line();
uint8_t irsens_is_at_edge();
edge_t irsens_get_last_edge();

#endif
