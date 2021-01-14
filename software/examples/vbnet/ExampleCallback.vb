Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Servo Bricklet 2.0

    ' Use position reached callback to swing back and forth
    Sub PositionReachedCB(ByVal sender As BrickletServoV2, _
                          ByVal servoChannel As Integer, ByVal position As Short)
        If position = 9000 Then
            Console.WriteLine("Position: 90°, going to -90°")
            sender.SetPosition(servoChannel, -9000)
        Else If position = -9000 Then
            Console.WriteLine("Position: -90°, going to 90°")
            sender.SetPosition(servoChannel, 9000)
        Else
            ' Can only happen if another program sets position
            Console.WriteLine("Error")
        End If
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim s As New BrickletServoV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register position reached callback to subroutine PositionReachedCB
        AddHandler s.PositionReachedCallback, AddressOf PositionReachedCB

        ' Enable position reached callback
        s.SetPositionReachedCallbackConfiguration(0, True)

        ' Set velocity to 100°/s. This has to be smaller or equal to the
        ' maximum velocity of the servo you are using, otherwise the position
        ' reached callback will be called too early
        s.SetMotionConfiguration(0, 10000, 500000, 500000)
        s.SetPosition(0, 9000)
        s.SetEnable(0, True)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()

        s.SetEnable(0, False)

        ipcon.Disconnect()
    End Sub
End Module
