#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "ABC"

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_servo_v2 import BrickletServoV2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    s = BrickletServoV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd

    s.set_servo_current_configuration(0b1111111111 | (1 << 15), 255)
    s.set_input_voltage_configuration(255)

    for ch in range(10):
        print(ch, s.get_servo_current_configuration(ch))
    print(s.get_input_voltage_configuration())

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
