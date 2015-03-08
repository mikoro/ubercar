#ifndef CAR_MOTOR_H
#define CAR_MOTOR_H

#include <stdint.h>

typedef enum { MOTOR_FORWARDS, MOTOR_BACKWARDS } motor_dir_t;

void motor_init(void);
void motor_set_enabled(uint8_t en);
void motor_set_direction(motor_dir_t dir);
void motor_set_power(uint8_t power);
uint8_t motor_get_status();
uint8_t motor_get_current();

#endif
