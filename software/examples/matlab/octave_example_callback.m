function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Servo Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    s = javaObject("com.tinkerforge.BrickletServoV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register position reached callback to function cb_position_reached
    s.addPositionReachedCallback(@cb_position_reached);

    % Enable position reached callback
    s.setPositionReachedCallbackConfiguration(0, true);

    % Set velocity to 100°/s. This has to be smaller or equal to the
    % maximum velocity of the servo you are using, otherwise the position
    % reached callback will be called too early
    s.setMotionConfiguration(0, 10000, 500000, 500000);
    s.setPosition(0, 9000);
    s.setEnable(0, true);

    input("Press key to exit\n", "s");

    s.setEnable(0, false);

    ipcon.disconnect();
end

% Use position reached callback to swing back and forth
function cb_position_reached(e)
    s = e.getSource();
    position = java2int(e.position);

    if position == 9000
        fprintf("Position: 90°, going to -90°\n");
        s.setPosition(java2int(e.servoChannel), -9000);
    elseif position == -9000
        fprintf("Position: -90°, going to 90°\n");
        s.setPosition(java2int(e.servoChannel), 9000);
    else
        fprintf("Error\n"); % Can only happen if another program sets position
    end
end

function int = java2int(value)
    if compare_versions(version(), "3.8", "<=")
        int = value.intValue();
    else
        int = value;
    end
end
