#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletServoV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Servo Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
our $s = Tinkerforge::BrickletServoV2->new(&UID, $ipcon); # Create device object

# Use position reached callback to swing back and forth
sub cb_position_reached
{
    my ($servo_channel, $position) = @_;

    if ($position == 9000)
    {
        print "Position: 90°, going to -90°\n";
        $s->set_position($servo_channel, -9000);
    }
    elsif ($position == -9000)
    {
        print "Position: -90°, going to 90°\n";
        $s->set_position($servo_channel, 9000);
    }
    else
    {
        print "Error\n"; # Can only happen if another program sets position
    }
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register position reached callback to subroutine cb_position_reached
$s->register_callback($s->CALLBACK_POSITION_REACHED, 'cb_position_reached');

# Enable position reached callback
$s->set_position_reached_callback_configuration(0, 1);

# Set velocity to 100°/s. This has to be smaller or equal to the
# maximum velocity of the servo you are using, otherwise the position
# reached callback will be called too early
$s->set_motion_configuration(0, 10000, 500000, 500000);
$s->set_position(0, 9000);
$s->set_enable(0, 1);

print "Press key to exit\n";
<STDIN>;

$s->set_enable(0, 0);

$ipcon->disconnect();
