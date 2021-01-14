use std::{error::Error, io};

use tinkerforge::{ip_connection::IpConnection, servo_v2_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Servo Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let s = ServoV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Servo 1: Connected to port 0, period of 19.5ms, pulse width of 1 to 2ms
    //          and operating angle -100 to 100°
    s.set_degree(0, -10000, 10000).recv()?;
    s.set_pulse_width(0, 1000, 2000).recv()?;
    s.set_period(0, 19500).recv()?;
    s.set_motion_configuration(0, 500000, 1000, 1000).recv()?; // Full velocity with slow ac-/deceleration

    // Servo 2: Connected to port 5, period of 20ms, pulse width of 0.95 to 1.95ms
    //          and operating angle -90 to 90°
    s.set_degree(5, -9000, 9000).recv()?;
    s.set_pulse_width(5, 950, 1950).recv()?;
    s.set_period(5, 20000).recv()?;
    s.set_motion_configuration(5, 500000, 500000, 500000).recv()?; // Full velocity with full ac-/deceleration

    s.set_position(0, 10000).recv()?; // Set to most right position
    s.set_enable(0, true).recv()?;

    s.set_position(5, -9000).recv()?; // Set to most left position
    s.set_enable(5, true).recv()?;

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;

    s.set_enable(0, false).recv()?;
    s.set_enable(5, false).recv()?;

    ipcon.disconnect();
    Ok(())
}
