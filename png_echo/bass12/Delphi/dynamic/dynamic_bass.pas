{

  BASS 1.2 Multimedia Library
  -----------------------------
  (c) 1999-2001 Ian Luck.
  Please report bugs/suggestions/etc... to bass@un4seen.com

  Questions, suggestions, etc. regarding the Dynamic Delphi API
  can be sent to MagicRT@hotmail.com

  See the BASS.CHM file for more complete documentation

  NOTE: This unit will only work with BASS.DLL version 1.2
  Check http://www.un4seen.com/music/ for any later versions of BASS.PAS


  How to install
  ----------------
  Copy .PAS to the \LIB subdirectory of your Delphi path or your project dir
  e.g. to C:\Program Files\Borland\Delphi 3\Lib
  Include in your project's uses clause & you should be able to build/run the demo projects.

  Enjoy !
}


unit Dynamic_Bass;

interface

uses
  Windows;

{
Most of the declarations were borrowed from the normal Delphi API...thanks for typing them in, Titus !
}


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
  BASS_ERROR_EMPTY	  = 31;	  // the MOD music has no sequence data
  BASS_ERROR_NONET	  = 32;	  // no internet connection could be opened
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
  BASS_DEVICE_LEAVEVOL	  = 32;	  // leave the volume as it is
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
  BASS_MUSIC_SURROUND	  = 512; // surround sound
  BASS_MUSIC_SURROUND2	  = 1024;// surround sound (mode 2)
  BASS_MUSIC_STOPBACK	  = 2048;// stop the music on a backwards jump effect
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

  BASS_MP3_HALFRATE       = $10000; // reduced quality MP3 (half sample rate)
  BASS_MP3_SETPOS         = $20000; // enable seeking on the MP3

  BASS_STREAM_AUTOFREE	=$40000;// automatically free the stream when it stop/ends
  BASS_STREAM_RESTRATE	=$80000;// restrict the download rate of internet file streams
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
  DWORD = cardinal;
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
    pan: Integer;              // default pan (-100=left, 0=middle, 100=right)
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
  STREAMPROC = function(handle: HSTREAM; buffer: Pointer; length: DWORD; user: DWORD): DWORD; stdcall;
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
  SYNCPROC = procedure(handle: HSYNC; channel, data: DWORD; user: DWORD); stdcall;
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

  DSPPROC = procedure(handle: HSYNC; channel: DWORD; buffer: Pointer; length: DWORD; user: DWORD); stdcall;
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



{
Here comes the stuff I made...
}

// Vars that will hold our dynamically loaded functions...hopefully I didn't miss one of them...if I did miss one (stupid, I know !-; ) please let me know which one it was !

