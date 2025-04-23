//=== just pure stuff

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <SDL.h>
#include <math.h>

#include "Bitmap.h"
#include "BitmapMask.h"
#include "YTable.h"

#define M_PI	3.14159265358979323846

long h1,h2,h3,h4;

unsigned long r,g,b;

unsigned long offset;

CBitmap		*xtable;
CBitmap		*rysunek;

CBitmapMask	*vaddr01,*vaddr02,*vaddr03;

CYtable		*ytable;

unsigned char temp_liner[800];
unsigned char temp_lineg[800];
unsigned char temp_lineb[800];


main (int argc, char **argv)
{
	SDL_Surface *screen;

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(2);
	}

	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(800, 600, 16,
			(SDL_FULLSCREEN|SDL_HWSURFACE));
	
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode %dx%d: %s\n",
				800, 600, SDL_GetError());
		exit(3);
	}
	
	SDL_WM_SetCaption("Main", "main");

	/* Ignore app focus and mouse motion events */
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

	//ytable = new CYtable(600,800);

	rysunek = new CBitmap(480000,"pic.bmp");
	
	vaddr01 = new CBitmapMask(480000);
	vaddr02 = new CBitmapMask(480000);
	vaddr03 = new CBitmapMask(480000);

	for (h1=0;h1<480000;h1++)
	{
		vaddr01->mBitmapMaskData[h1]=rysunek->mBitmapDataB[h1];
		vaddr02->mBitmapMaskData[h1]=rysunek->mBitmapDataG[h1];
		vaddr03->mBitmapMaskData[h1]=rysunek->mBitmapDataR[h1];
	}

	while ( SDL_PollEvent(NULL) == 0 ) {

		/* Lock the video surface */
		if ( SDL_LockSurface(screen) < 0 ) {
			continue;
		}


		for (int h1=0;h1<480000;h1++)
		{



			//r = (vaddr01->mBitmapMaskData[h1]&0xf80000)>>8;
			//g = (vaddr02->mBitmapMaskData[h1]&0xfc00)>>5;
			//b = (vaddr03->mBitmapMaskData[h1]&0xf8)>>3;



  //((r>>3)<<11) +
//    ((g>>2)<<5 ) +
  //  ((b>>3)    ); 

//		target_red = (source_color & 0xf80000) >> 8;
//        target_green = (source_color & 0xfc00) >> 5;
//        target_blue = (source_color & 0xf8) >> 3;

//	r = col >> 4;
//	g = b = col >> 3;
	
//	*(tmp++) = (word) ((r<<11) + (g<<5) + b);

			//r = (((pixel&0xF800)>>11)<<3);		 			\
	//g = (((pixel&0x07E0)>>5)<<2); 					\
	//b = ((pixel&0x001F)<<3); 					\			
			
		RGB_FROM_RGB565(screen->pixels[h1]>>2,vaddr01->mBitmapMaskData[h1],
											  vaddr02->mBitmapMaskData[h1],
											  vaddr03->mBitmapMaskData[h1]);

			//((long*)screen->pixels)[h1>>1] = RGB_FROM_RGB565(pixel, r, g, b)
				//(((vaddr01->mBitmapMaskData[h1]&0xf800)>>11)<<3)|
				//(((vaddr02->mBitmapMaskData[h1]&0x07e0)>>5)<<2)|
				//(((vaddr03->mBitmapMaskData[h1]&0x001f)<<3));
			
		}	
		/* Unlock and update the screen */
		SDL_UnlockSurface(screen);
		SDL_Flip(screen);
	}

	exit(0);
	return 0;

}
