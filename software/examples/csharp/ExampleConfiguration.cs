using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Servo Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletServoV2 s = new BrickletServoV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Servo 1: Connected to port 0, period of 19.5ms, pulse width of 1 to 2ms
		//          and operating angle -100 to 100°
		s.SetDegree(0, -10000, 10000);
		s.SetPulseWidth(0, 1000, 2000);
		s.SetPeriod(0, 19500);
		s.SetMotionConfiguration(0, 500000, 1000,
		                         1000); // Full velocity with slow ac-/deceleration


		// Servo 2: Connected to port 5, period of 20ms, pulse width of 0.95 to 1.95ms
		//          and operating angle -90 to 90°
		s.SetDegree(5, -9000, 9000);
		s.SetPulseWidth(5, 950, 1950);
		s.SetPeriod(5, 20000);
		s.SetMotionConfiguration(5, 500000, 500000,
		                         500000); // Full velocity with full ac-/deceleration

		s.SetPosition(0, 10000); // Set to most right position
		s.SetEnable(0, true);

		s.SetPosition(5, -9000); // Set to most left position
		s.SetEnable(5, true);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();

		s.SetEnable(0, false);
		s.SetEnable(5, false);

		ipcon.Disconnect();
	}
}