var RT_BASS_GetVersion: Function : DWORD; stdcall;
var RT_BASS_GetDeviceDescription : Function(devnum: Integer; var desc: PChar): BOOL; stdcall;
var RT_BASS_SetBufferLength: Procedure (length: FLOAT); stdcall; 
var RT_BASS_SetGlobalVolumes : procedure (musvol, samvol, strvol: Integer); stdcall;
var RT_BASS_GetGlobalVolumes: procedure (var musvol, samvol, strvol: DWORD); stdcall;
var RT_BASS_SetLogCurves : procedure (volume, pan: BOOL); stdcall;
var RT_BASS_Set3DAlgorithm : procedure (algo: DWORD); stdcall;
var RT_BASS_ErrorGetCode : Function: DWORD; stdcall;
var RT_BASS_Init: Function (device: Integer; freq, flags: DWORD; win: HWND): BOOL; stdcall;
var RT_BASS_Free:procedure ; stdcall;
var RT_BASS_GetDSoundObject: Function(obj: DWORD): Pointer; stdcall;
var RT_BASS_GetInfo: procedure (var info: BASS_INFO); stdcall;
var RT_BASS_GetCPU:Function : FLOAT; stdcall;
var RT_BASS_Start: Function : BOOL; stdcall;
var RT_BASS_Stop: Function : BOOL; stdcall;
var RT_BASS_Pause: Function : BOOL; stdcall;
var RT_BASS_SetVolume: Function(volume: DWORD): BOOL; stdcall;
var RT_BASS_GetVolume: Function : Integer; stdcall;
var RT_BASS_Set3DFactors: Function (distf, rollf, doppf: FLOAT): BOOL; stdcall;
var RT_BASS_Get3DFactors: Function (var distf, rollf, doppf: FLOAT): BOOL; stdcall;
var RT_BASS_Set3DPosition : Function (var pos, vel, front, top: BASS_3DVECTOR): BOOL; stdcall;
var RT_BASS_Get3DPosition : Function (var pos, vel, front, top: BASS_3DVECTOR): BOOL; stdcall;
var RT_BASS_Apply3D: Function : BOOL; stdcall;
var RT_BASS_SetEAXParameters: Function (env: Integer; vol, decay, damp: FLOAT): BOOL; stdcall;
var RT_BASS_GetEAXParameters: Function (var env: DWORD; var vol, decay, damp: FLOAT): BOOL; stdcall;
var RT_BASS_SetA3DResManager: Function (mode: DWORD): BOOL; stdcall;
var RT_BASS_GetA3DResManager: Function: DWORD; stdcall;
var RT_BASS_SetA3DHFAbsorbtion: Function (factor: FLOAT): BOOL; stdcall;
var RT_BASS_GetA3DHFAbsorbtion: Function (var factor: FLOAT): BOOL; stdcall;
var RT_BASS_MusicLoad: Function (mem: BOOL; f: Pointer; offset, length, flags: DWORD): HMUSIC; stdcall;
var RT_BASS_MusicFree: procedure (handle: HMUSIC); stdcall;
var RT_BASS_MusicGetName: Function (handle: HMUSIC): PChar; stdcall;
var RT_BASS_MusicGetLength: Function (handle: HMUSIC; playlen: BOOL): DWORD; stdcall;
var RT_BASS_MusicPlay: Function (handle: HMUSIC): BOOL; stdcall;
var RT_BASS_MusicPlayEx: Function (handle: HMUSIC; pos: DWORD; flags: Integer; reset: BOOL): BOOL; stdcall;
var RT_BASS_MusicSetAmplify: Function (handle: HMUSIC; amp: DWORD): BOOL; stdcall;
var RT_BASS_MusicSetPanSep:function (handle: HMUSIC; pan: DWORD): BOOL; stdcall;
var RT_BASS_MusicSetPositionScaler:Function (handle: HMUSIC; scale: DWORD): BOOL; stdcall;
var RT_BASS_SampleLoad: Function(mem: BOOL; f: Pointer; offset, length, max, flags: DWORD): HSAMPLE; stdcall;
var RT_BASS_SampleCreate: Function (length, freq, max, flags: DWORD): Pointer; stdcall;
var RT_BASS_SampleCreateDone:Function : HSAMPLE; stdcall;
var RT_BASS_SampleFree: procedure (handle: HSAMPLE); stdcall;
var RT_BASS_SampleGetInfo:Function (handle: HSAMPLE; var info: BASS_SAMPLE): BOOL;stdcall;
var RT_BASS_SampleSetInfo: Function (handle: HSAMPLE; var info: BASS_SAMPLE): BOOL; stdcall;
var RT_BASS_SamplePlay: Function (handle: HSAMPLE): HCHANNEL; stdcall;
var RT_BASS_SamplePlayEx: function (handle: HSAMPLE; start: DWORD; freq, volume, pan: Integer; loop: BOOL): HCHANNEL; stdcall;
var RT_BASS_SamplePlay3D: function (handle: HSAMPLE; var pos, orient, vel: BASS_3DVECTOR): HCHANNEL; stdcall;
var RT_BASS_SamplePlay3DEx: function (handle: HSAMPLE; var pos, orient, vel: BASS_3DVECTOR; start: DWORD; freq, volume: Integer; loop: BOOL): HCHANNEL; stdcall;
var RT_BASS_SampleStop:Function (handle: HSAMPLE): BOOL; stdcall;
var RT_BASS_StreamCreate :Function (freq, flags: DWORD; proc: STREAMPROC; user: DWORD): HSTREAM; stdcall;
var RT_BASS_StreamCreateFile: Function (mem: BOOL; f: Pointer; offset, length, flags: DWORD): HSTREAM; stdcall;
var RT_BASS_StreamFree: procedure (handle: HSTREAM); stdcall;
var RT_BASS_StreamGetLength: Function(handle: HSTREAM): DWORD; stdcall;
var RT_BASS_StreamPlay : function (handle: HSTREAM; flush: BOOL; flags: DWORD): BOOL; stdcall;
var RT_BASS_CDInit:Function (drive: PChar): BOOL; stdcall;
var RT_BASS_CDFree:procedure; stdcall;
var RT_BASS_CDInDrive:function : BOOL; stdcall;
var RT_BASS_CDPlay:function (track: DWORD; loop: BOOL; wait: BOOL): BOOL; stdcall;
var RT_BASS_ChannelGetFlags:function (handle: DWORD): DWORD; stdcall;
var RT_BASS_ChannelStop:function (handle: DWORD): BOOL; stdcall;
var RT_BASS_ChannelPause:function (handle: DWORD): BOOL; stdcall;
var RT_BASS_ChannelResume:function (handle: DWORD): BOOL; stdcall;
var RT_BASS_ChannelSetAttributes:function (handle: DWORD; freq, volume, pan: Integer): BOOL; stdcall;
var RT_BASS_ChannelGetAttributes:function (handle: DWORD; var freq, volume: DWORD; var pan: Integer): BOOL; stdcall;
var RT_BASS_ChannelSet3DAttributes:function (handle: DWORD; mode: Integer; min, max: FLOAT; iangle, oangle, outvol: Integer): BOOL; stdcall;
var RT_BASS_ChannelGet3DAttributes:function (handle: DWORD; var mode: DWORD; var min, max: FLOAT; var iangle, oangle, outvol: DWORD): BOOL; stdcall;
var RT_BASS_ChannelSet3DPosition:function (handle: DWORD; var pos, orient, vel: BASS_3DVECTOR): BOOL; stdcall;
var RT_BASS_ChannelGet3DPosition:function (handle: DWORD; var pos, orient, vel: BASS_3DVECTOR): BOOL; stdcall;
var RT_BASS_ChannelSetPosition:function (handle: DWORD; pos: DWORD): BOOL; stdcall;
var RT_BASS_ChannelGetPosition:function (handle: DWORD): DWORD; stdcall;
var RT_BASS_ChannelGetLevel:function (handle: DWORD): DWORD; stdcall;
var RT_BASS_ChannelGetData:function (handle: DWORD; buffer: Pointer; length: DWORD): DWORD; stdcall;
var RT_BASS_ChannelSetSync:function (handle: DWORD; atype, param: DWORD; proc: SYNCPROC; user: DWORD): HSYNC; stdcall;
var RT_BASS_ChannelRemoveSync:function (handle: DWORD; sync: HSYNC): BOOL; stdcall;
var RT_BASS_ChannelSetDSP:function (handle: DWORD; proc: DSPPROC; user: DWORD): HDSP; stdcall;
var RT_BASS_ChannelRemoveDSP:function (handle: DWORD; dsp: HDSP): BOOL; stdcall;
var RT_BASS_ChannelSetEAXMix:function (handle: DWORD; mix: FLOAT): BOOL; stdcall;
var RT_BASS_ChannelGetEAXMix:function (handle: DWORD; var mix: FLOAT): BOOL; stdcall;

