#ifndef CAR_STATES_H
#define CAR_STATES_H

// remember to try -fshort-enums!
typedef enum { STATE_IDLE, STATE_DRIVE, STATE_MEASURE, STATE_RECOVER, NUM_STATES } state_t;
typedef void (*state_proc_t)(void);

typedef struct {
	state_proc_t start_proc;
	state_proc_t update_proc;
} state_info_t;

void state_manager_init();
void state_manager_run();
void state_manager_set_state(state_t new_state);

void state_idle_start();
void state_idle_update();
void state_drive_start();
void state_drive_update();
void state_measure_start();
void state_measure_update();
void state_recover_start();
void state_recover_update();

#endif
