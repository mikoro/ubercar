#include "pid.h"
#include "lcd.h"
#include "setup.h"

static fix8_t steering_kp = F8(STEERING_KP);
static fix8_t steering_ki = F8(STEERING_KI);
static fix8_t steering_kd = F8(STEERING_KD);
static fix8_t steering_integral = F8(0.0);
static fix8_t steering_previous_error = F8(0.0);
static fix8_t steering_output = F8(0.0);

static fix8_t motor_kp = F8(MOTOR_KP);
static fix8_t motor_ki = F8(MOTOR_KI);
static fix8_t motor_kd = F8(MOTOR_KD);
static fix8_t motor_integral = F8(0.0);
static fix8_t motor_previous_error = F8(0.0);
static fix8_t motor_output = F8(-128.0);

static fix8_t time_step = F8(0.05); // 20 Hz 50 ms
static fix8_t one_over_time_step = F8(20);

void pid_steering_reset()
{
	steering_integral = F8(0.0);
	steering_previous_error = F8(0.0);
	steering_output = F8(0.0);
}

void pid_motor_reset()
{
	motor_integral = F8(0.0);
	motor_previous_error = F8(0.0);
	motor_output = F8(-128.0);
}

void pid_steering_set_kp(fix8_t kp)
{
	steering_kp = kp;
}

void pid_steering_set_ki(fix8_t ki)
{
	steering_ki = ki;
}

void pid_steering_set_kd(fix8_t kd)
{
	steering_kd = kd;
}

void pid_motor_set_kp(fix8_t kp)
{
	motor_kp = kp;
}

void pid_motor_set_ki(fix8_t ki)
{
	motor_ki = ki;
}

void pid_motor_set_kd(fix8_t kd)
{
	motor_kd = kd;
}

int8_t pid_steering_calculate(int8_t ref)
{
	int16_t temp_ref = (int16_t)ref;
	int16_t temp_meas = (int16_t)fix8_to_int(steering_output);
	
	// e = r - m
	int16_t error = temp_ref - temp_meas;
	
	if (error < -128)
		error = -128;
	
	if (error > 127)
		error = 127;
	
	fix8_t clamped_error = fix8_from_int((int8_t)error);
	
	// I = I + e * dt
	steering_integral = fix8_add(steering_integral, fix8_mul(clamped_error, time_step));
	
	// D = (e - pe) / dt
	fix8_t steering_derivate = fix8_mul(fix8_sub(clamped_error, steering_previous_error), one_over_time_step);
	steering_previous_error = clamped_error;
	
	// u = Kp * e
	fix8_t control_value = fix8_mul(steering_kp, clamped_error);
	
	// u += Ki * I
	control_value = fix8_add(control_value, fix8_mul(steering_ki, steering_integral));
	
	// u += Kd * D
	control_value = fix8_add(control_value, fix8_mul(steering_kd, steering_derivate));
	
	// o += u
	steering_output = fix8_add(steering_output, control_value);
	
	return fix8_to_int(steering_output);
}

uint8_t pid_motor_calculate(uint8_t ref, uint8_t meas)
{
	int16_t temp_ref = (int16_t)ref;
	int16_t temp_meas = (int16_t)meas;
	
	// e = r - m
	int16_t error = temp_ref - temp_meas;
	
	if (error < -128)
		error = -128;
		
	if (error > 127)
		error = 127;
		
	fix8_t clamped_error = fix8_from_int((int8_t)error);
	
	// I = I + e * dt
	motor_integral = fix8_add(motor_integral, fix8_mul(clamped_error, time_step));
	
	// D = (e - pe) / dt
	fix8_t motor_derivate = fix8_mul(fix8_sub(clamped_error, motor_previous_error), one_over_time_step);
	motor_previous_error = clamped_error;
	
	// u = Kp * e
	fix8_t control_value = fix8_mul(motor_kp, clamped_error);
	
	// u += Ki * I
	control_value = fix8_add(control_value, fix8_mul(motor_ki, motor_integral));
	
	// u += Kd * D
	control_value = fix8_add(control_value, fix8_mul(motor_kd, motor_derivate));
	
	// o += u
	motor_output = fix8_add(motor_output, control_value);
	
	// shift output to 0..255 range
	int16_t output = (int16_t)fix8_to_int(motor_output);
	return (uint8_t)(output + 128);
}