// these were added/changed in v0.9
var RT_BASS_StreamCreateURL:Function(URL : PChar; flags:DWORD):HSTREAM;stdcall;
var RT_BASS_StreamGetFilePosition : Function(handle:HSTREAM; mode:DWORD) : DWORD;stdcall;
var RT_BASS_CDGetTracks:Function:DWORD;stdcall;
var RT_BASS_CDGetTrackLength:Function(track:DWORD):DWORD;stdcall;
var RT_BASS_ChannelIsActive: function (handle: DWORD): integer; stdcall;

// v1.1
var RT_BASS_CDGetID:Function(id: DWORD):PChar; stdcall;
var RT_BASS_ChannelSetFX:Function(handle, etype: DWORD): HFX; stdcall;
var RT_BASS_ChannelRemoveFX:Function(handle: DWORD; fx: HFX): BOOL; stdcall;
var RT_BASS_FXSetParameters:Function(handle: HFX; par: Pointer): BOOL; stdcall;
var RT_BASS_FXGetParameters:Function(handle: HFX; par: Pointer): BOOL; stdcall;

// v1.2
var RT_BASS_ChannelSetLink:Function(handle, chan: DWORD): BOOL; stdcall;
var RT_BASS_ChannelRemoveLink:Function(handle, chan: DWORD): BOOL; stdcall;
var RT_BASS_MusicPreBuf:Function(handle: HMUSIC): BOOL; stdcall;
var RT_BASS_StreamPreBuf:Function(handle: HMUSIC): BOOL; stdcall;

// Descriptions for Functions can be found in the Static Delphi API or the BASS Help

