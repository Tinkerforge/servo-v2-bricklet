// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_servo_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Servo Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


static TF_ServoV2 s;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_servo_v2_create(&s, UID, hal), "create device object");

	// Servo 1: Connected to port 0, period of 19.5ms, pulse width of 1 to 2ms
	//          and operating angle -100 to 100°
	check(tf_servo_v2_set_degree(&s, 0, -10000, 10000), "call set_degree");
	check(tf_servo_v2_set_pulse_width(&s, 0, 1000, 2000), "call set_pulse_width");
	check(tf_servo_v2_set_period(&s, 0, 19500), "call set_period");
	check(tf_servo_v2_set_motion_configuration(&s, 0, 500000, 1000,
	                                           1000), "call set_motion_configuration"); // Full velocity with slow ac-/deceleration


	// Servo 2: Connected to port 5, period of 20ms, pulse width of 0.95 to 1.95ms
	//          and operating angle -90 to 90°
	check(tf_servo_v2_set_degree(&s, 5, -9000, 9000), "call set_degree");
	check(tf_servo_v2_set_pulse_width(&s, 5, 950, 1950), "call set_pulse_width");
	check(tf_servo_v2_set_period(&s, 5, 20000), "call set_period");
	check(tf_servo_v2_set_motion_configuration(&s, 5, 500000, 500000,
	                                           500000), "call set_motion_configuration"); // Full velocity with full ac-/deceleration

	check(tf_servo_v2_set_position(&s, 0,
	                               10000), "call set_position"); // Set to most right position
	check(tf_servo_v2_set_enable(&s, 0, true), "call set_enable");

	check(tf_servo_v2_set_position(&s, 5,
	                               -9000), "call set_position"); // Set to most left position
	check(tf_servo_v2_set_enable(&s, 5, true), "call set_enable");
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
