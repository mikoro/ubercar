#include "controller.h"
#include "lcd.h"
#include "tacho.h"
#include "steering.h"
#include "motor.h"
#include "irsens.h"
#include "setup.h"

static fix8_t steering_kp = F8(STEERING_KP);
static fix8_t steering_ki = F8(STEERING_KI);
static fix8_t steering_kd = F8(STEERING_KD);
static fix8_t steering_integral = F8(0.0);
static fix8_t steering_previous_error = F8(0.0);
static fix8_t steering_direction_value = F8(0.0);
static fix8_t steering_x = F8(0.0);

static fix8_t motor_kp = F8(MOTOR_KP);
static fix8_t motor_ki = F8(MOTOR_KI);
static fix8_t motor_kd = F8(MOTOR_KD);
static fix8_t motor_integral = F8(0.0);
static fix8_t motor_previous_error = F8(0.0);
static fix8_t motor_power_value = F8(-128.0);
static uint8_t motor_target_speed = 0;

void controller_steering_reset()
{
	steering_integral = F8(0.0);
	steering_previous_error = F8(0.0);
	steering_direction_value = F8(0.0);
	steering_x = F8(0.0);
}

void controller_motor_reset()
{
	motor_integral = F8(0.0);
	motor_previous_error = F8(0.0);
	motor_power_value = F8(-128.0);
	motor_target_speed = 0;
}

void controller_motor_set_target_speed(uint8_t speed)
{
	motor_target_speed = speed;
}

void controller_steering_update_pid()
{
	int8_t steering_direction = fix8_to_int(steering_direction_value);
	
	// vy
	fix8_t velocity_y = fix8_from_int((int8_t)tacho_get_speed());
	
	// sin(steering_angle)
	// it is actually the sine of the real physical turning angle of the wheels
	fix8_t direction_sine = steering_get_sine(steering_direction);
	
	// vx = vy * sin(steering_angle)
	fix8_t velocity_x = fix8_mul(velocity_y, direction_sine);
	
	// vx = vx * dt
	//velocity_x = fix8_mul(velocity_x, F8(TIME_STEP));
	velocity_x = fix8_mul(velocity_x, F8(4.0));
	
	if (steering_direction > 0)
		velocity_x = fix8_mul(velocity_x, F8(-1));
	
	// x = x + vx
	steering_x = fix8_add(steering_x, velocity_x);
		
	// if we have active sensor reading, use that instead
	//if (irsens_is_sensing())
		//steering_x = fix8_from_int(irsens_get_location_pid());
	
	// always try to steer back to center
	// e = r - m
	fix8_t clamped_error = fix8_sub(F8(0), steering_x);
	
	// I = I + e * dt
	steering_integral = fix8_add(steering_integral, fix8_mul(clamped_error, F8(TIME_STEP)));
	
	// D = (e - pe) / dt
	fix8_t steering_derivate = fix8_mul(fix8_sub(clamped_error, steering_previous_error), F8(ONE_OVER_TIME_STEP));
	steering_previous_error = clamped_error;
	
	// u = Kp * e
	fix8_t control_value = fix8_mul(steering_kp, clamped_error);
	
	// u += Ki * I
	control_value = fix8_add(control_value, fix8_mul(steering_ki, steering_integral));
	
	// u += Kd * D
	control_value = fix8_add(control_value, fix8_mul(steering_kd, steering_derivate));
	
	// o += u
	steering_direction_value = fix8_sub(steering_direction_value, control_value);
	
	steering_set_direction(fix8_to_int(steering_direction_value));
}

void controller_steering_update_fixed()
{
	steering_set_direction(irsens_get_location_fixed());
}

void controller_motor_update_pid()
{
	// e = r - m
	int16_t error = (int16_t)motor_target_speed - (int16_t)tacho_get_speed();
	
	if (error < -128)
		error = -128;
	
	if (error > 127)
		error = 127;
	
	fix8_t clamped_error = fix8_from_int((int8_t)error);
	
	// I = I + e * dt
	motor_integral = fix8_add(motor_integral, fix8_mul(clamped_error, F8(TIME_STEP)));
	
	// D = (e - pe) / dt
	fix8_t motor_derivate = fix8_mul(fix8_sub(clamped_error, motor_previous_error), F8(ONE_OVER_TIME_STEP));
	motor_previous_error = clamped_error;
	
	// u = Kp * e
	fix8_t control_value = fix8_mul(motor_kp, clamped_error);
	
	// u += Ki * I
	control_value = fix8_add(control_value, fix8_mul(motor_ki, motor_integral));
	
	// u += Kd * D
	control_value = fix8_add(control_value, fix8_mul(motor_kd, motor_derivate));
	
	// o += u
	motor_power_value = fix8_add(motor_power_value, control_value);
	
	// shift to 0..255 range
	uint8_t motor_power_value_shifted = (uint8_t)((int16_t)fix8_to_int(motor_power_value) + 128);
	
	motor_set_power(motor_power_value_shifted);
}
