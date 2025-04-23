{
  BASS 1.2 Multimedia Library
  -----------------------------
  (c) 1999-2001 Ian Luck.
  Please report bugs/suggestions/etc... to bass@un4seen.com
  TMT Pascal Version by Vadim Bodrov, TMT Development Corporation

  See the BASS.CHM file for more complete documentation
}
unit Bass;

{$oa+}

interface

{$ifndef __MULTITARGET__}
  This unit can be compiled with TMT Pascal Multi-target only
{$endif}

{$ifndef __WIN32__}
  Invalid compilation target
{$endif}

uses
  Windows;

const
  // Error codes returned by BASS_GetErrorCode()
  BASS_OK                 = 0;    // all is OK
  BASS_ERROR_MEM          = 1;    // memory error
  BASS_ERROR_FILEOPEN     = 2;    // can't open the file
  BASS_ERROR_DRIVER       = 3;    // can't find a free sound driver
  BASS_ERROR_BUFLOST      = 4;    // the sample buffer was lost - please report this!
  BASS_ERROR_HANDLE       = 5;    // invalid handle
  BASS_ERROR_FORMAT       = 6;    // unsupported format
  BASS_ERROR_POSITION     = 7;    // invalid playback position
  BASS_ERROR_INIT         = 8;    // BASS_Init has not been successfully called
  BASS_ERROR_START        = 9;    // BASS_Start has not been successfully called
  BASS_ERROR_INITCD       = 10;   // can't initialize CD
  BASS_ERROR_CDINIT       = 11;   // BASS_CDInit has not been successfully called
  BASS_ERROR_NOCD         = 12;   // no CD in drive
  BASS_ERROR_CDTRACK      = 13;   // can't play the selected CD track
  BASS_ERROR_ALREADY      = 14;   // already initialized
  BASS_ERROR_CDVOL        = 15;   // CD has no volume control
  BASS_ERROR_NOPAUSE      = 16;   // not paused
  BASS_ERROR_NOTAUDIO     = 17;   // not an audio track
  BASS_ERROR_NOCHAN       = 18;   // can't get a free channel
  BASS_ERROR_ILLTYPE      = 19;   // an illegal type was specified
  BASS_ERROR_ILLPARAM     = 20;   // an illegal parameter was specified
  BASS_ERROR_NO3D         = 21;   // no 3D support
  BASS_ERROR_NOEAX        = 22;   // no EAX support
  BASS_ERROR_DEVICE       = 23;   // illegal device number
  BASS_ERROR_NOPLAY       = 24;   // not playing
  BASS_ERROR_FREQ         = 25;   // illegal sample rate
  BASS_ERROR_NOA3D        = 26;   // A3D.DLL is not installed
  BASS_ERROR_NOTFILE      = 27;   // the stream is not a file stream (WAV/MP3/MP2/MP1/OGG)
  BASS_ERROR_NOHW         = 29;   // no hardware voices available
  BASS_ERROR_NOSYNC       = 30;   // synchronizers have been disabled
  BASS_ERROR_EMPTY        = 31;   // the MOD music has no sequence data
  BASS_ERROR_NONET        = 32;   // no internet connection could be opened
  BASS_ERROR_CREATE       = 33;   // couldn't create the file
  BASS_ERROR_NOFX         = 34;   // effects are not enabled
  BASS_ERROR_PLAYING      = 35;   // the channel is playing
  BASS_ERROR_NOOGG        = 36;   // OGG.DLL/VORBIS.DLL could not be loaded
  BASS_ERROR_UNKNOWN      = -1;   // some other mystery error


  // Device setup flags
  BASS_DEVICE_8BITS       = 1;    // use 8 bit resolution, else 16 bit
  BASS_DEVICE_MONO        = 2;    // use mono, else stereo
  BASS_DEVICE_3D          = 4;    // enable 3D functionality
  {
    If the BASS_DEVICE_3D flag is not specified when
    initilizing BASS, then the 3D flags (BASS_SAMPLE_3D
    and BASS_MUSIC_3D) are ignored when loading/creating
    a sample/stream/music.
  }
  BASS_DEVICE_A3D         = 8;    // enable A3D functionality
  BASS_DEVICE_NOSYNC      = 16;   // disable synchronizers
  BASS_DEVICE_LEAVEVOL    = 32;   // leave the volume as it is
  BASS_DEVICE_OGG         = 64;   // enable OGG support (requires OGG.DLL & VORBIS.DLL)

  // DirectSound interfaces (for use with BASS_GetDSoundObject)
  BASS_OBJECT_DS          = 1;   // IDirectSound
  BASS_OBJECT_DS3DL       = 2;   // IDirectSound3DListener

  // BASS_INFO flags (from DSOUND.H)
  DSCAPS_CONTINUOUSRATE   = $00000010;
  { supports all sample rates between min/maxrate }
  DSCAPS_EMULDRIVER       = $00000020;
  { device does NOT have hardware DirectSound support }
  DSCAPS_CERTIFIED        = $00000040;
  { device driver has been certified by Microsoft }
  {
    The following flags tell what type of samples are
    supported by HARDWARE mixing, all these formats are
    supported by SOFTWARE mixing
  }
  DSCAPS_SECONDARYMONO    = $00000100;     // mono
  DSCAPS_SECONDARYSTEREO  = $00000200;     // stereo
  DSCAPS_SECONDARY8BIT    = $00000400;     // 8 bit
  DSCAPS_SECONDARY16BIT   = $00000800;     // 16 bit

  // Music flags
  BASS_MUSIC_RAMP         = 1;   // normal ramping
  BASS_MUSIC_RAMPS        = 2;   // sensitive ramping
  {
    Ramping doesn't take a lot of extra processing and
    improves the sound quality by removing "clicks".
    Sensitive ramping will leave sharp attacked samples,
    unlike normal ramping.
  }
  BASS_MUSIC_LOOP         = 4;   // loop music
  BASS_MUSIC_FT2MOD       = 16;  // play .MOD as FastTracker 2 does
  BASS_MUSIC_PT1MOD       = 32;  // play .MOD as ProTracker 1 does
  BASS_MUSIC_MONO         = 64;  // force mono mixing (less CPU usage)
  BASS_MUSIC_3D           = 128; // enable 3D functionality
  BASS_MUSIC_POSRESET     = 256; // stop all notes when moving position
  BASS_MUSIC_SURROUND     = 512; // surround sound
  BASS_MUSIC_SURROUND2    = 1024;// surround sound (mode 2)
  BASS_MUSIC_STOPBACK     = 2048;// stop the music on a backwards jump effect
  BASS_MUSIC_FX           = 4096;// enable DX8 effects
  BASS_MUSIC_CALCLEN      = 8192;// calculate playback length


  // Sample info flags
  BASS_SAMPLE_8BITS       = 1;   // 8 bit, else 16 bit
  BASS_SAMPLE_MONO        = 2;   // mono, else stereo
  BASS_SAMPLE_LOOP        = 4;   // looped
  BASS_SAMPLE_3D          = 8;   // 3D functionality enabled
  BASS_SAMPLE_SOFTWARE    = 16;  // it's NOT using hardware mixing
  BASS_SAMPLE_MUTEMAX     = 32;  // muted at max distance (3D only)
  BASS_SAMPLE_VAM         = 64;  // uses the DX7 voice allocation & management
  BASS_SAMPLE_FX          = 128;     // the DX8 effects are enabled
  BASS_SAMPLE_OVER_VOL    = $10000; // override lowest volume
  BASS_SAMPLE_OVER_POS    = $20000; // override longest playing
  BASS_SAMPLE_OVER_DIST   = $30000; // override furthest from listener (3D only)

  BASS_MP3_HALFRATE       = $10000; // reduced quality MP3/MP2/MP1 (half sample rate)
  BASS_MP3_SETPOS         = $20000; // enable seeking on the MP3/MP2/MP1/OGG

  BASS_STREAM_AUTOFREE    = $40000; // automatically free the stream when it stop/ends
  BASS_STREAM_RESTRATE    = $80000; // restrict the download rate of internet file streams
  BASS_STREAM_BLOCK       = $100000; // download & play internet
                                     // file stream (MPx/OGG) in small blocks

  // DX7 voice allocation flags
  BASS_VAM_HARDWARE       = 1;
  {
    Play the sample in hardware. If no hardware voices are available then
    the "play" call will fail
  }
  BASS_VAM_SOFTWARE       = 2;
  {
    Play the sample in software (ie. non-accelerated). No other VAM flags
    may be used together with this flag.
  }

  // DX7 voice management flags
  {
    These flags enable hardware resource stealing... if the hardware has no
    available voices, a currently playing buffer will be stopped to make room
    for the new buffer. NOTE: only samples loaded/created with the
    BASS_SAMPLE_VAM flag are considered for termination by the DX7 voice
    management.
  }
  BASS_VAM_TERM_TIME      = 4;
  {
    If there are no free hardware voices, the buffer to be terminated will be
    the one with the least time left to play.
  }
  BASS_VAM_TERM_DIST      = 8;
  {
    If there are no free hardware voices, the buffer to be terminated will be
    one that was loaded/created with the BASS_SAMPLE_MUTEMAX flag and is
    beyond
    it's max distance. If there are no buffers that match this criteria, then
    the "play" call will fail.
  }
  BASS_VAM_TERM_PRIO      = 16;
  {
    If there are no free hardware voices, the buffer to be terminated will be
    the one with the lowest priority.
  }

  // 3D channel modes
  BASS_3DMODE_NORMAL      = 0;
  { normal 3D processing }
  BASS_3DMODE_RELATIVE    = 1;
  {
    The channel's 3D position (position/velocity/
    orientation) are relative to the listener. When the
    listener's position/velocity/orientation is changed
    with BASS_Set3DPosition, the channel's position
    relative to the listener does not change.
  }
  BASS_3DMODE_OFF         = 2;
  {
    Turn off 3D processing on the channel, the sound will
    be played in the center.
  }

  // EAX environments, use with BASS_SetEAXParameters
  EAX_ENVIRONMENT_OFF               = -1;
  EAX_ENVIRONMENT_GENERIC           = 0;
  EAX_ENVIRONMENT_PADDEDCELL        = 1;
  EAX_ENVIRONMENT_ROOM              = 2;
  EAX_ENVIRONMENT_BATHROOM          = 3;
  EAX_ENVIRONMENT_LIVINGROOM        = 4;
  EAX_ENVIRONMENT_STONEROOM         = 5;
  EAX_ENVIRONMENT_AUDITORIUM        = 6;
  EAX_ENVIRONMENT_CONCERTHALL       = 7;
  EAX_ENVIRONMENT_CAVE              = 8;
  EAX_ENVIRONMENT_ARENA             = 9;
  EAX_ENVIRONMENT_HANGAR            = 10;
  EAX_ENVIRONMENT_CARPETEDHALLWAY   = 11;
  EAX_ENVIRONMENT_HALLWAY           = 12;
  EAX_ENVIRONMENT_STONECORRIDOR     = 13;
  EAX_ENVIRONMENT_ALLEY             = 14;
  EAX_ENVIRONMENT_FOREST            = 15;
  EAX_ENVIRONMENT_CITY              = 16;
  EAX_ENVIRONMENT_MOUNTAINS         = 17;
  EAX_ENVIRONMENT_QUARRY            = 18;
  EAX_ENVIRONMENT_PLAIN             = 19;
  EAX_ENVIRONMENT_PARKINGLOT        = 20;
  EAX_ENVIRONMENT_SEWERPIPE         = 21;
  EAX_ENVIRONMENT_UNDERWATER        = 22;
  EAX_ENVIRONMENT_DRUGGED           = 23;
  EAX_ENVIRONMENT_DIZZY             = 24;
  EAX_ENVIRONMENT_PSYCHOTIC         = 25;
  // total number of environments
  EAX_ENVIRONMENT_COUNT             = 26;

  // A3D resource manager modes
  A3D_RESMODE_OFF                   = 0;    // off
  A3D_RESMODE_NOTIFY                = 1;    // notify when there are no free hardware channels
  A3D_RESMODE_DYNAMIC               = 2;    // non-looping channels are managed
  A3D_RESMODE_DYNAMIC_LOOPERS       = 3;    // all (inc. looping) channels are managed (req A3D 1.2)

  // software 3D mixing algorithm modes (used with BASS_Set3DAlgorithm)
  BASS_3DALG_DEFAULT                = 0;
  {
    default algorithm (currently translates to BASS_3DALG_OFF)
  }
  BASS_3DALG_OFF                    = 1;
  {
    Uses normal left and right panning. The vertical axis is ignored except
    for scaling of volume due to distance. Doppler shift and volume scaling
    are still applied, but the 3D filtering is not performed. This is the
    most CPU efficient software implementation, but provides no virtual 3D
    audio effect. Head Related Transfer Function processing will not be done.
    Since only normal stereo panning is used, a channel using this algorithm
    may be accelerated by a 2D hardware voice if no free 3D hardware voices
    are available.
  }
  BASS_3DALG_FULL                   = 2;
  {
    This algorithm gives the highest quality 3D audio effect, but uses more
    CPU. Requires Windows 98 2nd Edition or Windows 2000 that uses WDM
    drivers, if this mode is not available then BASS_3DALG_OFF will be used
    instead.
  }
  BASS_3DALG_LIGHT                  = 3;
  {
    This algorithm gives a good 3D audio effect, and uses less CPU than the
    FULL mode. Requires Windows 98 2nd Edition or Windows 2000 that uses WDM
    drivers, if this mode is not available then BASS_3DALG_OFF will be used
    instead.
  }

  {
    Sync types (with BASS_ChannelSetSync() "param" and
    SYNCPROC "data" definitions) & flags.
  }
  BASS_SYNC_POS                     = 0;
  BASS_SYNC_MUSICPOS                = 0;
  {
    Sync when a music or stream reaches a position.
    if HMUSIC...
    param: LOWORD=order (0=first, -1=all) HIWORD=row (0=first, -1=all)
    data : LOWORD=order HIWORD=row
    if HSTREAM...
    param: position in bytes
    data : not used
  }
  BASS_SYNC_MUSICINST               = 1;
  {
    Sync when an instrument (sample for the non-instrument
    based formats) is played in a music (not including
    retrigs).
    param: LOWORD=instrument (1=first) HIWORD=note (0=c0...119=b9, -1=all)
    data : LOWORD=note HIWORD=volume (0-64)
  }
  BASS_SYNC_END                     = 2;
  {
    Sync when a music or file stream reaches the end.
    param: not used
    data : not used
  }
  BASS_SYNC_MUSICFX                 = 3;
  {
    Sync when the "sync" effect (XM/MTM/MOD: E8x/Wxx, IT/S3M: S0x) is used.
    param: 0:data=pos, 1:data="x" value
    data : param=0: LOWORD=order HIWORD=row, param=1: "x" value
  }
  BASS_SYNC_MIXTIME                 = $40000000;
  { FLAG: sync at mixtime, else at playtime }
  BASS_SYNC_ONETIME                 = $80000000;
  { FLAG: sync only once, else continuously }

  CDCHANNEL                         = 0; // CD channel, for use with BASS_Channel functions

  // CD ID flags, use with BASS_CDGetID
  BASS_CDID_IDENTITY  = 0;
  BASS_CDID_UPC       = 1;

  // BASS_ChannelGetData flags
  BASS_DATA_FFT512  = $80000000; // 512 sample FFT
  BASS_DATA_FFT1024	= $80000001; // 1024 FFT
  BASS_DATA_FFT2048	= $80000002; // 2048 FFT

  BASS_FX_CHORUS      = 0;      // GUID_DSFX_STANDARD_CHORUS
  BASS_FX_COMPRESSOR  = 1;      // GUID_DSFX_STANDARD_COMPRESSOR
  BASS_FX_DISTORTION  = 2;      // GUID_DSFX_STANDARD_DISTORTION
  BASS_FX_ECHO        = 3;      // GUID_DSFX_STANDARD_ECHO
  BASS_FX_FLANGER     = 4;      // GUID_DSFX_STANDARD_FLANGER
  BASS_FX_GARGLE      = 5;      // GUID_DSFX_STANDARD_GARGLE
  BASS_FX_I3DL2REVERB = 6;      // GUID_DSFX_STANDARD_I3DL2REVERB
  BASS_FX_PARAMEQ     = 7;      // GUID_DSFX_STANDARD_PARAMEQ
  BASS_FX_REVERB      = 8;      // GUID_DSFX_WAVES_REVERB

  BASS_FX_PHASE_NEG_180 = 0;
  BASS_FX_PHASE_NEG_90  = 1;
  BASS_FX_PHASE_ZERO    = 2;
  BASS_FX_PHASE_90      = 3;
  BASS_FX_PHASE_180     = 4;

