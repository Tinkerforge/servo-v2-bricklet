// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_servo_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

// Use position reached callback to swing back and forth
static void position_reached_handler(TF_ServoV2 *device, uint16_t servo_channel,
                                     int16_t position, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	if (position == 9000) {
		tf_hal_printf("Position: 90°, going to -90°\n");
		tf_servo_v2_set_position(device, servo_channel, -9000);
	} else if (position == -9000) {
		tf_hal_printf("Position: -90°, going to 90°\n");
		tf_servo_v2_set_position(device, servo_channel, 9000);
	} else {
		tf_hal_printf("Error\n"); // Can only happen if another program sets position
	}
}

static TF_ServoV2 s;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_servo_v2_create(&s, NULL, hal), "create device object");

	// Register position reached callback to function position_reached_handler
	tf_servo_v2_register_position_reached_callback(&s,
	                                               position_reached_handler,
	                                               NULL);

	// Enable position reached callback
	check(tf_servo_v2_set_position_reached_callback_configuration(&s, 0,
	                                                              true), "call set_position_reached_callback_configuration");

	// Set velocity to 100°/s. This has to be smaller or equal to the
	// maximum velocity of the servo you are using, otherwise the position
	// reached callback will be called too early
	check(tf_servo_v2_set_motion_configuration(&s, 0, 10000, 500000,
	                                           500000), "call set_motion_configuration");
	check(tf_servo_v2_set_position(&s, 0, 9000), "call set_position");
	check(tf_servo_v2_set_enable(&s, 0, true), "call set_enable");
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
