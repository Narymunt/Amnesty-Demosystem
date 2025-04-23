// Sound.h: interface for the CSound class.
//
// klasa do obslugi sampla w formacie .wav 
//////////////////////////////////////////////////////////////////////

#ifndef _GHOSTSOUND_
#define _GHOSTSOUND_

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dshow.h>
#include <dmusicc.h>
#include <dmusici.h>

#include "Audio.h"

class __declspec(dllexport) CSound  
{

public:
	CSound(char pCurrentDirectory[200], CAudio *currentAudio, WCHAR wstrFilename[MAX_PATH]);

	virtual ~CSound();
	
	virtual void Play(CAudio *currentAudio,float x_sound, float y_sound, float z_sound);


	IDirectMusicSegment8     *g_pMusicSegmentData;
	
	IDirectSound3DBuffer8	 *pDirectSoundBuffer;
	IDirectMusicAudioPath8	 *pDirectAudioPath;

	IDirectMusicPerformance8 *pMusic_Performance;  


};

#endif 
