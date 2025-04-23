//=== just pure stuff

#include <stdio.h>
#include <stdlib.h>
#include <FOX.h>
#include <math.h>
#include <windows.h>

#include "Bitmap.h"
#include "BitmapMask.h"

#define M_PI	3.14159265358979323846

long h1,h2,h3,h4,h5,h6,h7;

unsigned char r,g,b;

unsigned long offset;

CBitmap		*texture;

CBitmapMask	*xtable;
CBitmapMask *ytable;

unsigned char temp_line[256000];

main (int argc, char **argv)
{
	FOX_Surface *screen;

	texture = new CBitmap(65536,"texture.bmp");
	
	xtable = new CBitmapMask(256000,"big_x.bin");
	ytable = new CBitmapMask(256000,"big_y.bin");
	
	if ( FOX_Init(FOX_INIT_VIDEO) < 0 ) {
		
		fprintf(stderr, "Couldn't initialize FOX: %s\n",FOX_GetError());
		exit(2);
	}

	atexit(FOX_Quit);

	screen = FOX_SetVideoMode(640, 480, 32,
			(FOX_FULLSCREEN|FOX_DOUBLEBUF));
	
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode %dx%d: %s\n",
				640, 480, FOX_GetError());
		exit(3);
	}
	
	FOX_WM_SetCaption("Main", "main");

	/* Ignore app focus and mouse motion events */
	FOX_EventState(FOX_ACTIVEEVENT, FOX_IGNORE);
	FOX_EventState(FOX_MOUSEMOTION, FOX_IGNORE);

	h3=h4=0;

	while ( FOX_PollEvent(NULL) == 0 ) {

		/* Lock the video surface */
		if ( FOX_LockSurface(screen) < 0 ) {
			continue;
		}

		memcpy(&temp_line,xtable->mBitmapMaskData,1);
		memcpy(xtable->mBitmapMaskData,xtable->mBitmapMaskData+1,256000-1);
		memcpy(xtable->mBitmapMaskData+256000-1,&temp_line,1);

		memcpy(&temp_line,ytable->mBitmapMaskData,1);
		memcpy(ytable->mBitmapMaskData,ytable->mBitmapMaskData+1,256000-1);
		memcpy(ytable->mBitmapMaskData+256000-1,&temp_line,1);


		for (int h1=0;h1<400;h1++)
		{
			for (h2=0;h2<640;h2++)
			{
			 	

				offset = (++xtable->mBitmapMaskData[ 640*(h1/2)+(h2/2)]<<8)+
						  ytable->mBitmapMaskData[ 640*(h1/2)+(h2/2)]++;

				

				b=texture->mBitmapDataB[offset];
				g=texture->mBitmapDataG[offset];
				r=texture->mBitmapDataR[offset];

			((long*)screen->pixels)[640*h1+h2+25600] = 
				(r<<screen->format->Rshift)|
				(g<<screen->format->Gshift)|
				(b<<screen->format->Bshift);
			
			}
		}	
		/* Unlock and update the screen */
		FOX_UnlockSurface(screen);
		FOX_Flip(screen);
	}

	exit(0);
	return 0;

}