procedure BASS_EAXPreset(env: Integer);
{
  This function is defined in the implementation part of this unit.
  It is not part of BASS.DLL but an extra function which makes it easier
  to set the predefined EAX environments.
  env    : a EAX_ENVIRONMENT_xxx constant
}


{ok, now we need something that loads our DLL and gets rid of it as well...}

const BASS_Handle:Thandle=0; // this will hold our handle for the dll; it functions nicely as a mutli-dll prevention unit as well...

Function Load_BASSDLL (const dllpath:string ; const dllfilename:string) :boolean; // well, this functions uses sub-space field harmonics to erase all your credit cards in a 30 meter area...look at it's name, what do you think it does ?
{
  This function loads the DLL, it also checks for multiple instances of the DLL in oone application and prevents it, furthermore it also provides error protection against invalid linked in code.
  It is not part of BASS.DLL, it's here only to enable the dynamic API to perform it's duties (check out the demo for more information)
  and therefore, it needs to be called before any of the BASS.Dll functions (which will exist after this one has been called).
  dllpath  : path to the Dll (where will we load it from ?)
  dllname  :  dllname (if you assigned a random name to the dll, input that here so we know what to load...)
}

Procedure Unload_BASSDLL; // another mystery function ???
{
  This function frees the dynamically linked-in functions from memory...don't forget to call it once you're done !
  Best place to put this is probably the OnDestroy of your Main-Form;
  suggested use in OnDestroy :
  - Call RT_BASS_Free to get rid of everything that's eating memory (automatically called, but just to be on the safe-side !),
  - Then call this function.
}

implementation


Function Load_BASSDLL (const dllpath:string ; const dllfilename:string) :boolean;
var DlPath:String;
var pt:pchar;
var oldmode:integer;
begin
  if BASS_Handle<>0 then result:=true {is it already there ?}
  else begin {go & load the dll}
    if (dllpath[length(dllpath)]<>'\') then dlpath:=dllpath+'\' else dlpath:=dllpath; {path with \ or without... add a \ if there isn't one...}
    pt:=pchar(dlpath+dllfilename); {now cast it a bit}
    oldmode:=SetErrorMode($8001);
    BASS_Handle:=LoadLibrary(pt); // obtain the handle we want
    SetErrorMode(oldmode);
    if BASS_Handle<>0 then
       begin {now we tie the functions to the VARs from above}


