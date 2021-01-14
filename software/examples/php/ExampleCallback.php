<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletServoV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletServoV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Servo Bricklet 2.0

// Use position reached callback to swing back and forth
function cb_positionReached($servo_channel, $positio, $user_data)
{
    $s = $user_data;

    if ($position == 9000) {
        echo "Position: 90°, going to -90°\n";
        $s->setPosition($servo_channel, -9000);
    } elseif ($position == -9000) {
        echo "Position: -90°, going to 90°\n";
        $s->setPosition($servo_channel, 9000);
    } else {
        echo "Error\n"; // Can only happen if another program sets position
    }
}

$ipcon = new IPConnection(); // Create IP connection
$s = new BrickletServoV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register position reached callback to function cb_positionReached
$s->registerCallback(BrickletServoV2::CALLBACK_POSITION_REACHED, 'cb_positionReached', $s);

// Enable position reached callback
$s->setPositionReachedCallbackConfiguration(0, TRUE);

// Set velocity to 100°/s. This has to be smaller or equal to the
// maximum velocity of the servo you are using, otherwise the position
// reached callback will be called too early
$s->setMotionConfiguration(0, 10000, 500000, 500000);
$s->setPosition(0, 9000);
$s->setEnable(0, TRUE);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
