{
BASS Simple Test, copyright (c) 1999-2000 Ian Luck.
======================================================
Other source: BTMain.pas; BTMain.dfm
Delphi version by Titus Miloi (titus.a.m@t-online.de)
}
program BassTest;

uses
  Forms,
  BTMain in 'BTMain.pas' {Form1};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'BASS - Simple Test';
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
