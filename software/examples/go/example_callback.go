package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/servo_v2_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Servo Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	s, _ := servo_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	s.RegisterPositionReachedCallback(func(servoChannel uint16, position int16) {
		if position == 9000 {
			fmt.Println("Position: 90°, going to -90°")
			s.SetPosition(servoChannel, -9000)
		} else if position == -9000 {
			fmt.Println("Position: -90°, going to 90°")
			s.SetPosition(servoChannel, 9000)
		} else {
			// Can only happen if another program sets position
			fmt.Println("Error")
		}
	})

	// Enable position reached callback
	s.SetPositionReachedCallbackConfiguration(0, true)

	// Set velocity to 100°/s. This has to be smaller or equal to the
	// maximum velocity of the servo you are using, otherwise the position
	// reached callback will be called too early
	s.SetMotionConfiguration(0, 10000, 500000, 500000)
	s.SetPosition(0, 9000)
	s.SetEnable(0, true)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

	s.SetEnable(0, false)
}
