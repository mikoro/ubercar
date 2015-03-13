#ifndef CAR_SETUP_H
#define CAR_SETUP_H

// control loop frequency
#define CONTROL_FREQ 20 // Hz
#define TIME_STEP (1.0 / CONTROL_FREQ) // seconds
#define TIME_STEP_MS (TIME_STEP * 1000.0) // milliseconds
#define ONE_OVER_TIME_STEP (1.0 / TIME_STEP) // should be same as control freq

#define TACHO_UPDATE_FREQ 4 // Hz
#define TACHO_AVG_AMOUNT 4 // taps
#define TACHO_ENABLE_STOP_DETECTION 1
#define TACHO_STOP_THRESHOLD 10 // speed

//                          right edge                  left edge
#define IRSENS_LOCATION_MAP {127, 91, 54, 10, -10, -54, -91, -127}
//#define IRSENS_LOCATION_MAP {127, 96, 64, 32, 0, -43, -86, -128}
//#define IRSENS_LOCATION_MAP {127, 84, 42, 0, -32, -64, -96, -128}
#define IRSENS_SPEED_DECREASE_MAP {6, 4, 2, 0, 0, 2, 4, 6}
#define IRSENS_ENABLE_STUCK_DETECTION 0
#define IRSENS_STUCK_DURATION 4 // seconds

////////////////////////
// Car 1 (Red leds)
////////////////////////
///*
#define MINIMUM_POWER 50
#define TARGET_SPEED 22
#define MOTOR_KP 0.3
#define MOTOR_KI 0.01
#define MOTOR_KD 0.4
#define STEERING_KP 1.0
#define STEERING_KI 0.0
#define STEERING_KD 0.0
//*/

////////////////////////
// Car 2 (Green leds)
////////////////////////

////////////////////////
// Car 3 (Yellow leds)
////////////////////////

#endif
