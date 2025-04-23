/* BASS Simple Console Test, copyright (c) 1999-2001 Ian Luck.
==============================================================
Imports: bass.lib, kernel32.lib, user32.lib, winmm.lib
*/

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <conio.h>
#include "bass.h"

/* display error messages */
void Error(char *text) 
{
	printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
	BASS_Free();
	ExitProcess(0);
}

static DWORD starttime;

/* looping synchronizer, resets the clock */
void CALLBACK LoopSync(HSYNC handle, DWORD channel, DWORD data, DWORD user)
{
	starttime=timeGetTime();
}

void main(int argc, char **argv)
{
	HMUSIC mod;
	HSTREAM str;
	DWORD time,pos,level;
	int a,freq;
	BOOL mono=FALSE;
	int act;

	printf("Simple console mode BASS example : MOD/MPx/OGG/WAV player\n"
			"---------------------------------------------------------\n");

	/* check that BASS 1.2 was loaded */
	if (BASS_GetVersion()!=MAKELONG(1,2)) {
		printf("BASS version 1.2 was not loaded\n");
		return;
	}

	if (argc!=2) {
		printf("\tusage: contest <file>\n");
		return;
	}

	/* setup output - default device, 44100hz, stereo, 16 bits, OGG if available */
	if (!BASS_Init(-1,44100,BASS_DEVICE_OGG,0)
		&& !BASS_Init(-1,44100,0,0))
		Error("Can't initialize device");

	/* try streaming the file */
	if ((str=BASS_StreamCreateFile(FALSE,argv[1],0,0,0))
		|| (str=BASS_StreamCreateURL(argv[1],BASS_STREAM_RESTRATE,0))) {
		/* check if the stream is mono (for the level indicator) */
		mono=BASS_ChannelGetFlags(str)&BASS_SAMPLE_MONO;
		/* set a synchronizer for when the stream reaches the end */
		BASS_ChannelSetSync(str,BASS_SYNC_END,0,&LoopSync,0);
		pos=BASS_StreamGetLength(str);
		if (BASS_StreamGetFilePosition(str,1)!=-1) {
			/* streaming from the internet */
			if (pos)
				printf("streaming internet file [%d bytes]",pos);
			else
				printf("streaming internet file");
		} else
			printf("streaming file [%d bytes]",pos);
	} else {
		/* try loading the MOD (with looping, sensitive ramping, surround sound and calculate the duration) */
		if (!(mod=BASS_MusicLoad(FALSE,argv[1],0,0,BASS_MUSIC_LOOP|BASS_MUSIC_RAMPS|BASS_MUSIC_SURROUND|BASS_MUSIC_CALCLEN)))
			/* not a MOD either */
			Error("Can't play the file");
		/* set a synchronizer for when the MOD reaches the end */
		BASS_ChannelSetSync(mod,BASS_SYNC_END,0,&LoopSync,0);
		printf("playing MOD music \"%s\" [%d orders]",BASS_MusicGetName(mod),BASS_MusicGetLength(mod,FALSE));
		pos=BASS_MusicGetLength(mod,TRUE);
	}

	/* calulate and display the time length */
	if (pos) {
		a=BASS_ChannelGetFlags(str?str:mod);
		BASS_ChannelGetAttributes(str?str:mod,&freq,NULL,NULL);
		pos/=freq*(a&BASS_SAMPLE_MONO?1:2)*(a&BASS_SAMPLE_8BITS?1:2);
		printf(" %d:%02d\n",pos/60,pos%60);
	} else /* no time length available */
		printf("\n");

	BASS_Start();
	if (str)
		BASS_StreamPlay(str,FALSE,BASS_SAMPLE_LOOP);
	else
		BASS_MusicPlay(mod);
	starttime=timeGetTime();

   /* NOTE: some compilers don't support _kbhit */
	while (!_kbhit() && (act=BASS_ChannelIsActive(str?str:mod))) {
		/* display some stuff and wait a bit */
		time=timeGetTime()-starttime;
		level=BASS_ChannelGetLevel(str?str:mod);
		pos=BASS_ChannelGetPosition(str?str:mod);
		if (str)
			printf("pos %09d - time %d:%02d - L ",pos,time/60000,(time/1000)%60);
		else
			printf("pos %03d:%03d - time %d:%02d - L ",LOWORD(pos),HIWORD(pos),time/60000,(time/1000)%60);
		if (act==2) {
			/* internet file stream playback has stalled */
			printf("-- buffering : %05d --",
				BASS_StreamGetFilePosition(str,1)-BASS_StreamGetFilePosition(str,0));
		} else {
			for (a=93;a;a=a*2/3) putchar(LOWORD(level)>=a?'*':'-');
			putchar(' ');
			if (mono)
				for (a=1;a<128;a+=a-(a>>1)) putchar(LOWORD(level)>=a?'*':'-');
			else
				for (a=1;a<128;a+=a-(a>>1)) putchar(HIWORD(level)>=a?'*':'-');
		}
		printf(" R - cpu %.1f%%  \r",BASS_GetCPU());
		Sleep(50);
	}
	printf("                                                                         \r");

	/* get the frequency... and wind it down */
	BASS_ChannelGetAttributes(str?str:mod,&freq,NULL,NULL);
	level=freq/40;
	for (;freq>2000;freq-=level) {
		BASS_ChannelSetAttributes(str?str:mod,freq,-1,-101);
		Sleep(5);
	}

	/* fade-out to avoid a "click" */
	for (a=100;a>=0;a-=5) {
		BASS_SetVolume(a);
		Sleep(1);
	}

	BASS_Free();
}
