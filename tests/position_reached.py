#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "ABC"

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_servo_v2 import BrickletServoV2

def cb_position_reached(servo_num, position):
    print(servo_num, position)

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    s = BrickletServoV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd

    s.register_callback(s.CALLBACK_POSITION_REACHED, cb_position_reached)
    print(s.get_position_reached_callback_configuration(0))
    s.set_position_reached_callback_configuration(0b1111111111 | (1 << 15), True)
    print(s.get_position_reached_callback_configuration(0))

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
