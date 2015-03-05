#ifndef CAR_MOTOR_H
#define CAR_MOTOR_H

void init_motor(void);
void motor_set_enabled(uint8_t en);
void motor_set_duty_cycle(uint16_t dc);
void motor_set_duty_cycle2(uint8_t dc);

#endif
