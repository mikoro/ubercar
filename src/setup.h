#ifndef CAR_SETUP_H
#define CAR_SETUP_H

// control loop frequency 20 Hz
#define TIME_STEP 0.05 // seconds
#define TIME_STEP_MS 50 // milliseconds
#define ONE_OVER_TIME_STEP 20 // Hz

#define TACHO_UPDATE_FREQ 20 // Hz
#define TACHO_AVG_AMOUNT 5

//                          right edge                  left edge
#define IRSENS_LOCATION_MAP {127, 91, 54, 18, -18, -54, -91, -127}
//#define IRSENS_LOCATION_MAP {127, 96, 64, 32, 0, -43, -86, -128}
//#define IRSENS_LOCATION_MAP {127, 84, 42, 0, -32, -64, -96, -128}
#define IRSENS_RELATIVE_LOCATION_MAP {3, 2, 1, 0, 0, 1, 2, 3}

#define ENABLE_STUCK_DETECTION 0
#define STUCK_DURATION 4 // seconds

////////////////////////
// Car 1
////////////////////////
///*
#define MINIMUM_POWER 80
#define NORMAL_SPEED 20
#define FAST_SPEED 5
#define MOTOR_KP 0.5
#define MOTOR_KI 0.0
#define MOTOR_KD 0.0
#define STEERING_KP 1.0
#define STEERING_KI 0.0
#define STEERING_KD 0.0
//*/

////////////////////////
// Car 2
////////////////////////

////////////////////////
// Car 3
////////////////////////

#endif
