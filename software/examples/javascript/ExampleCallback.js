var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Servo Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var s = new Tinkerforge.BrickletServoV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Enable position reached callback
        s.setPositionReachedCallbackConfiguration(0, true);

        // Set velocity to 100°/s. This has to be smaller or equal to the
        // maximum velocity of the servo you are using, otherwise the position
        // reached callback will be called too early
        s.setMotionConfiguration(0, 10000, 500000, 500000);
        s.setPosition(0, 9000);
        s.setEnable(0, true);
    }
);

// Register position reached callback
s.on(Tinkerforge.BrickletServoV2.CALLBACK_POSITION_REACHED,
    // Use position reached callback to swing back and forth
    function (servoChannel, position) {
        if(position === 9000) {
            console.log('Position: 90°, going to -90°');
            servo.setPosition(servoChannel, -9000);
        }
        else if(position === -9000) {
            console.log('Position: -90°, going to 90°');
            servo.setPosition(servoChannel, 9000);
        }
        else {
            console.log('Error'); // Can only happen if another program sets position
        }
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        s.setEnable(0, false);

        ipcon.disconnect();
        process.exit(0);
    }
);
