#ifndef CAR_CONTROLLER_H
#define CAR_CONTROLLER_H

#include <stdint.h>

#include "fix8.h"

void controller_steering_reset();
void controller_motor_reset();

void controller_motor_set_target_speed(uint8_t speed);

void controller_steering_update_pid();
void controller_steering_update_fixed();
void controller_motor_update_pid();

#endif