@RT_BASS_GetVersion:=GetProcAddress(BASS_Handle,pchar('BASS_GetVersion'));
@RT_BASS_GetDeviceDescription:=GetProcAddress(BASS_Handle,pchar('BASS_GetDeviceDescription'));
@RT_BASS_SetBufferLength:=GetProcAddress(BASS_Handle,pchar('BASS_SetBufferLength'));
@RT_BASS_SetGlobalVolumes:=GetProcAddress(BASS_Handle,pchar('BASS_SetGlobalVolumes'));
@RT_BASS_GetGlobalVolumes:=GetProcAddress(BASS_Handle,pchar('BASS_GetGlobalVolumes'));
@RT_BASS_SetLogCurves:=GetProcAddress(BASS_Handle,pchar('BASS_SetLogCurves'));
@RT_BASS_Set3DAlgorithm:=GetProcAddress(BASS_Handle,pchar('BASS_Set3DAlgorithm'));
@RT_BASS_ErrorGetCode:=GetProcAddress(BASS_Handle,pchar('BASS_ErrorGetCode'));
@RT_BASS_Init:=GetProcAddress(BASS_Handle,pchar('BASS_Init'));
@RT_BASS_Free:=GetProcAddress(BASS_Handle,pchar('BASS_Free'));
@RT_BASS_GetDSoundObject:=GetProcAddress(BASS_Handle,pchar('BASS_GetDSoundObject'));
@RT_BASS_GetInfo:=GetProcAddress(BASS_Handle,pchar('BASS_GetInfo'));
@RT_BASS_GetCPU:=GetProcAddress(BASS_Handle,pchar('BASS_GetCPU'));
@RT_BASS_Start:=GetProcAddress(BASS_Handle,pchar('BASS_Start'));
@RT_BASS_Stop:=GetProcAddress(BASS_Handle,pchar('BASS_Stop'));
@RT_BASS_Pause:=GetProcAddress(BASS_Handle,pchar('BASS_Pause'));
@RT_BASS_SetVolume:=GetProcAddress(BASS_Handle,pchar('BASS_SetVolume'));
@RT_BASS_GetVolume:=GetProcAddress(BASS_Handle,pchar('BASS_GetVolume'));
@RT_BASS_Set3DFactors:=GetProcAddress(BASS_Handle,pchar('BASS_Set3DFactors'));
@RT_BASS_Get3DFactors:=GetProcAddress(BASS_Handle,pchar('BASS_Get3DFactors'));
@RT_BASS_Set3DPosition:=GetProcAddress(BASS_Handle,pchar('BASS_Set3DPosition'));
@RT_BASS_Get3DPosition:=GetProcAddress(BASS_Handle,pchar('BASS_Get3DPosition'));
@RT_BASS_Apply3D:=GetProcAddress(BASS_Handle,pchar('BASS_Apply3D'));
@RT_BASS_SetEAXParameters:=GetProcAddress(BASS_Handle,pchar('BASS_SetEAXParameters'));
@RT_BASS_GetEAXParameters:=GetProcAddress(BASS_Handle,pchar('BASS_GetEAXParameters'));
@RT_BASS_SetA3DResManager:=GetProcAddress(BASS_Handle,pchar('BASS_SetA3DResManager'));
@RT_BASS_GetA3DResManager:=GetProcAddress(BASS_Handle,pchar('BASS_GetA3DResManager'));
@RT_BASS_SetA3DHFAbsorbtion:=GetProcAddress(BASS_Handle,pchar('BASS_SetA3DHFAbsorbtion'));
@RT_BASS_GetA3DHFAbsorbtion:=GetProcAddress(BASS_Handle,pchar('BASS_GetA3DHFAbsorbtion'));
@RT_BASS_MusicLoad:=GetProcAddress(BASS_Handle,pchar('BASS_MusicLoad'));
@RT_BASS_MusicFree:=GetProcAddress(BASS_Handle,pchar('BASS_MusicFree'));
@RT_BASS_MusicGetName:=GetProcAddress(BASS_Handle,pchar('BASS_MusicGetName'));
@RT_BASS_MusicGetLength:=GetProcAddress(BASS_Handle,pchar('BASS_MusicGetLength'));
@RT_BASS_MusicPlay:=GetProcAddress(BASS_Handle,pchar('BASS_MusicPlay'));
@RT_BASS_MusicPlayEx:=GetProcAddress(BASS_Handle,pchar('BASS_MusicPlayEx'));
@RT_BASS_MusicSetAmplify:=GetProcAddress(BASS_Handle,pchar('BASS_MusicSetAmplify'));
@RT_BASS_MusicSetPanSep:=GetProcAddress(BASS_Handle,pchar('BASS_MusicSetPanSep'));
@RT_BASS_MusicSetPositionScaler:=GetProcAddress(BASS_Handle,pchar('BASS_MusicSetPositionScaler'));
@RT_BASS_SampleLoad:=GetProcAddress(BASS_Handle,pchar('BASS_SampleLoad'));
@RT_BASS_SampleCreate:=GetProcAddress(BASS_Handle,pchar('BASS_SampleCreate'));
@RT_BASS_SampleCreateDone:=GetProcAddress(BASS_Handle,pchar('BASS_SampleCreateDone'));
@RT_BASS_SampleFree:=GetProcAddress(BASS_Handle,pchar('BASS_SampleFree'));
@RT_BASS_SampleGetInfo:=GetProcAddress(BASS_Handle,pchar('BASS_SampleGetInfo'));
@RT_BASS_SampleSetInfo:=GetProcAddress(BASS_Handle,pchar('BASS_SampleSetInfo'));
@RT_BASS_SamplePlay:=GetProcAddress(BASS_Handle,pchar('BASS_SamplePlay'));
@RT_BASS_SamplePlayEx:=GetProcAddress(BASS_Handle,pchar('BASS_SamplePlayEx'));
@RT_BASS_SamplePlay3D:=GetProcAddress(BASS_Handle,pchar('BASS_SamplePlay3D'));
@RT_BASS_SamplePlay3DEx:=GetProcAddress(BASS_Handle,pchar('BASS_SamplePlay3DEx'));
@RT_BASS_SampleStop:=GetProcAddress(BASS_Handle,pchar('BASS_SampleStop'));
@RT_BASS_StreamCreate:=GetProcAddress(BASS_Handle,pchar('BASS_StreamCreate'));
@RT_BASS_StreamCreateFile:=GetProcAddress(BASS_Handle,pchar('BASS_StreamCreateFile'));
@RT_BASS_StreamFree:=GetProcAddress(BASS_Handle,pchar('BASS_StreamFree'));
@RT_BASS_StreamGetLength:=GetProcAddress(BASS_Handle,pchar('BASS_StreamGetLength'));
@RT_BASS_StreamPlay:=GetProcAddress(BASS_Handle,pchar('BASS_StreamPlay'));
@RT_BASS_CDInit:=GetProcAddress(BASS_Handle,pchar('BASS_CDInit'));
@RT_BASS_CDFree:=GetProcAddress(BASS_Handle,pchar('BASS_CDFree'));
@RT_BASS_CDInDrive:=GetProcAddress(BASS_Handle,pchar('BASS_CDInDrive'));
@RT_BASS_CDPlay:=GetProcAddress(BASS_Handle,pchar('BASS_CDPlay'));
@RT_BASS_ChannelGetFlags:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelGetFlags'));
@RT_BASS_ChannelStop:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelStop'));
@RT_BASS_ChannelPause:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelPause'));
@RT_BASS_ChannelResume:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelResume'));
@RT_BASS_ChannelSetAttributes:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSetAttributes'));
@RT_BASS_ChannelGetAttributes:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelGetAttributes'));
@RT_BASS_ChannelSet3DAttributes:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSet3DAttributes'));
@RT_BASS_ChannelGet3DAttributes:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelGet3DAttributes'));
@RT_BASS_ChannelSet3DPosition:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSet3DPosition'));
@RT_BASS_ChannelGet3DPosition:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelGet3DPosition'));
@RT_BASS_ChannelSetPosition:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSetPosition'));
@RT_BASS_ChannelGetPosition:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelGetPosition'));
@RT_BASS_ChannelGetLevel:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelGetLevel'));
@RT_BASS_ChannelGetData:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelGetData'));
@RT_BASS_ChannelSetSync:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSetSync'));
@RT_BASS_ChannelRemoveSync:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelRemoveSync'));
@RT_BASS_ChannelSetDSP:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSetDSP'));
@RT_BASS_ChannelRemoveDSP:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelRemoveDSP'));
@RT_BASS_ChannelSetEAXMix:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSetEAXMix'));
@RT_BASS_ChannelGetEAXMix:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelGetEAXMix'));

