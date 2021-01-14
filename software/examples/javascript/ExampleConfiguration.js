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
        // Servo 1: Connected to port 0, period of 19.5ms, pulse width of 1 to 2ms
        //          and operating angle -100 to 100°
        s.setDegree(0, -10000, 10000);
        s.setPulseWidth(0, 1000, 2000);
        s.setPeriod(0, 19500);
        s.setMotionConfiguration(0, 500000, 1000,
                                 1000); // Full velocity with slow ac-/deceleration


        // Servo 2: Connected to port 5, period of 20ms, pulse width of 0.95 to 1.95ms
        //          and operating angle -90 to 90°
        s.setDegree(5, -9000, 9000);
        s.setPulseWidth(5, 950, 1950);
        s.setPeriod(5, 20000);
        s.setMotionConfiguration(5, 500000, 500000,
                                 500000); // Full velocity with full ac-/deceleration

        s.setPosition(0, 10000); // Set to most right position
        s.setEnable(0, true);

        s.setPosition(5, -9000); // Set to most left position
        s.setEnable(5, true);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        s.setEnable(0, false);
        s.setEnable(5, false);

        ipcon.disconnect();
        process.exit(0);
    }
);
