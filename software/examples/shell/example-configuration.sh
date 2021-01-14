#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Servo Bricklet 2.0

# Servo 1: Connected to port 0, period of 19.5ms, pulse width of 1 to 2ms
#          and operating angle -100 to 100°
tinkerforge call servo-v2-bricklet $uid set-degree 0 -10000 10000
tinkerforge call servo-v2-bricklet $uid set-pulse-width 0 1000 2000
tinkerforge call servo-v2-bricklet $uid set-period 0 19500
tinkerforge call servo-v2-bricklet $uid set-motion-configuration 0 500000 1000 1000 # Full velocity with slow ac-/deceleration


# Servo 2: Connected to port 5, period of 20ms, pulse width of 0.95 to 1.95ms
#          and operating angle -90 to 90°
tinkerforge call servo-v2-bricklet $uid set-degree 5 -9000 9000
tinkerforge call servo-v2-bricklet $uid set-pulse-width 5 950 1950
tinkerforge call servo-v2-bricklet $uid set-period 5 20000
tinkerforge call servo-v2-bricklet $uid set-motion-configuration 5 500000 500000 500000 # Full velocity with full ac-/deceleration

tinkerforge call servo-v2-bricklet $uid set-position 0 10000 # Set to most right position
tinkerforge call servo-v2-bricklet $uid set-enable 0 true

tinkerforge call servo-v2-bricklet $uid set-position 5 -9000 # Set to most left position
tinkerforge call servo-v2-bricklet $uid set-enable 5 true

echo "Press key to exit"; read dummy

tinkerforge call servo-v2-bricklet $uid set-enable 0 false
tinkerforge call servo-v2-bricklet $uid set-enable 5 false