// new in v0.9
@RT_BASS_StreamCreateURL:=GetProcAddress(BASS_Handle,pchar('BASS_StreamCreateURL'));
@RT_BASS_StreamGetFilePosition:=GetProcAddress(BASS_Handle,pchar('BASS_StreamGetFilePosition'));
@RT_BASS_CDGetTracks:=GetProcAddress(BASS_Handle,pchar('BASS_CDGetTracks'));
@RT_BASS_CDGetTrackLength:=GetProcAddress(BASS_Handle,pchar('BASS_CDGetTrackLength'));
@RT_BASS_ChannelIsActive:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelIsActive'));

// v1.1
@RT_BASS_CDGetID:=GetProcAddress(BASS_Handle,pchar('BASS_CDGetID'));
@RT_BASS_ChannelSetFX:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSetFX'));
@RT_BASS_ChannelRemoveFX:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelRemoveFX'));
@RT_BASS_FXSetParameters:=GetProcAddress(BASS_Handle,pchar('BASS_FXSetParameters'));
@RT_BASS_FXGetParameters:=GetProcAddress(BASS_Handle,pchar('BASS_FXGetParameters'));

// v1.2
@RT_BASS_ChannelSetLink:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelSetLink'));
@RT_BASS_ChannelRemoveLink:=GetProcAddress(BASS_Handle,pchar('BASS_ChannelRemoveLink'));
@RT_BASS_MusicPreBuf:=GetProcAddress(BASS_Handle,pchar('BASS_MusicPreBuf'));
@RT_BASS_StreamPreBuf:=GetProcAddress(BASS_Handle,pchar('BASS_StreamPreBuf'));

      {now check if everything is linked in correctly}
      if
