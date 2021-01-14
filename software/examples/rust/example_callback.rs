use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, servo_v2_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Servo Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let s = ServoV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let position_reached_receiver = s.get_position_reached_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `s` object
    // is dropped, so there is no need for manual cleanup.
    let s_copy = s.clone(); // Device objects don't implement Sync, so they can't be shared 
                            // between threads (by reference). So clone the device and move the copy.
    thread::spawn(move || {
        for position_reached in position_reached_receiver {
            if position_reached.position == 9000 {
                println!("Position: 90°, going to -90°");
                s_copy.set_position(position_reached.servo_channel, -9000).recv()?;
            } else if position_reached.position == -9000 {
                println!("Position: -90°, going to 90°");
                s_copy.set_position(position_reached.servo_channel, 9000).recv()?;
            } else {
                // Can only happen if another program sets position
                println!("Error");
            }
        }
    });

    // Enable position reached callback
    s.set_position_reached_callback_configuration(0, true).recv()?;

    // Set velocity to 100°/s. This has to be smaller or equal to the
    // maximum velocity of the servo you are using, otherwise the position
    // reached callback will be called too early
    s.set_motion_configuration(0, 10000, 500000, 500000).recv()?;
    s.set_position(0, 9000).recv()?;
    s.set_enable(0, true).recv()?;

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;

    s.set_enable(0, false).recv()?;

    ipcon.disconnect();
    Ok(())
}
