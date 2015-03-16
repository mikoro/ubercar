#include "measurer.h"
#include "manager.h"
#include "irsens.h"
#include "tacho.h"
#include "lcd.h"
#include "setup.h"

static uint8_t lap_count = 0;
static uint16_t lap_times[MEASURER_MAX_LAP_TIMES] = {0};
static uint8_t lap_time_index = 0;
static uint8_t update_count = 0;
static uint8_t is_pre_lap = 1;

void measurer_reset()
{
	lap_count = 0;
	
	for (uint8_t i = 0; i < MEASURER_MAX_LAP_TIMES; ++i)
		lap_times[i] = 0;
	
	lap_time_index = 0;
	update_count = 0;
	is_pre_lap = 1;
}

void measurer_update()
{
	if (MEASURER_ENABLE_PRINT_INFO && (++update_count > (MEASURER_PRINT_INFO_INTERVAL * CONTROL_FREQ)))
	{
		measurer_print_info();
		update_count = 0;
	}
	
	if (irsens_has_crossed_start_line())
	{
		irsens_reset_has_crossed_start_line();
		
		if (!is_pre_lap)
		{
			++lap_count;
			lap_times[++lap_time_index % MEASURER_MAX_LAP_TIMES] = manager_get_timer_elapsed_ms();
		}
		else
			is_pre_lap = 0;
			
		manager_reset_timer();
	}
}

void measurer_print_info()
{
	uint16_t best_lap_time = UINT16_MAX;
	
	for (uint8_t i = 0; i < MEASURER_MAX_LAP_TIMES; ++i)
	{
		if (lap_times[i] < best_lap_time && lap_times[i] != 0)
			best_lap_time = lap_times[i];
	}
	
	uint16_t best_lap_time_s = best_lap_time / 1000;
	uint16_t best_lap_time_ms = best_lap_time % 1000;
	
	lcd_printf(0, 9, 3, 31, 63, 31, "Total laps: %u      ", lap_count);
	lcd_printf(0, 10, 3, 31, 63, 31, "Total dist: %u m    ", tacho_get_distance_m());
	lcd_printf(0, 11, 3, 31, 63, 31, "Best time:  %u.%03u s    ", best_lap_time_s, best_lap_time_ms);
}

uint8_t measurer_get_lap_count()
{
	return lap_count;
}