(@RT_BASS_GetVersion=nil)  or
(@RT_BASS_GetDeviceDescription=nil)  or
(@RT_BASS_SetBufferLength=nil)  or
(@RT_BASS_SetGlobalVolumes=nil)  or
(@RT_BASS_GetGlobalVolumes=nil)  or
(@RT_BASS_SetLogCurves=nil)  or
(@RT_BASS_Set3DAlgorithm=nil)  or
(@RT_BASS_ErrorGetCode=nil)  or
(@RT_BASS_Init=nil)  or
(@RT_BASS_Free=nil)  or
(@RT_BASS_GetDSoundObject=nil)  or
(@RT_BASS_GetInfo=nil)  or
(@RT_BASS_GetCPU=nil)  or
(@RT_BASS_Start=nil)  or
(@RT_BASS_Stop=nil)  or
(@RT_BASS_Pause=nil)  or
(@RT_BASS_SetVolume=nil)  or
(@RT_BASS_GetVolume=nil)  or
(@RT_BASS_Set3DFactors=nil)  or
(@RT_BASS_Get3DFactors=nil)  or
(@RT_BASS_Set3DPosition=nil)  or
(@RT_BASS_Get3DPosition=nil)  or
(@RT_BASS_Apply3D=nil)  or
(@RT_BASS_SetEAXParameters=nil)  or
(@RT_BASS_GetEAXParameters=nil)  or
(@RT_BASS_SetA3DResManager=nil)  or
(@RT_BASS_GetA3DResManager=nil)  or
(@RT_BASS_SetA3DHFAbsorbtion=nil)  or
(@RT_BASS_GetA3DHFAbsorbtion=nil)  or
(@RT_BASS_MusicLoad=nil)  or
(@RT_BASS_MusicFree=nil)  or
(@RT_BASS_MusicGetName=nil)  or
(@RT_BASS_MusicGetLength=nil)  or
(@RT_BASS_MusicPlay=nil)  or
(@RT_BASS_MusicPlayEx=nil)  or
(@RT_BASS_MusicSetAmplify=nil)  or
(@RT_BASS_MusicSetPanSep=nil)  or
(@RT_BASS_MusicSetPositionScaler=nil)  or
(@RT_BASS_SampleLoad=nil)  or
(@RT_BASS_SampleCreate=nil)  or
(@RT_BASS_SampleCreateDone=nil)  or
(@RT_BASS_SampleFree=nil)  or
(@RT_BASS_SampleGetInfo=nil)  or
(@RT_BASS_SampleSetInfo=nil)  or
(@RT_BASS_SamplePlay=nil)  or
(@RT_BASS_SamplePlayEx=nil)  or
(@RT_BASS_SamplePlay3D=nil)  or
(@RT_BASS_SamplePlay3DEx=nil)  or
(@RT_BASS_SampleStop=nil)  or
(@RT_BASS_StreamCreate=nil)  or
(@RT_BASS_StreamCreateFile=nil)  or
(@RT_BASS_StreamFree=nil)  or
(@RT_BASS_StreamGetLength=nil)  or
(@RT_BASS_StreamPlay=nil) or
(@RT_BASS_CDInit=nil)  or
(@RT_BASS_CDFree=nil)  or
(@RT_BASS_CDInDrive=nil)  or
(@RT_BASS_CDPlay=nil)  or
(@RT_BASS_ChannelGetFlags=nil)  or
(@RT_BASS_ChannelStop=nil)  or
(@RT_BASS_ChannelPause=nil)  or
(@RT_BASS_ChannelResume=nil)  or
(@RT_BASS_ChannelSetAttributes=nil)  or
(@RT_BASS_ChannelGetAttributes=nil)  or
(@RT_BASS_ChannelSet3DAttributes=nil)  or
(@RT_BASS_ChannelGet3DAttributes=nil)  or
(@RT_BASS_ChannelSet3DPosition=nil)  or
(@RT_BASS_ChannelGet3DPosition=nil)  or
(@RT_BASS_ChannelSetPosition=nil)  or
(@RT_BASS_ChannelGetPosition=nil)  or
(@RT_BASS_ChannelGetLevel=nil)  or
(@RT_BASS_ChannelGetData=nil)  or
(@RT_BASS_ChannelSetSync=nil)  or
(@RT_BASS_ChannelRemoveSync=nil)  or
(@RT_BASS_ChannelSetDSP=nil)  or
(@RT_BASS_ChannelRemoveDSP=nil)  or
(@RT_BASS_ChannelSetEAXMix=nil)  or
(@RT_BASS_ChannelGetEAXMix=nil)  or

(@RT_BASS_StreamCreateURL=nil) or
(@RT_BASS_StreamGetFilePosition=nil) or
(@RT_BASS_CDGetTracks=nil) or
(@RT_BASS_CDGetTrackLength=nil) or
(@RT_BASS_ChannelIsActive=nil) or

(@RT_BASS_CDGetID=nil) or
(@RT_BASS_ChannelSetFX=nil) or
(@RT_BASS_ChannelRemoveFX=nil) or
(@RT_BASS_FXSetParameters=nil) or
(@RT_BASS_FXGetParameters=nil) or

