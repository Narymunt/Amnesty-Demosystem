(*
BASS Simple Console Test, copyright (c) 1999-2001 Ian Luck.
TMT Pascal version by Vadim Bodrov, TMT Development Corp. (support@tmt.com)
*)

uses Strings, Windows, MMSystem, BASS, Keyboard;

var
  starttime: DWORD;

function IntToFixed(val, digits: Longint): string;
var
  s: string;
begin
  s := IntToStr(val);
  while Length(s) < digits do s := '0' + s;
  Result := s;
end;

// display error messages
procedure Error(text: string);
begin
  WriteLn('Error(' + IntToStr(BASS_ErrorGetCode) + '): ' + text);
  BASS_Free;
  Halt(0);
end;

// looping synchronizer, resets the clock
procedure LoopSync(handle: HSYNC; channel, data, user: DWORD); stdcall;
begin
  starttime := timeGetTime;
end;

var
  mus: HMUSIC;
  strm: HSTREAM;
  vol: DWORD;
  time, pos, level: DWORD;
  a, pan: Longint;
  freq: DWORD;
  mono: Boolean;
  tmp: array[0..MAX_PATH] of char;

  function StrMus: DWORD;
  begin
    if strm <> 0 then
      Result := strm
    else
      Result := mus;
  end;

begin
  WriteLn('Simple console mode BASS example : MOD/MP3/WAV player');
  Writeln('-----------------------------------------------------');

  // check that BASS 1.2 was loaded
  if (BASS_GetVersion <> MAKELONG(1, 2)) then
  begin
    Writeln('BASS version 1.2 was not loaded');
    Exit;
  end;
  if (ParamCount <> 1) then
  begin
    WriteLn(#9 + 'usage: contest <file>');
    Exit;
  end;

  // setup output - default device, 44100hz, stereo, 16 bits
  if BASS_Init(-1, 44100, 0, 0) = LONGFALSE then
    Error('Can''t initialize device');

  // try streaming the file
  strm := BASS_StreamCreateFile(LONGFALSE, StrPCopy(tmp, ParamStr(1)), 0, 0, 0);
  if strm = 0 then
    strm := BASS_StreamCreateURL(StrPCopy(tmp, ParamStr(1)), BASS_STREAM_RESTRATE, nil);
  if strm <> 0 then
  begin
    // check if the stream is mono (for the level indicator)
    mono := (BASS_ChannelGetFlags(strm) and BASS_SAMPLE_MONO) > 0;
    // set a synchronizer for when the stream reaches the end
    BASS_ChannelSetSync(strm, BASS_SYNC_END, 0, @LoopSync, 0);
    if BASS_StreamGetFilePosition(strm, 1) <> $FFFFFFFF then
      // streaming from the internet
      Writeln('streaming internet file [', BASS_StreamGetLength(strm), ' bytes]')
    else
      Writeln('streaming file [', BASS_StreamGetLength(strm), ' bytes]');
  end else
  begin
    // try loading the MOD (with looping, sensitive ramping and surround sound)
    mus := BASS_MusicLoad(LONGFALSE, StrPCopy(tmp, ParamStr(1)), 0, 0, BASS_MUSIC_LOOP or BASS_MUSIC_RAMPS or BASS_MUSIC_SURROUND);
    if mus = 0 then
      // not a MOD either
      Error('Can''t play the file');
    // set a synchronizer for when the MOD reaches the end
    BASS_ChannelSetSync(mus, BASS_SYNC_END, 0, @LoopSync, 0);
    Writeln('playing MOD music ', BASS_MusicGetName(mus), ' [', + BASS_MusicGetLength(mus,FALSE), ' orders]');
  end;

  BASS_Start;
  if strm <> 0 then
    BASS_StreamPlay(strm, LONGFALSE, BASS_SAMPLE_LOOP)
  else
    BASS_MusicPlay(mus);
  starttime := timeGetTime;

  while (not KbHit) and (BASS_ChannelIsActive(StrMus) <> 0) do
  begin
    // display some stuff and wait a bit
    time := timeGetTime - starttime;
    level := BASS_ChannelGetLevel(StrMus);
    pos := BASS_ChannelGetPosition(StrMus);

    if (strm <> 0) then
      Write('pos ' + IntToFixed(pos, 9) + ' - time ' + IntToStr(time div 60000) + ':' + IntToFixed((time div 1000) mod 60, 2) + ' - L ')
    else
      Write('pos ' + IntToFixed(LOWORD(pos), 3) + ':' + IntToFixed(HIWORD(pos), 3) + ' - time ' + IntToStr(time div 60000) + ':' + IntToFixed((time div 1000) mod 60, 2) + ' - L ');

    if BASS_ChannelIsActive(StrMus) = 2 then
      // internet file stream playback has stalled
      Write('-- buffering : ', BASS_StreamGetFilePosition(strm, 1) - BASS_StreamGetFilePosition(strm, 0), ' --')
    else
    begin
      a := 93;
      while a > 0 do
      begin
        if LOWORD(level) >= a then
          Write('*')
        else
          Write('-');
        a := a * 2 div 3;
      end;

      Write(' ');
      if mono then
      begin
        a := 1;
        while (a < 128) do
        begin
          if LOWORD(level) >= a then
            Write('*')
          else
            Write('-');
          a +:= a - (a shr 1);
        end;
      end else
      begin
        a := 1;
        while (a < 128) do
        begin
          if HIWORD(level) >= a then
            Write('*')
          else
            Write('-');
          a +:= a - (a shr 1);
        end;
      end;
    end;
    Write(' R - cpu ' + Fix(BASS_GetCPU, 2, 1) + '%  ' + #13);
    Sleep(50);
  end;

  // get the frequency... and wind it down
  BASS_ChannelGetAttributes(StrMus, freq, vol, pan);
  level := freq div 40;
  while (freq > 2000) do
  begin
    BASS_ChannelSetAttributes(StrMus, freq, -1, -101);
    freq -:= level;
    Sleep(5);
  end;

  // fade-out to avoid a "click"
  for a := 20 downto 0 do
  begin
    BASS_SetVolume(a * 5);
    Sleep(1);
  end;

  BASS_Free;
  Sleep(500);
end.
