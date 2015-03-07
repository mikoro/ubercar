#include "state_manager.h"
#include "iomap.h"
#include "led.h"

#include <avr/interrupt.h>

static state_t current_state = STATE_IDLE;
static volatile uint8_t ms_elapsed = 0;
static uint8_t led_toggle_counter = 0;

static state_info_t const state_procs[NUM_STATES] = {
	{state_idle_start, state_idle_update},
	{state_drive_start, state_drive_update},
	{state_measure_start, state_measure_update},
	{state_recover_start, state_recover_update},
};

void init_state_manager()
{
	// Reference clock (interrupt)
	STATEMANREF_TCCRA = BIT(WGM01); // CTC mode
	STATEMANREF_TCCRB = BIT(CS01) | BIT(CS00); // 64 divider
	STATEMANREF_TIMSK = BIT(OCIE0A); // enable compare interrupt
	STATEMANREF_OCRA = 250; // 1 ms
	STATEMANREF_TCNT = 0;
}

ISR(STATEMANREF_COMPA_vect)
{
	++ms_elapsed;
	STATEMANREF_TCNT = 0;
}

void state_manager_run()
{
	for(;;)
	{
		state_procs[current_state].update_proc();
		
		if (++led_toggle_counter % 4 == 0)
			led_toggle0();
		
		// lock the loop to 20 Hz
		while (ms_elapsed < 50)
		{
			// busy wait
		}
		
		ms_elapsed = 0;
	}
}

void state_manager_set_state(state_t new_state)
{
	current_state = new_state;
	state_procs[current_state].start_proc();
}