(@RT_BASS_ChannelSetLink=nil) or
(@RT_BASS_ChannelRemoveLink=nil) or
(@RT_BASS_MusicPreBuf=nil) or
(@RT_BASS_StreamPreBuf=nil)

           {or ... anything I forgot ?}

         then
          begin {if something went wrong during linking, free library & reset handle}
            FreeLibrary(BASS_Handle);
           BASS_Handle:=0;
         end;
       end;
    result:=(BASS_Handle<>0);
  end;
end;

Procedure Unload_BASSDLL;
begin
  if BASS_Handle<>0 then
     begin
       if (@RT_BASS_Free<>nil) then RT_BASS_Free; // make sure we kick out everything
       FreeLibrary(BASS_Handle);
      end;
  BASS_Handle:=0;
end;


procedure BASS_EAXPreset(env: Integer);
begin
if (BASS_Handle<>0) and (@RT_BASS_SetEAXParameters<>nil) then // it has to be loaded if you want to be able to call RT_BASS_???...
  case (env) of
    EAX_ENVIRONMENT_GENERIC:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_GENERIC, 0.5, 1.493, 0.5);
    EAX_ENVIRONMENT_PADDEDCELL:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_PADDEDCELL, 0.25, 0.1, 0);
    EAX_ENVIRONMENT_ROOM:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_ROOM, 0.417, 0.4, 0.666);
    EAX_ENVIRONMENT_BATHROOM:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_BATHROOM, 0.653, 1.499, 0.166);
    EAX_ENVIRONMENT_LIVINGROOM:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_LIVINGROOM, 0.208, 0.478, 0);
    EAX_ENVIRONMENT_STONEROOM:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_STONEROOM, 0.5, 2.309, 0.888);
    EAX_ENVIRONMENT_AUDITORIUM:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_AUDITORIUM, 0.403, 4.279, 0.5);
    EAX_ENVIRONMENT_CONCERTHALL:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_CONCERTHALL, 0.5, 3.961, 0.5);
    EAX_ENVIRONMENT_CAVE:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_CAVE, 0.5, 2.886, 1.304);
    EAX_ENVIRONMENT_ARENA:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_ARENA, 0.361, 7.284, 0.332);
    EAX_ENVIRONMENT_HANGAR:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_HANGAR, 0.5, 10.0, 0.3);
    EAX_ENVIRONMENT_CARPETEDHALLWAY:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_CARPETEDHALLWAY, 0.153, 0.259, 2.0);
    EAX_ENVIRONMENT_HALLWAY:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_HALLWAY, 0.361, 1.493, 0);
    EAX_ENVIRONMENT_STONECORRIDOR:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_STONECORRIDOR, 0.444, 2.697, 0.638);
    EAX_ENVIRONMENT_ALLEY:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_ALLEY, 0.25, 1.752, 0.776);
    EAX_ENVIRONMENT_FOREST:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_FOREST, 0.111, 3.145, 0.472);
    EAX_ENVIRONMENT_CITY:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_CITY, 0.111, 2.767, 0.224);
    EAX_ENVIRONMENT_MOUNTAINS:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_MOUNTAINS, 0.194, 7.841, 0.472);
    EAX_ENVIRONMENT_QUARRY:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_QUARRY, 1, 1.499, 0.5);
    EAX_ENVIRONMENT_PLAIN:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_PLAIN, 0.097, 2.767, 0.224);
    EAX_ENVIRONMENT_PARKINGLOT:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_PARKINGLOT, 0.208, 1.652, 1.5);
    EAX_ENVIRONMENT_SEWERPIPE:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_SEWERPIPE, 0.652, 2.886, 0.25);
    EAX_ENVIRONMENT_UNDERWATER:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_UNDERWATER, 1, 1.499, 0);
    EAX_ENVIRONMENT_DRUGGED:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_DRUGGED, 0.875, 8.392, 1.388);
    EAX_ENVIRONMENT_DIZZY:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_DIZZY, 0.139, 17.234, 0.666);
    EAX_ENVIRONMENT_PSYCHOTIC:
      RT_BASS_SetEAXParameters(EAX_ENVIRONMENT_PSYCHOTIC, 0.486, 7.563, 0.806);
    else
      RT_BASS_SetEAXParameters(-1, 0, -1, -1);
  end;
end;

end.
// END OF FILE /////////////////////////////////////////////////////////////////

