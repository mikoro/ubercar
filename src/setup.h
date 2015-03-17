#ifndef CAR_SETUP_H
#define CAR_SETUP_H

// control loop frequency
#define CONTROL_FREQ 20 // Hz
#define TIME_STEP (1.0 / CONTROL_FREQ) // seconds
#define TIME_STEP_MS (TIME_STEP * 1000.0) // milliseconds
#define ONE_OVER_TIME_STEP (1.0 / TIME_STEP) // should be same as control freq

#define TACHO_UPDATE_FREQ 4 // Hz
#define TACHO_AVG_AMOUNT 4 // taps
#define TACHO_ENABLE_STOP_DETECTION 0
#define TACHO_STOP_DETECTION_THRESHOLD 5 // speed

#define IRSENS_ENABLE_STUCK_DETECTION 0
#define IRSENS_STUCK_DETECTION_THRESHOLD 4 // seconds
#define IRSENS_START_LINE_DETECTION_THRESHOLD 3 // bits, i.e. simultaneous readings from the sensor
#define IRSENS_START_LINE_DRIVE_STRAIGHT_MS 200 // milliseconds
#define IRSENS_START_LINE_IGNORE_DURATION 2 // seconds

#define MEASURER_MAX_LAP_TIMES 10
#define MEASURER_ENABLE_PRINT_INFO 0
#define MEASURER_PRINT_INFO_INTERVAL 2 // seconds

////////////////////////
// Car 1 (Red leds)
////////////////////////
///*
#define POWER_OFFSET 50
#define STEERING_OFFSET 0
#define TARGET_SPEED 10

#define IRSENS_LOCATION_MAP_FIXED { 127, 91, 54, 10, -10, -54, -91, -127 }
#define IRSENS_LOCATION_MAP_PID { 127, 91, 54, 10, -10, -54, -91, -127 }
#define IRSENS_SPEED_DECREASE_MAP { 6, 4, 2, 0, 0, 2, 4, 6 }

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
