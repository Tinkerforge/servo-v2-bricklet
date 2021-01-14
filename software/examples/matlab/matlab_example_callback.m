function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletServoV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Servo Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    s = handle(BrickletServoV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register position reached callback to function cb_position_reached
    set(s, 'PositionReachedCallback', @(h, e) cb_position_reached(e));

    % Enable position reached callback
    s.setPositionReachedCallbackConfiguration(0, true);

    % Set velocity to 100°/s. This has to be smaller or equal to the
    % maximum velocity of the servo you are using, otherwise the position
    % reached callback will be called too early
    s.setMotionConfiguration(0, 10000, 500000, 500000);
    s.setPosition(0, 9000);
    s.setEnable(0, true);

    input('Press key to exit\n', 's');

    s.setEnable(0, false);

    ipcon.disconnect();
end

% Use position reached callback to swing back and forth
function cb_position_reached(e)
    s = e.getSource();

    if e.position == 9000
        fprintf('Position: 90°, going to -90°\n');
        s.setPosition(e.servoChannel, -9000);
    elseif e.position == -9000
        fprintf('Position: -90°, going to 90°\n');
        s.setPosition(e.servoChannel, 9000);
    else
        fprintf('Error\n'); % Can only happen if another program sets position
    end
end
