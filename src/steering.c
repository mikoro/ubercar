#include <stdlib.h>
#include <avr/io.h>

#include "steering.h"
#include "iomap.h"
#include "lcd.h"

static const fix8_t dir_to_sine_map[128] = { F8(0.002162), F8(0.005183), F8(0.008231), F8(0.011305), F8(0.014406), F8(0.017535), F8(0.020689), F8(0.023871), F8(0.027079), F8(0.030314), F8(0.033576), F8(0.036864), F8(0.040179), F8(0.043520), F8(0.046888), F8(0.050282), F8(0.053702), F8(0.057149), F8(0.060622), F8(0.064121), F8(0.067646), F8(0.071197), F8(0.074774), F8(0.078377), F8(0.082006), F8(0.085661), F8(0.089341), F8(0.093047), F8(0.096779), F8(0.100536), F8(0.104318), F8(0.108126), F8(0.111959), F8(0.115817), F8(0.119700), F8(0.123607), F8(0.127540), F8(0.131498), F8(0.135480), F8(0.139486), F8(0.143517), F8(0.147572), F8(0.151652), F8(0.155755), F8(0.159883), F8(0.164034), F8(0.168209), F8(0.172407), F8(0.176629), F8(0.180874), F8(0.185142), F8(0.189433), F8(0.193747), F8(0.198084), F8(0.202443), F8(0.206824), F8(0.211228), F8(0.215654), F8(0.220101), F8(0.224571), F8(0.229061), F8(0.233574), F8(0.238107), F8(0.242661), F8(0.247236), F8(0.251832), F8(0.256448), F8(0.261084), F8(0.265740), F8(0.270416), F8(0.275112), F8(0.279827), F8(0.284561), F8(0.289313), F8(0.294085), F8(0.298875), F8(0.303683), F8(0.308509), F8(0.313352), F8(0.318213), F8(0.323091), F8(0.327986), F8(0.332898), F8(0.337826), F8(0.342770), F8(0.347730), F8(0.352706), F8(0.357696), F8(0.362702), F8(0.367722), F8(0.372757), F8(0.377805), F8(0.382867), F8(0.387943), F8(0.393031), F8(0.398133), F8(0.403247), F8(0.408372), F8(0.413510), F8(0.418659), F8(0.423818), F8(0.428989), F8(0.434170), F8(0.439360), F8(0.444560), F8(0.449770), F8(0.454988), F8(0.460214), F8(0.465448), F8(0.470690), F8(0.475939), F8(0.481195), F8(0.486457), F8(0.491725), F8(0.496998), F8(0.502277), F8(0.507560), F8(0.512847), F8(0.518138), F8(0.523432), F8(0.528728), F8(0.534027), F8(0.539328), F8(0.544630), F8(0.549933), F8(0.555237), F8(0.560540), F8(0.565842) };

/* Steering servo:
 *
 * Full right = 45000
 * Neutral    = 40000
 * Full left  = 35000
 *
 * Values outside range will cause bad noises
 */
 
void steering_init(void) {
	// Enable output pin
	STEER_DDR |= STEER0;
	STEER_PORT &= ~STEER0;

	// Fast PWM, divide clock by 1
	STEER_TCCRA = BIT(WGM11);
	STEER_TCCRB = BIT(WGM12) | BIT(WGM13) | BIT(CS10);

	// PWM period 4ms (250 Hz), set TOP accordingly
	STEER_ICR = 64000;
	STEER_OCRA = 40000; // neutral
}

void steering_set_enabled(uint8_t en) {
	if (en) {
		STEER_TCCRA |= BIT(COM1A1) | BIT(COM1A0); // PWM on (inverted, i.e normally low)
	} else {
		STEER_TCCRA &= ~(BIT(COM1A1) | BIT(COM1A0)); // PWM disabled
	}
}

// Input in range -127 .. 127, 0 is neutral, -127 is left
void steering_set_direction(int8_t dir) {
	// exact factor is 39.216 (do not change, steering angle sines have been calculated with this)
	STEER_OCRA = 40000 + 39 * dir;
}

fix8_t steering_get_sine(int8_t dir)
{
	uint8_t index = abs(dir);
	
	if (index > 127)
		index = 127;
		
	return dir_to_sine_map[index];
}
