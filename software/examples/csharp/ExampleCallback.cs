using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Servo Bricklet 2.0

	// Use position reached callback to swing back and forth
	static void PositionReachedCB(BrickletServoV2 sender, int servoChannel,
	                              short position)
	{
		if(position == 9000)
		{
			Console.WriteLine("Position: 90°, going to -90°");
			sender.SetPosition(servoChannel, -9000);
		}
		else if(position == -9000)
		{
			Console.WriteLine("Position: -90°, going to 90°");
			sender.SetPosition(servoChannel, 9000);
		}
		else
		{
			// Can only happen if another program sets position
			Console.WriteLine("Error");
		}
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletServoV2 s = new BrickletServoV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register position reached callback to function PositionReachedCB
		s.PositionReachedCallback += PositionReachedCB;

		// Enable position reached callback
		s.SetPositionReachedCallbackConfiguration(0, true);

		// Set velocity to 100°/s. This has to be smaller or equal to the
		// maximum velocity of the servo you are using, otherwise the position
		// reached callback will be called too early
		s.SetMotionConfiguration(0, 10000, 500000, 500000);
		s.SetPosition(0, 9000);
		s.SetEnable(0, true);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();

		s.SetEnable(0, false);

		ipcon.Disconnect();
	}
}
