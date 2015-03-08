#ifndef CAR_PID_H
#define CAR_PID_H

#include <stdint.h>

#include "fixed/fix8.h"

void pid_steering_reset();
void pid_motor_reset();

void pid_steering_set_kp(fix8_t kp);
void pid_steering_set_ki(fix8_t ki);
void pid_steering_set_kd(fix8_t kd);

// ref: wanted steering angle, -127 .. 127
// meas: location value from ir sensor, -127 .. 127
// return: angle to which steering should be set, -127 .. 127
int8_t pid_steering_calculate(int8_t ref, int8_t meas);

void pid_motor_set_kp(fix8_t kp);
void pid_motor_set_ki(fix8_t ki);
void pid_motor_set_kd(fix8_t kd);

// ref: wanted speed (in tacho units), 0 .. 255
// meas: speed value from tacho, 0 .. 255
// return: motor power value, 0 .. 255
uint8_t pid_motor_calculate(uint8_t ref, uint8_t meas);

#endif
