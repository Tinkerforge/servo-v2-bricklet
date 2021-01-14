#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_servo_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Servo Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
s = BrickletServoV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Use position reached callback to swing back and forth
s.register_callback(BrickletServoV2::CALLBACK_POSITION_REACHED) do |servo_channel,
                                                                    position|
  if position == 9000
    puts 'Position: 90°, going to -90°'
    s.set_position servo_channel, -9000
  elsif position == -9000
    puts 'Position: -90°, going to 90°'
    s.set_position servo_channel, 9000
  else
    puts 'Error' # Can only happen if another program sets position
  end
end

# Enable position reached callback
s.set_position_reached_callback_configuration 0, true

# Set velocity to 100°/s. This has to be smaller or equal to the
# maximum velocity of the servo you are using, otherwise the position
# reached callback will be called too early
s.set_motion_configuration 0, 10000, 500000, 500000
s.set_position 0, 9000
s.set_enable 0, true

puts 'Press key to exit'
$stdin.gets

s.set_enable 0, false

ipcon.disconnect
