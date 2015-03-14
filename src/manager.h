#ifndef CAR_MANAGER_H
#define CAR_MANAGER_H

#include <stdint.h>

typedef enum { STATE_IDLE, STATE_DRIVE, STATE_MEASURE, STATE_RECOVER, STATE_ERROR, NUM_STATES } state_t;

void manager_init();
void manager_set_state(state_t new_state);
void manager_run();
void manager_reset_timer();
uint16_t manager_get_timer_elapsed_ms();

#endif
