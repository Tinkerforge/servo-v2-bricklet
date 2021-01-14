#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Servo Bricklet 2.0

# Use position reached callback to swing back and forth
tinkerforge dispatch servo-v2-bricklet $uid position-reached\
 --execute "if   [ {position} -eq  9000 ];
            then echo 'Position: 90°, going to -90°' && tinkerforge call servo-v2-bricklet $uid set-position 0 -9000;
            elif [ {position} -eq -9000 ];
            then echo 'Position: -90°, going to 90°' && tinkerforge call servo-v2-bricklet $uid set-position 0 9000;
            else echo error; fi" &

# Enable position reached callback
tinkerforge call servo-v2-bricklet $uid set-position-reached-callback-configuration 0 true

# Set velocity to 100°/s. This has to be smaller or equal to the
# maximum velocity of the servo you are using, otherwise the position
# reached callback will be called too early
tinkerforge call servo-v2-bricklet $uid set-motion-configuration 0 10000 500000 500000
tinkerforge call servo-v2-bricklet $uid set-position 0 9000
tinkerforge call servo-v2-bricklet $uid set-enable 0 true

echo "Press key to exit"; read dummy

tinkerforge call servo-v2-bricklet $uid set-enable 0 false

kill -- -$$ # Stop callback dispatch in background
