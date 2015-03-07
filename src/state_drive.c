#include "state_manager.h"
#include "iomap.h"
#include "lcd.h"
#include "motor.h"
#include "steering.h"
#include "tacho.h"
#include "irsens.h"

void state_drive_start()
{
	motor_set_duty_cycle2(0);
	motor_set_enabled(1);
	
	steering_set_direction2(127);
	steering_set_enabled(1);
	
	lcd_printf(0, "DRIVE");
}

void state_drive_update()
{
	// should do something
}

