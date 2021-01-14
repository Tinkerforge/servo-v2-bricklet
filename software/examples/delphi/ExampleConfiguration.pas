program ExampleConfiguration;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletServoV2;

type
  TExample = class
  private
    ipcon: TIPConnection;
    s: TBrickletServoV2;
  public
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your Servo Bricklet 2.0 }

var
  e: TExample;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  s := TBrickletServoV2.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Servo 1: Connected to port 0, period of 19.5ms, pulse width of 1 to 2ms
             and operating angle -100 to 100° }
  s.SetDegree(0, -10000, 10000);
  s.SetPulseWidth(0, 1000, 2000);
  s.SetPeriod(0, 19500);
  s.SetMotionConfiguration(0, 500000, 1000,
                           1000); { Full velocity with slow ac-/deceleration }


  { Servo 2: Connected to port 5, period of 20ms, pulse width of 0.95 to 1.95ms
             and operating angle -90 to 90° }
  s.SetDegree(5, -9000, 9000);
  s.SetPulseWidth(5, 950, 1950);
  s.SetPeriod(5, 20000);
  s.SetMotionConfiguration(5, 500000, 500000,
                           500000); { Full velocity with full ac-/deceleration }

  s.SetPosition(0, 10000); { Set to most right position }
  s.SetEnable(0, true);

  s.SetPosition(5, -9000); { Set to most left position }
  s.SetEnable(5, true);

  WriteLn('Press key to exit');
  ReadLn;

  s.SetEnable(0, false);
  s.SetEnable(5, false);

  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
