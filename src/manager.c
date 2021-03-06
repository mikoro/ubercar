#include <avr/interrupt.h>
#include <util/atomic.h>

#include "manager.h"
#include "states.h"
#include "iomap.h"
#include "led.h"
#include "button.h"
#include "lcd.h"
#include "motor.h"
#include "steering.h"
#include "tacho.h"
#include "irsens.h"
#include "setup.h"

typedef void (*state_proc_t)(void);

typedef struct {
	state_proc_t init_proc;
	state_proc_t update_fixed_proc;
	state_proc_t update_fast_proc;
} state_info_t;

static state_info_t const state_procs[NUM_STATES] = {
	{state_idle_init, state_idle_update_fixed, state_idle_update_fast},
	{state_drive_init, state_drive_update_fixed, state_drive_update_fast},
	{state_recover_init, state_recover_update_fixed, state_recover_update_fast},
	{state_error_init, state_error_update_fixed, state_error_update_fast},
};

static state_t current_state = STATE_IDLE;
static volatile uint8_t loop_elapsed_ms = 0;
static volatile uint16_t timer_elapsed_ms = 0;
static uint16_t led_toggle_counter = 0;
static uint8_t button_down_count = 0;

ISR(STATEMANREF_COMPA_vect)
{
	++loop_elapsed_ms;
	++timer_elapsed_ms;
	
	STATEMANREF_TCNT = 0;
}

void manager_init()
{
	// Reference clock (interrupt)
	STATEMANREF_TCCRA = BIT(WGM01); // CTC mode
	STATEMANREF_TCCRB = BIT(CS01) | BIT(CS00); // 64 divider
	STATEMANREF_TIMSK = BIT(OCIE0A); // enable compare interrupt
	STATEMANREF_OCRA = 250; // 1 ms
	STATEMANREF_TCNT = 0;
}

void manager_set_state(state_t new_state)
{
	current_state = new_state;
	state_procs[current_state].init_proc();
}

void manager_run()
{
	for(;;)
	{
		button_update();
		
		state_procs[current_state].update_fixed_proc();
		
		if (motor_get_status() != 0 && current_state != STATE_ERROR)
			manager_set_state(STATE_ERROR);
			
		if (button_is_down() && current_state != STATE_IDLE)
		{
			// 1 second
			if (++button_down_count >= (1 * CONTROL_FREQ))
			{
				manager_set_state(STATE_IDLE);
				button_ignore_next();
				button_down_count = 0;
			}
		}
		else
			button_down_count = 0;
		
		// blink the other led at 1 Hz
		if (++led_toggle_counter % (CONTROL_FREQ / 2) == 0)
			led_toggle0();
		
		// lock the fixed loop to given Hz
		// no atomic block (one byte read)
		while (loop_elapsed_ms < TIME_STEP_MS)
		{
			// while waiting, do non-fixed-time updates
			state_procs[current_state].update_fast_proc();
		}
		
		loop_elapsed_ms = 0;
	}
}

void manager_reset_timer()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		timer_elapsed_ms = 0;
	}
}

uint16_t manager_get_timer_elapsed_ms()
{
	uint16_t timer_elapsed_ms_temp;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		timer_elapsed_ms_temp = timer_elapsed_ms;
	}
	
	return timer_elapsed_ms_temp;
}
