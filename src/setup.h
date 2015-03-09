#ifndef CAR_SETUP_H
#define CAR_SETUP_H

//                       right edge                   left edge
//#define IRSENS_MAPPING {127, 91, 54, 18, -18, -54, -91, -127}
#define IRSENS_MAPPING   {127, 96, 64, 32, 0, -43, -86, -128}
//#define IRSENS_MAPPING {127, 84, 42, 0, -32, -64, -96, -128}
		
#define ENABLE_STUCK_DETECTION 0
#define STUCK_DURATION 5 // seconds (at 20 Hz)

////////////////////////
// Car 1
////////////////////////
///*
#define MINIMUM_POWER 70
#define NORMAL_SPEED 3
#define FAST_SPEED 5
#define MOTOR_KP 1.0
#define MOTOR_KI 0.5
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
