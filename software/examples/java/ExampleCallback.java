import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletServoV2;
import com.tinkerforge.TinkerforgeException;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Servo Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		// Note: Declare s as final, so the listener can access it
		final BrickletServoV2 s = new BrickletServoV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Use position reached callback to swing back and forth
		s.addPositionReachedListener(new BrickletServoV2.PositionReachedListener() {
			public void positionReached(int servoChannel, int position) {
				if (position == 9000) {
					System.out.println("Position: 90°, going to -90°");
					try {
						s.setPosition(servoChannel, -9000);
					} catch(TinkerforgeException e) {
					}
				} else if (position == -9000) {
					System.out.println("Position: -90°, going to 90°");
					try {
						s.setPosition(servoChannel, 9000);
					} catch(TinkerforgeException e) {
					}
				} else {
					// Can only happen if another program sets velocity
					System.out.println("Error");
				}
			}
		});

		// Enable position reached callback
		s.setPositionReachedCallbackConfiguration(0, true);

		// Set velocity to 100°/s. This has to be smaller or equal to the
		// maximum velocity of the servo you are using, otherwise the position
		// reached callback will be called too early
		s.setMotionConfiguration(0, 10000, 500000, 500000);
		s.setPosition(0, 9000);
		s.setEnable(0, true);

		System.out.println("Press key to exit"); System.in.read();

		s.setEnable(0, false);

		ipcon.disconnect();
	}
}