type
  BOOL = LongBool;
  FLOAT = Single;

  HMUSIC = DWORD;               // MOD music handle
  HSAMPLE = DWORD;              // sample handle
  HCHANNEL = DWORD;             // playing sample's channel handle
  HSTREAM = DWORD;              // sample stream handle
  HSYNC = DWORD;                // synchronizer handle
  HDSP = DWORD;                 // DSP handle
  HFX = DWORD;                  // DX8 effect handle

  BASS_INFO = record
    size: DWORD;               // size of this struct (set this before calling the function)
    flags: DWORD;              // device capabilities (DSCAPS_xxx flags)
    {
      The following values are irrelevant if the device
      doesn't have hardware support
      (DSCAPS_EMULDRIVER is specified in flags)
    }
    hwsize: DWORD;             // size of total device hardware memory
    hwfree: DWORD;             // size of free device hardware memory
    freesam: DWORD;            // number of free sample slots in the hardware
    free3d: DWORD;             // number of free 3D sample slots in the hardware
    minrate: DWORD;            // min sample rate supported by the hardware
    maxrate: DWORD;            // max sample rate supported by the hardware
    eax: BOOL;                 // device supports EAX? (always FALSE if BASS_DEVICE_3D was not used)
    a3d: DWORD;                // A3D version (0=unsupported or BASS_DEVICE_A3D was not used)
    dsver: DWORD;              // DirectSound version (use to check for DX5/7 functions)
  end;

  // Sample info structure
  BASS_SAMPLE = record
    freq: DWORD;               // default playback rate
    volume: DWORD;             // default volume (0-100)
    pan: Longint;              // default pan (-100=left, 0=middle, 100=right)
    flags: DWORD;              // BASS_SAMPLE_xxx flags
    length: DWORD;             // length (in samples, not bytes)
    max: DWORD;                // maximum simultaneous playbacks
    {
      The following are the sample's default 3D attributes
      (if the sample is 3D, BASS_SAMPLE_3D is in flags)
      see BASS_ChannelSet3DAttributes
    }
    mode3d: DWORD;             // BASS_3DMODE_xxx mode
    mindist: FLOAT;            // minimum distance
    maxdist: FLOAT;            // maximum distance
    iangle: DWORD;             // angle of inside projection cone
    oangle: DWORD;             // angle of outside projection cone
    outvol: DWORD;             // delta-volume outside the projection cone
    {
      The following are the defaults used if the sample uses the DirectX 7
      voice allocation/management features.
    }
    vam: DWORD;                // voice allocation/management flags (BASS_VAM_xxx)
    priority: DWORD;           // priority (0=lowest, 0xffffffff=highest)
  end;

  // 3D vector (for 3D positions/velocities/orientations)
  BASS_3DVECTOR = record
    x: FLOAT;                  // +=right, -=left
    y: FLOAT;                  // +=up, -=down
    z: FLOAT;                  // +=front, -=behind
  end;

  BASS_FXCHORUS = record
    fWetDryMix: FLOAT;
    fDepth: FLOAT;
    fFeedback: FLOAT;
    fFrequency: FLOAT;
    lWaveform: DWORD;      // 0=triangle, 1=sine
    fDelay: FLOAT;
    lPhase: DWORD;         // BASS_FX_PHASE_xxx
  end;

  BASS_FXCOMPRESSOR = record
    fGain: FLOAT;
    fAttack: FLOAT;
    fRelease: FLOAT;
    fThreshold: FLOAT;
    fRatio: FLOAT;
    fPredelay: FLOAT;
  end;

  BASS_FXDISTORTION = record
    fGain: FLOAT;
    fEdge: FLOAT;
    fPostEQCenterFrequency: FLOAT;
    fPostEQBandwidth: FLOAT;
    fPreLowpassCutoff: FLOAT;
  end;

  BASS_FXECHO = record
    fWetDryMix: FLOAT;
    fFeedback: FLOAT;
    fLeftDelay: FLOAT;
    fRightDelay: FLOAT;
    lPanDelay: BOOL;
  end;

  BASS_FXFLANGER = record
    fWetDryMix: FLOAT;
    fDepth: FLOAT;
    fFeedback: FLOAT;
    fFrequency: FLOAT;
    lWaveform: DWORD;      // 0=triangle, 1=sine
    fDelay: FLOAT;
    lPhase: DWORD;         // BASS_FX_PHASE_xxx
  end;

  BASS_FXGARGLE = record
    dwRateHz: DWORD;               // Rate of modulation in hz
    dwWaveShape: DWORD;            // 0=triangle, 1=square
  end;

  BASS_FXI3DL2REVERB = record
    lRoom: Longint;                // [-10000, 0]      default: -1000 mB
    lRoomHF: Longint;              // [-10000, 0]      default: 0 mB
    flRoomRolloffFactor: FLOAT;    // [0.0, 10.0]      default: 0.0
    flDecayTime: FLOAT;            // [0.1, 20.0]      default: 1.49s
    flDecayHFRatio: FLOAT;         // [0.1, 2.0]       default: 0.83
    lReflections: Longint;         // [-10000, 1000]   default: -2602 mB
    flReflectionsDelay: FLOAT;     // [0.0, 0.3]       default: 0.007 s
    lReverb: Longint;              // [-10000, 2000]   default: 200 mB
    flReverbDelay: FLOAT;          // [0.0, 0.1]       default: 0.011 s
    flDiffusion: FLOAT;            // [0.0, 100.0]     default: 100.0 %
    flDensity: FLOAT;              // [0.0, 100.0]     default: 100.0 %
    flHFReference: FLOAT;          // [20.0, 20000.0]  default: 5000.0 Hz
  end;

  BASS_FXPARAMEQ = record
    fCenter: FLOAT;
    fBandwidth: FLOAT;
    fGain: FLOAT;
  end;

  BASS_FXREVERB = record
    fInGain: FLOAT;                // [-96.0,0.0]            default: 0.0 dB
    fReverbMix: FLOAT;             // [-96.0,0.0]            default: 0.0 db
    fReverbTime: FLOAT;            // [0.001,3000.0]         default: 1000.0 ms
    fHighFreqRTRatio: FLOAT;       // [0.001,0.999]          default: 0.001
  end;

  // callback function types
  STREAMPROC = ^function conv arg_stdcall (handle: HSTREAM; buffer: Pointer; length: DWORD; user: DWORD): DWORD;
  {
    Stream callback function. NOTE: A stream function should obviously be as
    quick as possible, other streams (and MOD musics) can't be mixed until
    it's finished.
    handle : The stream that needs writing
    buffer : Buffer to write the samples in
    length : Number of bytes to write
    user   : The 'user' parameter value given when calling BASS_StreamCreate
    RETURN : Number of bytes written. If less than "length" then the
             stream is assumed to be at the end, and is stopped.
  }
  SYNCPROC = ^procedure conv arg_stdcall (handle: HSYNC; channel, data: DWORD; user: DWORD);
  {
    Sync callback function. NOTE: a sync callback function should be very
    quick (eg. just posting a message) as other syncs cannot be processed
    until it has finished. If the sync is a "mixtime" sync, then other streams
    and MOD musics can not be mixed until it's finished either.
    handle : The sync that has occured
    channel: Channel that the sync occured in
    data   : Additional data associated with the sync's occurance
    user   : The 'user' parameter given when calling BASS_ChannelSetSync
  }

  DSPPROC = ^procedure conv arg_stdcall (handle: HSYNC; channel: DWORD; buffer: Pointer; length, user: DWORD);
  {
    DSP callback function. NOTE: A DSP function should obviously be as quick
    as possible... other DSP functions, streams and MOD musics can not be
    processed until it's finished.
    handle : The DSP handle
    channel: Channel that the DSP is being applied to
    buffer : Buffer to apply the DSP to
    length : Number of bytes in the buffer
    user   : The 'user' parameter given when calling BASS_ChannelSetDSP
  }

