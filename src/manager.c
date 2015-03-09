#include <avr/interrupt.h>

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

typedef void (*state_proc_t)(void);

typedef struct {
	state_proc_t init_proc;
	state_proc_t update_fixed_proc;
	state_proc_t update_fast_proc;
} state_info_t;

static state_info_t const state_procs[NUM_STATES] = {
	{state_idle_init, state_idle_update_fixed, state_idle_update_fast},
	{state_drive_init, state_drive_update_fixed, state_drive_update_fast},
	{state_measure_init, state_measure_update_fixed, state_measure_update_fast},
	{state_recover_init, state_recover_update_fixed, state_recover_update_fast},
	{state_error_init, state_error_update_fixed, state_error_update_fast},
};

static state_t current_state = STATE_IDLE;
static volatile uint8_t ms_elapsed = 0;
static uint8_t led_toggle_counter = 0;

ISR(STATEMANREF_COMPA_vect)
{
	++ms_elapsed;
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

//#define PRINT_DEBUG
static void print_debug_values()
{
	lcd_printf(0, 1, 3, 255, 255, 255, "print stuff");
}

void manager_run()
{
	for(;;)
	{
		button_update();
		
		state_procs[current_state].update_fixed_proc();
		
		if (motor_get_status() != 0x00 && current_state != STATE_ERROR)
			manager_set_state(STATE_ERROR);
		
		if (++led_toggle_counter % 10 == 0)
			led_toggle0();
			
		#ifdef PRINT_DEBUG
		print_debug_values();
		#endif
		
		// lock the fixed loop to 20 Hz
		while (ms_elapsed < 50)
		{
			// while waiting, do non-fixed-time updates
			state_procs[current_state].update_fast_proc();
		}
		
		ms_elapsed = 0;
	}
}
