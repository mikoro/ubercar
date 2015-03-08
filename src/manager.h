#ifndef CAR_MANAGER_H
#define CAR_MANAGER_H

typedef enum { STATE_IDLE, STATE_DRIVE, STATE_MEASURE, STATE_RECOVER, STATE_ERROR, NUM_STATES } state_t;

void manager_init();
void manager_set_state(state_t new_state);
void manager_run();

#endif