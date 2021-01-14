program ExampleCallback;

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
    procedure PositionReachedCB(sender: TBrickletServoV2; const servoChannel: word;
                                const position: smallint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your Servo Bricklet 2.0 }

var
  e: TExample;

{ Use position reached callback to swing back and forth }
procedure TExample.PositionReachedCB(sender: TBrickletServoV2; const servoChannel: word;
                                     const position: smallint);
begin
  if (position = 9000) then begin
    WriteLn('Position: 90°, going to -90°');
    sender.SetPosition(servoChannel, -9000);
  end
  else if (position = -9000) then begin
    WriteLn('Position: -90°, going to 90°');
    sender.SetPosition(servoChannel, 9000);
  end
  else begin
    WriteLn('Error'); { Can only happen if another program sets position }
  end;
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  s := TBrickletServoV2.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Register position reached callback to procedure PositionReachedCB }
  s.OnPositionReached := {$ifdef FPC}@{$endif}PositionReachedCB;

  { Enable position reached callback }
  s.SetPositionReachedCallbackConfiguration(0, true);

  { Set velocity to 100°/s. This has to be smaller or equal to the
    maximum velocity of the servo you are using, otherwise the position
    reached callback will be called too early }
  s.SetMotionConfiguration(0, 10000, 500000, 500000);
  s.SetPosition(0, 9000);
  s.SetEnable(0, true);

  WriteLn('Press key to exit');
  ReadLn;

  s.SetEnable(0, false);

  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
