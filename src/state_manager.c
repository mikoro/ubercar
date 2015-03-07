#include "state_manager.h"

#include <util/delay.h>

static state_t current_state = STATE_IDLE;

static state_info_t const state_procs[NUM_STATES] = {
	{state_idle_start, state_idle_update},
	{state_drive_start, state_drive_update},
	{state_measure_start, state_measure_update},
	{state_recover_start, state_recover_update},
};

void init_state_manager()
{
	
}

void state_manager_run()
{
	for(;;)
	{
		state_procs[current_state].update_proc();
	}
}

void state_manager_set_state(state_t new_state)
{
	current_state = new_state;
	state_procs[current_state].start_proc();
}
