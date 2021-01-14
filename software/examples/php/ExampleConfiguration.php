<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletServoV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletServoV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Servo Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$s = new BrickletServoV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Servo 1: Connected to port 0, period of 19.5ms, pulse width of 1 to 2ms
//          and operating angle -100 to 100°
$s->setDegree(0, -10000, 10000);
$s->setPulseWidth(0, 1000, 2000);
$s->setPeriod(0, 19500);
$s->setMotionConfiguration(0, 500000, 1000,
                           1000); // Full velocity with slow ac-/deceleration


// Servo 2: Connected to port 5, period of 20ms, pulse width of 0.95 to 1.95ms
//          and operating angle -90 to 90°
$s->setDegree(5, -9000, 9000);
$s->setPulseWidth(5, 950, 1950);
$s->setPeriod(5, 20000);
$s->setMotionConfiguration(5, 500000, 500000,
                           500000); // Full velocity with full ac-/deceleration

$s->setPosition(0, 10000); // Set to most right position
$s->setEnable(0, TRUE);

$s->setPosition(5, -9000); // Set to most left position
$s->setEnable(5, TRUE);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));

$s->setEnable(0, FALSE);
$s->setEnable(5, FALSE);

$ipcon->disconnect();

?>