// Functions
function BASS_GetVersion conv arg_stdcall: DWORD; external 'bass.dll' name 'BASS_GetVersion';
{
  Retrieve the version number of BASS that is loaded.
  RETURN : The BASS version  conv arg_stdcall (LOWORD.HIWORD)
}
function BASS_GetDeviceDescription conv arg_stdcall (devnum: Longint; var desc: PChar): BOOL; external 'bass.dll' name 'BASS_GetDeviceDescription';
{
  Get the text description of a device. This function can
  be used to enumerate the available devices.
  devnum : The device  conv arg_stdcall (0=first)
  desc   : Pointer to store pointer to text description at
}
procedure BASS_SetBufferLength conv arg_stdcall (length: FLOAT);
{
  Set the amount that BASS mixes ahead new musics/streams.
  Changing this setting does not affect musics/streams
  that have already been loaded/created. Increasing the
  buffer length, decreases the chance of the sound
  possibly breaking-up on slower computers, but also
  requires more memory. The default length is 0.5 secs.
  length : The buffer length in seconds  conv arg_stdcall (limited to 0.3-2.0s)

  NOTE: This setting does not represent the latency
   conv arg_stdcall (delay between playing and hearing the sound). The
  latency depends on the drivers, the power of the CPU,
  and the complexity of what's being mixed  conv arg_stdcall (eg. an IT
  using filters requires more processing than a plain
  4 channel MOD). So the buffer length actually has no
  effect on the latency.
}
procedure BASS_SetGlobalVolumes conv arg_stdcall (musvol, samvol, strvol: Longint); external 'bass.dll' name 'BASS_SetGlobalVolumes';
{
  Set the global music/sample/stream volume levels.
  musvol : MOD music global volume level  conv arg_stdcall (0-100, -1=leave current)
  samvol : Sample global volume level  conv arg_stdcall (0-100, -1=leave current)
  strvol : Stream global volume level  conv arg_stdcall (0-100, -1=leave current)
}
procedure BASS_GetGlobalVolumes conv arg_stdcall (var musvol, samvol, strvol: DWORD); external 'bass.dll' name 'BASS_GetGlobalVolumes';
{
  Retrive the global music/sample/stream volume levels.
  musvol : MOD music global volume level  conv arg_stdcall (NULL=don't retrieve it)
  samvol : Sample global volume level  conv arg_stdcall (NULL=don't retrieve it)
  strvol : Stream global volume level  conv arg_stdcall (NULL=don't retrieve it)
}
procedure BASS_SetLogCurves conv arg_stdcall (volume, pan: BOOL); external 'bass.dll' name 'BASS_SetLogCurves';
{
  Make the volume/panning values translate to a logarithmic curve,
  or a linear "curve"  conv arg_stdcall (the default).
  volume : volume curve  conv arg_stdcall (FALSE=linear, TRUE=log)
  pan    : panning curve  conv arg_stdcall (FALSE=linear, TRUE=log)
}
procedure BASS_Set3DAlgorithm conv arg_stdcall (algo: DWORD); external 'bass.dll' name 'BASS_Set3DAlgorithm';
{
  Set the 3D algorithm for software mixed 3D channels  conv arg_stdcall (does not affect
  hardware mixed channels). Changing the mode only affects subsequently
  created or loaded samples/streams/musics, not those that already exist.
  Requires DirectX 7 or above.
  algo   : algorithm flag  conv arg_stdcall (BASS_3DALG_xxx)
}
function BASS_ErrorGetCode conv arg_stdcall : DWORD; external 'bass.dll' name 'BASS_ErrorGetCode';
{
  Get the BASS_ERROR_xxx error code. Use this function to
  get the reason for an error.
}
function BASS_Init conv arg_stdcall (device: Longint; freq, flags: DWORD; win: HWND): BOOL; external 'bass.dll' name 'BASS_Init';
{
  Initialize the digital output. This must be called
  before all following BASS functions  conv arg_stdcall (except CD functions).
  The volume is initially set to 100  conv arg_stdcall (the maximum),
  use BASS_SetVolume conv arg_stdcall () to adjust it.
  device : Device to use  conv arg_stdcall (0=first, -1=default, -2=no sound)
  freq   : Output sample rate
  flags  : BASS_DEVICE_xxx flags
  win    : Owner window (0=current foreground window)

  NOTE: The "no sound" device  conv arg_stdcall (device=-2), allows loading
  and "playing" of MOD musics only  conv arg_stdcall (all sample/stream
  functions and most other functions fail). This is so
  that you can still use the MOD musics as synchronizers
  when there is no soundcard present. When using device -2,
  you should still set the other arguments as you would do
  normally.
}
procedure BASS_Free conv arg_stdcall; external 'bass.dll' name 'BASS_Free';
{
  Free all resources used by the digital output, including
  all musics and samples.
}
function BASS_GetDSoundObject conv arg_stdcall (obj: DWORD): Pointer; external 'bass.dll' name 'BASS_GetDSoundObject';
{
  Retrieve a pointer to a DirectSound interface. This can be used by
  advanced users to "plugin" external functionality.
  object : The interface to retrieve  conv arg_stdcall (BASS_OBJECT_xxx)
  RETURN : A pointer to the requested interface  conv arg_stdcall (NULL=error)
}
procedure BASS_GetInfo conv arg_stdcall (var info: BASS_INFO); external 'bass.dll' name 'BASS_GetInfo';
{
  Retrieve some information on the device being used.
  info   : Pointer to store info at
}
function BASS_GetCPU conv arg_stdcall: FLOAT;
{
  Get the current CPU usage of BASS. This includes the
  time taken to mix the MOD musics and sample streams.
  It does not include plain sample mixing which is done
  by the output device  conv arg_stdcall (hardware accelerated) or
  DirectSound  conv arg_stdcall (emulated). Audio CD playback requires no
  CPU usage.
  RETURN : The CPU usage percentage
}
function BASS_Start conv arg_stdcall : BOOL; external 'bass.dll' name 'BASS_Start';
{
  Start the digital output.
}
function BASS_Stop conv arg_stdcall : BOOL; external 'bass.dll' name 'BASS_Stop';
{
  Stop the digital output, stopping all musics/samples/
  streams.
}
function BASS_Pause conv arg_stdcall : BOOL; external 'bass.dll' name 'BASS_Pause';
{
  Stop the digital output, pausing all musics/samples/
  streams. Use BASS_Start to resume the digital output.
}
function BASS_SetVolume conv arg_stdcall (volume: DWORD): BOOL; external 'bass.dll' name 'BASS_SetVolume';
{
  Set the digital output master volume.
  volume : Desired volume level  conv arg_stdcall (0-100)
}
function BASS_GetVolume conv arg_stdcall : Longint; external 'bass.dll' name 'BASS_GetVolume';
{
  Get the digital output master volume.
  RETURN : The volume level  conv arg_stdcall (0-100, -1=error)
}
function BASS_Set3DFactors conv arg_stdcall (distf, rollf, doppf: FLOAT): BOOL;
{
  Set the factors that affect the calculations of 3D sound.
  distf  : Distance factor  conv arg_stdcall (0.0-10.0, 1.0=use meters, 0.3=use feet, <0.0=leave current)
           By default BASS measures distances in meters, you can change this
           setting if you are using a different unit of measurement.
  roolf  : Rolloff factor, how fast the sound quietens with distance
            conv arg_stdcall (0.0=no rolloff, 1.0=real world, 2.0=2x real... 10.0=max, <0.0=leave current)
  doppf  : Doppler factor  conv arg_stdcall (0.0=no doppler, 1.0=real world, 2.0=2x real... 10.0=max, <0.0=leave current)
           The doppler effect is the way a sound appears to change frequency when it is
           moving towards or away from you. The listener and sound velocity settings are
           used to calculate this effect, this "doppf" value can be used to lessen or
           exaggerate the effect.
}
function BASS_Get3DFactors conv arg_stdcall (var distf, rollf, doppf: FLOAT): BOOL; external 'bass.dll' name 'BASS_Get3DFactors';
{
  Get the factors that affect the calculations of 3D sound.
  distf  : Distance factor  conv arg_stdcall (NULL=don't get it)
  roolf  : Rolloff factor  conv arg_stdcall (NULL=don't get it)
  doppf  : Doppler factor  conv arg_stdcall (NULL=don't get it)
}
function BASS_Set3DPosition conv arg_stdcall (var pos, vel, front, top: BASS_3DVECTOR): BOOL; external 'bass.dll' name 'BASS_Set3DPosition';
{
  Set the position/velocity/orientation of the listener  conv arg_stdcall (ie. the player/viewer).
  pos    : Position of the listener  conv arg_stdcall (NULL=leave current)
  vel    : Listener's velocity, used to calculate doppler effect  conv arg_stdcall (NULL=leave current)
  front  : Direction that listener's front is pointing  conv arg_stdcall (NULL=leave current)
  top    : Direction that listener's top is pointing  conv arg_stdcall (NULL=leave current)
         NOTE: front & top must both be set in a single call
}
function BASS_Get3DPosition conv arg_stdcall (var pos, vel, front, top: BASS_3DVECTOR): BOOL; external 'bass.dll' name 'BASS_Get3DPosition';
{
  Get the position/velocity/orientation of the listener.
  pos    : Position of the listener  conv arg_stdcall (NULL=don't get it)
  vel    : Listener's velocity  conv arg_stdcall (NULL=don't get it)
  front  : Direction that listener's front is pointing  conv arg_stdcall (NULL=don't get it)
  top    : Direction that listener's top is pointing  conv arg_stdcall (NULL=don't get it)
         NOTE: front & top must both be retrieved in a single call
}
function BASS_Apply3D conv arg_stdcall : BOOL; external 'bass.dll' name 'BASS_Apply3D';
{
  Apply changes made to the 3D system. This must be called to apply any changes
  made with BASS_Set3DFactors, BASS_Set3DPosition, BASS_ChannelSet3DAttributes or
  BASS_ChannelSet3DPosition. It improves performance to have DirectSound do all the
  required recalculating at the same time like this, rather than recalculating after
  every little change is made. NOTE: This is automatically called when starting a 3D
  sample with BASS_SamplePlay3D/Ex.
}
function BASS_SetEAXParameters conv arg_stdcall (env: Longint; vol, decay, damp: FLOAT): BOOL;
{
  Set the type of EAX environment and it's parameters. Obviously, EAX functions
  have no effect if no EAX supporting device  conv arg_stdcall (ie. SB Live) is used.
  env    : Reverb environment  conv arg_stdcall (EAX_ENVIRONMENT_xxx, -1=leave current)
  vol    : Volume of the reverb  conv arg_stdcall (0.0=off, 1.0=max, <0.0=leave current)
  decay  : Time in seconds it takes the reverb to diminish by 60dB  conv arg_stdcall (0.1-20.0, <0.0=leave current)
  damp   : The damping, high or low frequencies decay faster  conv arg_stdcall (0.0=high decays quickest,
           1.0=low/high decay equally, 2.0=low decays quickest, <0.0=leave current)
}
function BASS_GetEAXParameters conv arg_stdcall (var env: DWORD; var vol, decay, damp: FLOAT): BOOL; external 'bass.dll' name 'BASS_GetEAXParameters';
{
  Get the current EAX parameters.
  env    : Reverb environment  conv arg_stdcall (NULL=don't get it)
  vol    : Reverb volume  conv arg_stdcall (NULL=don't get it)
  decay  : Decay duration  conv arg_stdcall (NULL=don't get it)
  damp   : The damping  conv arg_stdcall (NULL=don't get it)
}
function BASS_SetA3DResManager conv arg_stdcall (mode: DWORD): BOOL; external 'bass.dll' name 'BASS_SetA3DResManager';
{
  Set the A3D resource management mode.
  mode   : The mode  conv arg_stdcall (A3D_RESMODE_OFF=disable resource management,
           A3D_RESMODE_DYNAMIC=enable resource manager but looping channels are not managed,
           A3D_RESMODE_DYNAMIC_LOOPERS=enable resource manager including looping channels,
           A3D_RESMODE_NOTIFY=plays will fail when there are no available resources)
}
function BASS_GetA3DResManager conv arg_stdcall : DWORD; external 'bass.dll' name 'BASS_GetA3DResManager';
{
  Get the A3D resource management mode.
  RETURN : The mode  conv arg_stdcall (0xffffffff=error)
}
function BASS_SetA3DHFAbsorbtion conv arg_stdcall (factor: FLOAT): BOOL;
{
  Set the A3D high frequency absorbtion factor.
  factor  : Absorbtion factor  conv arg_stdcall (0.0=disabled, <1.0=diminished, 1.0=default,
            >1.0=exaggerated)
}
function BASS_GetA3DHFAbsorbtion conv arg_stdcall (var factor: FLOAT): BOOL; external 'bass.dll' name 'BASS_GetA3DHFAbsorbtion';
{
  Retrieve the current A3D high frequency absorbtion factor.
  factor  : The absorbtion factor
}
function BASS_MusicLoad conv arg_stdcall (mem: BOOL; f: Pointer; offset, length, flags: DWORD): HMUSIC; external 'bass.dll' name 'BASS_MusicLoad';
{
  Load a music  conv arg_stdcall (XM/MOD/S3M/IT/MTM). The amplification and pan
  seperation are initially set to 50, use BASS_MusicSetAmplify conv arg_stdcall ()
  and BASS_MusicSetPanSep conv arg_stdcall () to adjust them.
  mem    : TRUE = Load music from memory
  f      : Filename  conv arg_stdcall (mem=FALSE) or memory location  conv arg_stdcall (mem=TRUE)
  offset : File offset to load the music from  conv arg_stdcall (only used if mem=FALSE)
  length : Data length  conv arg_stdcall (only used if mem=FALSE, 0=use to end of file)
  flags  : BASS_MUSIC_xxx flags
  RETURN : The loaded music's handle  conv arg_stdcall (NULL=error)
}
procedure BASS_MusicFree conv arg_stdcall (handle: HMUSIC); external 'bass.dll' name 'BASS_MusicFree';
{
  Free a music's resources.
  handle : Music handle
}
function BASS_MusicGetName conv arg_stdcall (handle: HMUSIC): PChar; external 'bass.dll' name 'BASS_MusicGetName';
{
  Retrieves a music's name.
  handle : Music handle
  RETURN : The music's name  conv arg_stdcall (NULL=error)
}
function BASS_MusicGetLength conv arg_stdcall (handle: HMUSIC; playlen: BOOL): DWORD; external 'bass.dll' name 'BASS_MusicGetLength';
{
  Retrieves the length of a music in patterns (how many "orders" there are)
  or in output bytes (requires BASS_MUSIC_CALCLEN was used with BASS_MusicLoad).
  handle : Music handle
  playlen: TRUE=get the playback length, FALSE=get the pattern length
  RETURN : The length of the music  conv arg_stdcall (0xFFFFFFFF=error)
}
function BASS_MusicPreBuf conv arg_stdcall (handle: HMUSIC): BOOL; external 'bass.dll' name 'BASS_MusicPreBuf';
{
  Pre-buffer initial sample data ready for playback.
  handle : Handle of music
}
function BASS_MusicPlay conv arg_stdcall (handle: HMUSIC): BOOL; external 'bass.dll' name 'BASS_MusicPlay';
{
  Play a music. Playback continues from where it was last stopped/paused.
  Multiple musics may be played simultaneously.
  handle : Handle of music to play
}
function BASS_MusicPlayEx conv arg_stdcall (handle: HMUSIC; pos: DWORD; flags: Longint; reset: BOOL): BOOL; external 'bass.dll' name 'BASS_MusicPlayEx';
{
  Play a music, specifying start position and playback flags.
  handle : Handle of music to play
  pos    : Position to start playback from, LOWORD=order HIWORD=row
  flags  : BASS_MUSIC_xxx flags. These flags overwrite the defaults
           specified when the music was loaded.  conv arg_stdcall (-1=use current flags)
  reset  : TRUE = Stop all current playing notes and reset bpm/etc... */
}
function BASS_MusicSetAmplify conv arg_stdcall (handle: HMUSIC; amp: DWORD): BOOL; external 'bass.dll' name 'BASS_MusicSetAmplify';
{
  Set a music's amplification level.
  handle : Music handle
  amp    : Amplification level  conv arg_stdcall (0-100)
}
function BASS_MusicSetPanSep conv arg_stdcall (handle: HMUSIC; pan: DWORD): BOOL; external 'bass.dll' name 'BASS_MusicSetPanSep';
{
  Set a music's pan seperation.
  handle : Music handle
  pan    : Pan seperation  conv arg_stdcall (0-100, 50=linear)
}
function BASS_MusicSetPositionScaler conv arg_stdcall (handle: HMUSIC; scale: DWORD): BOOL; external 'bass.dll' name 'BASS_MusicSetPositionScaler';
{
  Set a music's "GetPosition" scaler
  When you call BASS_ChannelGetPosition, the "row"  conv arg_stdcall (HIWORD) will be
  scaled by this value. By using a higher scaler, you can get a more
  precise position indication.
  handle : Music handle
  scale  : The scaler  conv arg_stdcall (1-256)
}
function BASS_SampleLoad conv arg_stdcall (mem: BOOL; f: Pointer; offset, length, max, flags: DWORD): HSAMPLE; external 'bass.dll' name 'BASS_SampleLoad';
{
  Load a WAV/MP3/MP2/MP1 sample. If you're loading a sample with 3D
  functionality, then you should use BASS_GetInfo and BASS_SetInfo to set
  the default 3D parameters. You can also use these two functions to set
  the sample's default frequency/volume/pan/looping.
  mem    : TRUE = Load sample from memory
  f      : Filename  conv arg_stdcall (mem=FALSE) or memory location  conv arg_stdcall (mem=TRUE)
  offset : File offset to load the sample from  conv arg_stdcall (only used if mem=FALSE)
  length : Data length  conv arg_stdcall (only used if mem=FALSE, 0=use to end of file)
  max    : Maximum number of simultaneous playbacks  conv arg_stdcall (1-65535)
  flags  : BASS_SAMPLE_xxx flags  conv arg_stdcall (only the LOOP/3D/SOFTWARE/OVER_xxx flags are used)
  RETURN : The loaded sample's handle  conv arg_stdcall (NULL=error)
}
function BASS_SampleCreate conv arg_stdcall (length, freq, max, flags: DWORD): Pointer; external 'bass.dll' name 'BASS_SampleCreate';
{
  Create a sample. This function allows you to generate custom samples, or
  load samples that are not in the WAV format. A pointer is returned to the
  memory location at which you should write the sample's data. After writing
  the data, call BASS_SampleCreateDone to get the new sample's handle.
  length : The sample's length  conv arg_stdcall (in samples, NOT bytes)
  freq   : default sample rate
  max    : Maximum number of simultaneous playbacks  conv arg_stdcall (1-65535)
  flags  : BASS_SAMPLE_xxx flags
  RETURN : Memory location to write the sample's data  conv arg_stdcall (NULL=error)
}
function BASS_SampleCreateDone conv arg_stdcall : HSAMPLE; external 'bass.dll' name 'BASS_SampleCreateDone';
{
  Finished creating a new sample.
  RETURN : The new sample's handle  conv arg_stdcall (NULL=error)
}
procedure BASS_SampleFree conv arg_stdcall (handle: HSAMPLE); external 'bass.dll' name 'BASS_SampleFree';
{
  Free a sample's resources.
  handle : Sample handle
}
function BASS_SampleGetInfo conv arg_stdcall (handle: HSAMPLE; var info: BASS_SAMPLE): BOOL; external 'bass.dll' name 'BASS_SampleGetInfo';
{
  Retrieve a sample's current default attributes.
  handle : Sample handle
  info   : Pointer to store sample info
}
function BASS_SampleSetInfo conv arg_stdcall (handle: HSAMPLE; var info: BASS_SAMPLE): BOOL; external 'bass.dll' name 'BASS_SampleSetInfo';
{
  Set a sample's default attributes.
  handle : Sample handle
  info   : Sample info, only the freq/volume/pan/3D attributes and
           looping/override method flags are used
}
function BASS_SamplePlay conv arg_stdcall (handle: HSAMPLE): HCHANNEL; external 'bass.dll' name 'BASS_SamplePlay';
{
  Play a sample, using the sample's default attributes.
  handle : Handle of sample to play
  RETURN : Handle of channel used to play the sample  conv arg_stdcall (NULL=error)
}
function BASS_SamplePlayEx conv arg_stdcall (handle: HSAMPLE; start: DWORD; freq, volume, pan: Longint; loop: BOOL): HCHANNEL; external 'bass.dll' name 'BASS_SamplePlayEx';
{
  Play a sample, using specified attributes.
  handle : Handle of sample to play
  start  : Playback start position  conv arg_stdcall (in samples, not bytes)
  freq   : Playback rate  conv arg_stdcall (-1=default)
  volume : Volume  conv arg_stdcall (-1=default, 0=silent, 100=max)
  pan    : Pan position  conv arg_stdcall (-101=default, -100=left, 0=middle, 100=right)
  loop   : TRUE = Loop sample  conv arg_stdcall (-1=default)
  RETURN : Handle of channel used to play the sample  conv arg_stdcall (NULL=error)
}
function BASS_SamplePlay3D conv arg_stdcall (handle: HSAMPLE; var pos, orient, vel: BASS_3DVECTOR): HCHANNEL; external 'bass.dll' name 'BASS_SamplePlay3D';
{
  Play a 3D sample, setting it's 3D position, orientation and velocity.
  handle : Handle of sample to play
  pos    : position of the sound  conv arg_stdcall (NULL = x/y/z=0.0)
  orient : orientation of the sound, this is irrelevant if it's an
           omnidirectional sound source  conv arg_stdcall (NULL = x/y/z=0.0)
  vel    : velocity of the sound  conv arg_stdcall (NULL = x/y/z=0.0)
  RETURN : Handle of channel used to play the sample  conv arg_stdcall (NULL=error)
}
function BASS_SamplePlay3DEx conv arg_stdcall (handle: HSAMPLE; var pos, orient, vel: BASS_3DVECTOR; start: DWORD; freq, volume: Longint; loop: BOOL): HCHANNEL; external 'bass.dll' name 'BASS_SamplePlay3DEx';
{
  Play a 3D sample, using specified attributes.
  handle : Handle of sample to play
  pos    : position of the sound  conv arg_stdcall (NULL = x/y/z=0.0)
  orient : orientation of the sound, this is irrelevant if it's an
           omnidirectional sound source  conv arg_stdcall (NULL = x/y/z=0.0)
  vel    : velocity of the sound  conv arg_stdcall (NULL = x/y/z=0.0)
  start  : Playback start position  conv arg_stdcall (in samples, not bytes)
  freq   : Playback rate  conv arg_stdcall (-1=default)
  volume : Volume  conv arg_stdcall (-1=default, 0=silent, 100=max)
  loop   : TRUE = Loop sample  conv arg_stdcall (-1=default)
  RETURN : Handle of channel used to play the sample  conv arg_stdcall (NULL=error)
}
function BASS_SampleStop conv arg_stdcall (handle: HSAMPLE): BOOL; external 'bass.dll' name 'BASS_SampleStop';
{
  Stops all instances of a sample. For example, if a sample is playing
  simultaneously 3 times, calling this function will stop all 3 of them,
  which is obviously simpler than calling BASS_ChannelStop conv arg_stdcall () 3 times.
  handle : Handle of sample to stop
}
function BASS_StreamCreate conv arg_stdcall (freq, flags: DWORD; proc: STREAMPROC; user: DWORD): HSTREAM; external 'bass.dll' name 'BASS_StreamCreate';
{
  Create a user sample stream.
  freq   : Stream playback rate
  flags  : BASS_SAMPLE_xxx flags  conv arg_stdcall (only the 8BITS/MONO/3D flags are used)
  proc   : User defined stream writing function
  user   : The 'user' value passed to the callback function
  RETURN : The created stream's handle  conv arg_stdcall (NULL=error)
}
function BASS_StreamCreateFile conv arg_stdcall (mem: BOOL; f: Pointer; offset, length, flags: DWORD): HSTREAM; external 'bass.dll' name 'BASS_StreamCreateFile';
{
  Create a sample stream from an MP3 or WAV file.
  mem    : TRUE = Stream file from memory
  f      : Filename  conv arg_stdcall (mem=FALSE) or memory location  conv arg_stdcall (mem=TRUE)
  offset : File offset of the stream data
  length : File length  conv arg_stdcall (0=use whole file if mem=FALSE)
  flags  : BASS_SAMPLE_3D and/or BASS_MP3_LOWQ flags
  RETURN : The created stream's handle  conv arg_stdcall (NULL=error)
}
function BASS_StreamCreateURL conv arg_stdcall (url: PChar; flags: DWORD; save: PChar): HSTREAM; external 'bass.dll' name 'BASS_StreamCreateURL';
{
  Create a sample stream from an MP3/MP2/MP1 or WAV file on the internet.
  url    : The URL (beginning with "http://")
  flags  : BASS_SAMPLE_3D/BASS_SAMPLE_MONO/BASS_MP3_HALFRATE/BASS_STREAM_RESTRATE flags
  save   : Filename to save the streamed file as locally (NULL=don't save)
  RETURN : The created stream's handle (NULL=error) */
}
procedure BASS_StreamFree conv arg_stdcall (handle: HSTREAM); external 'bass.dll' name 'BASS_StreamFree';
{
  Free a sample stream's resources.
  stream : Stream handle
}
function BASS_StreamGetLength conv arg_stdcall (handle: HSTREAM): DWORD; external 'bass.dll' name 'BASS_StreamGetLength';
{
  Retrieves the playback length  conv arg_stdcall (in bytes) of a file stream. It's not
  always possible to 100% accurately guess the length of a stream, so
  the length returned may be only an approximation when using some WAV
  codecs.
  handle : Stream handle
  RETURN : The length  conv arg_stdcall (0xffffffff=error)
}
function BASS_StreamPreBuf conv arg_stdcall (handle: HMUSIC): BOOL; external 'bass.dll' name 'BASS_StreamPreBuf';
{
  Pre-buffer initial sample data ready for playback.
  handle : Handle of stream
}
function BASS_StreamPlay conv arg_stdcall (handle: HSTREAM; flush: BOOL; flags: DWORD): BOOL; external 'bass.dll' name 'BASS_StreamPlay';
{
  Play a sample stream, optionally flushing the buffer first.
  handle : Handle of stream to play
  flush  : Flush buffer contents. If you stop a stream and then want to
           continue it from where it stopped, don't flush it. Flushing
           a file stream causes it to restart from the beginning.
  flags  : BASS_SAMPLE_xxx flags  conv arg_stdcall (only affects file streams, and only the
           LOOP flag is used)
}
function BASS_StreamGetFilePosition conv arg_stdcall (handle: HSTREAM; mode: DWORD): DWORD; external 'bass.dll' name 'BASS_StreamGetFilePosition';
{
  Retrieves the file position of the decoding, the download (if streaming from
  the internet), or the end (total length). Obviously only works with file streams.
  handle : Stream handle
  mode   : The position to retrieve (0=decoding, 1=download, 2=end)
  RETURN : The position (0xffffffff=error)
}
function BASS_CDInit conv arg_stdcall (drive: PChar): BOOL; external 'bass.dll' name 'BASS_CDInit';
{
  Initialize the CD functions, must be called before any other CD
  functions. The volume is initially set to 100  conv arg_stdcall (the maximum), use
  BASS_ChannelSetAttributes conv arg_stdcall () to adjust it.
  drive  : The CD drive, for example: "d:"  conv arg_stdcall (NULL=use default drive)
}
procedure BASS_CDFree conv arg_stdcall ; external 'bass.dll' name 'BASS_CDFree';
{
  Free resources used by the CD
}
function BASS_CDInDrive conv arg_stdcall : BOOL; external 'bass.dll' name 'BASS_CDInDrive';
{
  Check if there is a CD in the drive.
}

function BASS_CDGetID conv arg_stdcall (id: DWORD): PChar; external 'bass.dll' name 'BASS_CDGetID';
{
  Retrieves identification info from the CD in the drive.
  id     : BASS_CDID_IDENTITY or BASS_CDID_UPC
  RETURN : ID string (NULL=error)
}

function BASS_CDGetTracks conv arg_stdcall : DWORD; external 'bass.dll' name 'BASS_CDGetTracks';
{
  Retrieves the number of tracks on the CD
}

function BASS_CDPlay conv arg_stdcall (track: DWORD; loop: BOOL; wait: BOOL): BOOL; external 'bass.dll' name 'BASS_CDPlay';
{
  Play a CD track.
  track  : Track number to play  conv arg_stdcall (1=first)
  loop   : TRUE = Loop the track
  wait   : TRUE = don't return until playback has started  conv arg_stdcall (some drives
           will always wait anyway)
}

function BASS_CDGetTrackLength conv arg_stdcall (track: DWORD): DWORD; external 'bass.dll' name 'BASS_CDGetTrackLength';
{
  Retrieves the playback length (in milliseconds) of a cd track.
  track  : The CD track (1=first)
}

{
  A "channel" can be a playing sample  conv arg_stdcall (HCHANNEL), a MOD music  conv arg_stdcall (HMUSIC),
  a sample stream  conv arg_stdcall (HSTREAM), or the CD  conv arg_stdcall (CDCHANNEL). The following
  functions can be used with one or more of these channel types.
}
function BASS_ChannelIsActive conv arg_stdcall (handle: DWORD): Longint; external 'bass.dll' name 'BASS_ChannelIsActive';
{
  Check if a channel is active (playing) or stalled.
  handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL
}
function BASS_ChannelGetFlags conv arg_stdcall (handle: DWORD): DWORD; external 'bass.dll' name 'BASS_ChannelGetFlags';
{
  Get some info about a channel.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HMUSIC/HSTREAM)
  RETURN : BASS_SAMPLE_xxx flags  conv arg_stdcall (0xffffffff=error)
}
function BASS_ChannelStop conv arg_stdcall (handle: DWORD): BOOL; external 'bass.dll' name 'BASS_ChannelStop';
{
  Stop a channel.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
}
function BASS_ChannelPause conv arg_stdcall (handle: DWORD): BOOL; external 'bass.dll' name 'BASS_ChannelPause';
{
  Pause a channel.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
}
function BASS_ChannelResume conv arg_stdcall (handle: DWORD): BOOL; external 'bass.dll' name 'BASS_ChannelResume';
{
  Resume a paused channel.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
}
function BASS_ChannelSetAttributes conv arg_stdcall (handle: DWORD; freq, volume, pan: Longint): BOOL; external 'bass.dll' name 'BASS_ChannelSetAttributes';
{
  Update a channel's attributes. The actual setting may not be exactly
  as specified, depending on the accuracy of the device and drivers.
  NOTE: Only the volume can be adjusted for the CD "channel", but not all
  soundcards allow controlling of the CD volume level.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
  freq   : Playback rate  conv arg_stdcall (-1=leave current)
  volume : Volume  conv arg_stdcall (-1=leave current, 0=silent, 100=max)
  pan    : Pan position  conv arg_stdcall (-101=current, -100=left, 0=middle, 100=right)
           panning has no effect on 3D channels
}
function BASS_ChannelGetAttributes conv arg_stdcall (handle: DWORD; var freq, volume: DWORD; var pan: Longint): BOOL; external 'bass.dll' name 'BASS_ChannelGetAttributes';
{
  Retrieve a channel's attributes. Only the volume is available for
  the CD "channel"  conv arg_stdcall (if allowed by the soundcard/drivers).
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
  freq   : Pointer to store playback rate  conv arg_stdcall (NULL=don't retrieve it)
  volume : Pointer to store volume  conv arg_stdcall (NULL=don't retrieve it)
  pan    : Pointer to store pan position  conv arg_stdcall (NULL=don't retrieve it)
}
function BASS_ChannelSet3DAttributes conv arg_stdcall (handle: DWORD; mode: Longint; min, max: FLOAT; iangle, oangle, outvol: Longint): BOOL;
{
  Set a channel's 3D attributes.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HSTREAM/HMUSIC)
  mode   : BASS_3DMODE_xxx mode  conv arg_stdcall (-1=leave current setting)
  min    : minimum distance, volume stops increasing within this distance  conv arg_stdcall (<0.0=leave current)
  max    : maximum distance, volume stops decreasing past this distance  conv arg_stdcall (<0.0=leave current)
  iangle : angle of inside projection cone in degrees  conv arg_stdcall (360=omnidirectional, -1=leave current)
  oangle : angle of outside projection cone in degrees  conv arg_stdcall (-1=leave current)
           NOTE: iangle & oangle must both be set in a single call
  outvol : delta-volume outside the projection cone  conv arg_stdcall (0=silent, 100=same as inside)
  The iangle/oangle angles decide how wide the sound is projected around the
  orientation angle. Within the inside angle the volume level is the channel
  level as set with BASS_ChannelSetAttributes, from the inside to the outside
  angles the volume gradually changes by the "outvol" setting.
}
function BASS_ChannelGet3DAttributes conv arg_stdcall (handle: DWORD; var mode: DWORD; var min, max: FLOAT; var iangle, oangle, outvol: DWORD): BOOL; external 'bass.dll' name 'BASS_ChannelGet3DAttributes';
{
  Retrieve a channel's 3D attributes.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HSTREAM/HMUSIC)
  mode   : BASS_3DMODE_xxx mode  conv arg_stdcall (NULL=don't retrieve it)
  min    : minumum distance  conv arg_stdcall (NULL=don't retrieve it)
  max    : maximum distance  conv arg_stdcall (NULL=don't retrieve it)
  iangle : angle of inside projection cone  conv arg_stdcall (NULL=don't retrieve it)
  oangle : angle of outside projection cone  conv arg_stdcall (NULL=don't retrieve it)
           NOTE: iangle & oangle must both be retrieved in a single call
  outvol : delta-volume outside the projection cone  conv arg_stdcall (NULL=don't retrieve it)
}
function BASS_ChannelSet3DPosition conv arg_stdcall (handle: DWORD; var pos, orient, vel: BASS_3DVECTOR): BOOL; external 'bass.dll' name 'BASS_ChannelSet3DPosition';
{
  Update a channel's 3D position, orientation and velocity. The velocity
  is only used to calculate the doppler effect.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HSTREAM/HMUSIC)
  pos    : position of the sound  conv arg_stdcall (NULL=leave current)
  orient : orientation of the sound, this is irrelevant if it's an
           omnidirectional sound source  conv arg_stdcall (NULL=leave current)
  vel    : velocity of the sound  conv arg_stdcall (NULL=leave current)
}
function BASS_ChannelGet3DPosition conv arg_stdcall (handle: DWORD; var pos, orient, vel: BASS_3DVECTOR): BOOL; external 'bass.dll' name 'BASS_ChannelGet3DPosition';
{
  Retrieve a channel's current 3D position, orientation and velocity.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HSTREAM/HMUSIC)
  pos    : position of the sound  conv arg_stdcall (NULL=don't retrieve it)
  orient : orientation of the sound, this is irrelevant if it's an
           omnidirectional sound source  conv arg_stdcall (NULL=don't retrieve it)
  vel    : velocity of the sound  conv arg_stdcall (NULL=don't retrieve it)
}
function BASS_ChannelSetPosition conv arg_stdcall (handle: DWORD; pos: DWORD): BOOL; external 'bass.dll' name 'BASS_ChannelSetPosition';
{
  Set the current playback position of a channel.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HMUSIC, or CDCHANNEL)
  pos    : the position
           if HCHANNEL: position in bytes
           if HMUSIC: LOWORD=order HIWORD=row ... use MAKELONG conv arg_stdcall (order,row)
           if CDCHANNEL: position in milliseconds from start of track
}
function BASS_ChannelGetPosition conv arg_stdcall (handle: DWORD): DWORD; external 'bass.dll' name 'BASS_ChannelGetPosition';
{
  Get the current playback position of a channel.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
  RETURN : the position  conv arg_stdcall (0xffffffff=error)
           if HCHANNEL: position in bytes
           if HMUSIC: LOWORD=order HIWORD=row  conv arg_stdcall (see BASS_MusicSetPositionScaler)
           if HSTREAM: total bytes played since the stream was last flushed
           if CDCHANNEL: position in milliseconds from start of track
}
function BASS_ChannelGetLevel conv arg_stdcall (handle: DWORD): DWORD; external 'bass.dll' name 'BASS_ChannelGetLevel';
{
  Calculate a channel's current output level.
  handle : Channel handle  conv arg_stdcall (HMUSIC/HSTREAM)
  RETURN : LOWORD=left level  conv arg_stdcall (0-128) HIWORD=right level  conv arg_stdcall (0-128)  conv arg_stdcall (0xffffffff=error)
}
function BASS_ChannelGetData conv arg_stdcall (handle: DWORD; buffer: Pointer; length: DWORD): DWORD; external 'bass.dll' name 'BASS_ChannelGetData';
{
  Retrieves upto "length" bytes of the channel's current sample data.
  This is useful if you wish to "visualize" the sound.
  handle : Channel handle  conv arg_stdcall (HMUSIC/HSTREAM)
  buffer : Location to write the data
  length : Number of bytes wanted, or a BASS_DATA_xxx flag
  RETURN : Number of bytes actually written to the buffer  conv arg_stdcall (0xffffffff=error)
}
function BASS_ChannelSetSync conv arg_stdcall (handle: DWORD; atype, param: DWORD; proc: SYNCPROC; user: DWORD): HSYNC; external 'bass.dll' name 'BASS_ChannelSetSync';
{
  Setup a sync on a channel. Multiple syncs may be used per channel.
  handle : Channel handle  conv arg_stdcall (currently there are only HMUSIC syncs)
  atype  : Sync type  conv arg_stdcall (BASS_SYNC_xxx type & flags)
  param  : Sync parameters  conv arg_stdcall (see the BASS_SYNC_xxx type description)
  proc   : User defined callback function
  user   : The 'user' value passed to the callback function
  RETURN : Sync handle  conv arg_stdcall (NULL=error)
}
function BASS_ChannelRemoveSync conv arg_stdcall (handle: DWORD; sync: HSYNC): BOOL; external 'bass.dll' name 'BASS_ChannelRemoveSync';
{
  Remove a sync from a channel
  handle : Channel handle  conv arg_stdcall (HMUSIC)
  sync   : Handle of sync to remove
}
function BASS_ChannelSetDSP conv arg_stdcall (handle: DWORD; proc: DSPPROC; user: DWORD): HDSP; external 'bass.dll' name 'BASS_ChannelSetDSP';
{
  Setup a user DSP function on a channel. When multiple DSP functions
  are used on a channel, they are called in the order that they were added.
  handle : Channel handle  conv arg_stdcall (HMUSIC/HSTREAM)
  proc   : User defined callback function
  user   : The 'user' value passed to the callback function
  RETURN : DSP handle  conv arg_stdcall (NULL=error)
}
function BASS_ChannelRemoveDSP conv arg_stdcall (handle: DWORD; dsp: HDSP): BOOL; external 'bass.dll' name 'BASS_ChannelRemoveDSP';
{
  Remove a DSP function from a channel
  handle : Channel handle  conv arg_stdcall (HMUSIC/HSTREAM)
  dsp    : Handle of DSP to remove
}
function BASS_ChannelSetEAXMix conv arg_stdcall (handle: DWORD; mix: FLOAT): BOOL;
{
  Set the wet conv arg_stdcall (reverb)/dry conv arg_stdcall (no reverb) mix ratio on the channel. By default
  the distance of the sound from the listener is used to calculate the mix.
  NOTE: The channel must have 3D functionality enabled for the EAX environment
  to have any affect on it.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HSTREAM/HMUSIC)
  mix    : The ratio  conv arg_stdcall (0.0=reverb off, 1.0=max reverb, -1.0=let EAX calculate
           the reverb mix based on the distance)
}
function BASS_ChannelGetEAXMix conv arg_stdcall (handle: DWORD; var mix: FLOAT): BOOL; external 'bass.dll' name 'BASS_ChannelGetEAXMix';
{
  Get the wet conv arg_stdcall (reverb)/dry conv arg_stdcall (no reverb) mix ratio on the channel.
  handle : Channel handle  conv arg_stdcall (HCHANNEL/HSTREAM/HMUSIC)
  mix    : Pointer to store the ratio at
}
function BASS_ChannelSetLink conv arg_stdcall (handle, chan: DWORD): BOOL; external 'bass.dll' name 'BASS_ChannelSetLink';
{
  Set a link between 2 channels. When the 1st is played/stopped/paused/resumed
  the 2nd channel is also played/stopped/paused/resumed.
  handle : Handle of channel to link "chan" to (HMUSIC/HSTREAM)
  chan   : Handle of channel to link to "handle" (HMUSIC/HSTREAM)
}
function BASS_ChannelRemoveLink conv arg_stdcall (handle, chan: DWORD): BOOL; external 'bass.dll' name 'BASS_ChannelRemoveLink';
{
  Remove a link from a channel.
  handle : Handle of channel to unlink with "chan" (HMUSIC/HSTREAM)
  chan   : Handle of channel to unlink from "handle" (HMUSIC/HSTREAM)
}

procedure BASS_EAXPreset conv arg_stdcall (env: Longint);
{
  This function is defined in the implementation part of this unit.
  It is not part of BASS.DLL but an extra function which makes it easier
  to set the predefined EAX environments.
  env    : a EAX_ENVIRONMENT_xxx constant
}

function BASS_ChannelSetFX conv arg_stdcall (handle, etype: DWORD): HFX;
{
  Setup a DX8 effect on a channel. Can only be used when the channel
  is not playing. Use BASS_FXSetParameters to set the effect parameters.
  Obviously requires DX8.
  handle : Channel handle (HMUSIC/HSTREAM)
  type   : Type of effect to setup (BASS_FX_xxx)
  RETURN : FX handle (NULL=error)
}

function BASS_ChannelRemoveFX conv arg_stdcall (handle: DWORD; fx: HFX): BOOL;
{
  Remove a DX8 effect from a channel. Can only be used when the
  channel is not playing.
  handle : Channel handle (HMUSIC/HSTREAM)
  fx     : Handle of FX to remove
}

function BASS_FXSetParameters conv arg_stdcall (handle: HFX; par: Pointer): BOOL;
{
  Set the parameters of a DX8 effect.
  handle : FX handle
  par    : Pointer to the parameter structure
}

function BASS_FXGetParameters conv arg_stdcall (handle: HFX; par: Pointer): BOOL;
{
  Retrieve the parameters of a DX8 effect.
  handle : FX handle
  par    : Pointer to the parameter structure
}

implementation

function BASS_ChannelSetFX conv arg_stdcall (handle, etype: DWORD): HFX; external 'bass.dll' name 'BASS_ChannelSetFX';
function BASS_ChannelRemoveFX conv arg_stdcall (handle: DWORD; fx: HFX): BOOL; external 'bass.dll' name 'BASS_ChannelRemoveFX';
function BASS_FXSetParameters conv arg_stdcall (handle: HFX; par: Pointer): BOOL; external 'bass.dll' name 'BASS_FXSetParameters';
function BASS_FXGetParameters conv arg_stdcall (handle: HFX; par: Pointer): BOOL; external 'bass.dll' name 'BASS_FXGetParameters';

{$IFNDEF __VER3__}
procedure BASS_SetBufferLength; external 'bass.dll';
function  BASS_GetCPU: FLOAT; external 'bass.dll';
function  BASS_Set3DFactors; external 'bass.dll';
function  BASS_SetEAXParameters; external 'bass.dll';
function  BASS_SetA3DHFAbsorbtion; external 'bass.dll';
function  BASS_ChannelSet3DAttributes; external 'bass.dll';
function  BASS_ChannelSetEAXMix; external 'bass.dll';
{$ELSE}
procedure _BASS_SetBufferLength conv arg_stdcall (length: FLOAT); external 'bass.dll' name 'BASS_SetBufferLength';
function  _BASS_GetCPU: FLOAT; external 'bass.dll' name 'BASS_GetCPU';
function  _BASS_Set3DFactors conv arg_stdcall (distf, rollf, doppf: FLOAT): BOOL; external 'bass.dll' name 'BASS_Set3DFactors';
function  _BASS_SetEAXParameters conv arg_stdcall (env: Longint; vol, decay, damp: FLOAT): BOOL; external 'bass.dll' name 'BASS_SetEAXParameters';
function  _BASS_SetA3DHFAbsorbtion conv arg_stdcall (factor: FLOAT): BOOL; external 'bass.dll' name 'BASS_SetA3DHFAbsorbtion';
function  _BASS_ChannelSet3DAttributes conv arg_stdcall (handle: DWORD; mode: Longint; min, max: FLOAT; iangle, oangle, outvol: Longint): BOOL; external 'bass.dll' name 'BASS_ChannelSet3DAttributes';
function  _BASS_ChannelSetEAXMix conv arg_stdcall (handle: DWORD; mix: FLOAT): BOOL; external 'bass.dll' name 'BASS_ChannelSetEAXMix';

procedure BASS_SetBufferLength conv arg_stdcall (length: FLOAT);
begin
  asm
    push dword ptr [length]
    call _BASS_SetBufferLength
  end;
end;

function BASS_GetCPU conv arg_stdcall : FLOAT;
var
  tmp: FLOAT;
begin
  asm
    call _BASS_GetCPU
    fstp dword ptr [tmp]
  end;
  Result := tmp;
end;

function BASS_Set3DFactors conv arg_stdcall (distf, rollf, doppf: FLOAT): BOOL;
begin
  asm
    push dword ptr [doppf]
    push dword ptr [rollf]
    push dword ptr [distf]
    call _BASS_Set3DFactors
  end;
end;

function BASS_SetEAXParameters conv arg_stdcall (env: Longint; vol, decay, damp: FLOAT): BOOL;
begin
  asm
    push dword ptr [damp]
    push dword ptr [decay]
    push dword ptr [vol]
    push dword ptr [env]
    call _BASS_SetEAXParameters
  end;
end;

function BASS_SetA3DHFAbsorbtion conv arg_stdcall (factor: FLOAT): BOOL;
begin
  asm
    push dword ptr [factor]
    call _BASS_SetA3DHFAbsorbtion
  end;
end;

function BASS_ChannelSet3DAttributes conv arg_stdcall (handle: DWORD; mode: Longint; min, max: FLOAT; iangle, oangle, outvol: Longint): BOOL;
begin
  asm
    push dword ptr [outvol]
    push dword ptr [oangle]
    push dword ptr [iangle]
    push dword ptr [max]
    push dword ptr [min]
    push dword ptr [mode]
    push dword ptr [handle]
    call _BASS_ChannelSet3DAttributes
  end;
end;

function BASS_ChannelSetEAXMix conv arg_stdcall (handle: DWORD; mix: FLOAT): BOOL;
begin
  asm
    push dword ptr [mix]
    push dword ptr [handle]
    call _BASS_ChannelSetEAXMix
  end;
end;
{$ENDIF}

procedure BASS_EAXPreset conv arg_stdcall (env: Longint);
begin
  case (env) of
    EAX_ENVIRONMENT_GENERIC:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_GENERIC, 0.5, 1.493, 0.5);
    EAX_ENVIRONMENT_PADDEDCELL:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_PADDEDCELL, 0.25, 0.1, 0);
    EAX_ENVIRONMENT_ROOM:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_ROOM, 0.417, 0.4, 0.666);
    EAX_ENVIRONMENT_BATHROOM:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_BATHROOM, 0.653, 1.499, 0.166);
    EAX_ENVIRONMENT_LIVINGROOM:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_LIVINGROOM, 0.208, 0.478, 0);
    EAX_ENVIRONMENT_STONEROOM:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_STONEROOM, 0.5, 2.309, 0.888);
    EAX_ENVIRONMENT_AUDITORIUM:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_AUDITORIUM, 0.403, 4.279, 0.5);
    EAX_ENVIRONMENT_CONCERTHALL:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_CONCERTHALL, 0.5, 3.961, 0.5);
    EAX_ENVIRONMENT_CAVE:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_CAVE, 0.5, 2.886, 1.304);
    EAX_ENVIRONMENT_ARENA:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_ARENA, 0.361, 7.284, 0.332);
    EAX_ENVIRONMENT_HANGAR:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_HANGAR, 0.5, 10.0, 0.3);
    EAX_ENVIRONMENT_CARPETEDHALLWAY:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_CARPETEDHALLWAY, 0.153, 0.259, 2.0);
    EAX_ENVIRONMENT_HALLWAY:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_HALLWAY, 0.361, 1.493, 0);
    EAX_ENVIRONMENT_STONECORRIDOR:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_STONECORRIDOR, 0.444, 2.697, 0.638);
    EAX_ENVIRONMENT_ALLEY:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_ALLEY, 0.25, 1.752, 0.776);
    EAX_ENVIRONMENT_FOREST:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_FOREST, 0.111, 3.145, 0.472);
    EAX_ENVIRONMENT_CITY:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_CITY, 0.111, 2.767, 0.224);
    EAX_ENVIRONMENT_MOUNTAINS:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_MOUNTAINS, 0.194, 7.841, 0.472);
    EAX_ENVIRONMENT_QUARRY:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_QUARRY, 1, 1.499, 0.5);
    EAX_ENVIRONMENT_PLAIN:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_PLAIN, 0.097, 2.767, 0.224);
    EAX_ENVIRONMENT_PARKINGLOT:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_PARKINGLOT, 0.208, 1.652, 1.5);
    EAX_ENVIRONMENT_SEWERPIPE:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_SEWERPIPE, 0.652, 2.886, 0.25);
    EAX_ENVIRONMENT_UNDERWATER:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_UNDERWATER, 1, 1.499, 0);
    EAX_ENVIRONMENT_DRUGGED:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_DRUGGED, 0.875, 8.392, 1.388);
    EAX_ENVIRONMENT_DIZZY:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_DIZZY, 0.139, 17.234, 0.666);
    EAX_ENVIRONMENT_PSYCHOTIC:
      BASS_SetEAXParameters(EAX_ENVIRONMENT_PSYCHOTIC, 0.486, 7.563, 0.806);
    else
      BASS_SetEAXParameters(-1, 0, -1, -1);
  end;
end;

end.
