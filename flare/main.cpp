//=== just pure stuff

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>
#include <string.h>

#define M_PI	3.14159265358979323846

long h1,h2,h3,h4;

long x,y;

unsigned long framecount;

double la,lb,lc;

unsigned char pic_r[480000];
unsigned char pic_g[480000];
unsigned char pic_b[480000];

unsigned char bump_r[480000];
unsigned char bump_g[480000];
unsigned char bump_b[480000];

unsigned char buffer[1440054];

unsigned long dX, dY, lX, lY;

unsigned char vaddr_r[480000];
unsigned char vaddr_g[480000];
unsigned char vaddr_b[480000];

unsigned char light_r[65536];
unsigned char light_g[65536];
unsigned char light_b[65536];

unsigned char r,g,b;

void put_light(int lx, int ly, unsigned char *l_r, unsigned char *l_g, unsigned char *l_b)
{

	unsigned int cr;

	// na srodek flary

	lx+=128; ly+=128;

	for (int l1=0;l1<256;l1++)
	{
		for (int l2=0;l2<256;l2++)
		{
			if ( ((lx+l2)>0)&&((lx+l2)<800)&&((ly+l1)>0)&&((ly+l1)<600))
			{
			
			cr = (int) vaddr_r[(800*(l1+ly))+lx+l2];
			cr +=l_r[(l1<<8)+l2];
			if (cr>255) cr=255;
			vaddr_r[(800*(l1+ly))+lx+l2]=cr;

			cr = (int) vaddr_g[(800*(l1+ly))+lx+l2];
			cr +=l_g[(l1<<8)+l2];
			if (cr>255) cr=255;
			vaddr_g[(800*(l1+ly))+lx+l2]=cr;

			cr = (int) vaddr_b[(800*(l1+ly))+lx+l2];
			cr +=l_b[(l1<<8)+l2];
			if (cr>255) cr=255;
			vaddr_b[(800*(l1+ly))+lx+l2]=cr;
			}
		}
	}
}

void put_light2(int lx, int ly, char *l_r, char *l_g, char *l_b)
{

	unsigned int cr;

	// na srodek flary

	lx+=128; ly+=128;

	for (int l1=0;l1<256;l1++)
	{
		for (int l2=0;l2<256;l2++)
		{
			vaddr_r[(800*(l1+ly))+lx+l2]=l_r[(l1<<8)+l2];
			vaddr_g[(800*(l1+ly))+lx+l2]=l_g[(l1<<8)+l2];
			vaddr_b[(800*(l1+ly))+lx+l2]=l_b[(l1<<8)+l2];
		}
	}
}


main (int argc, char **argv)
{
	SDL_Surface *screen;

	FILE *plik;

	plik=fopen("pic.bmp","rb");
	fread(&buffer,1440054,1,plik);
	fclose(plik);

	for (h1=0,h2=0;h1<480000;h1++,h2+=3)
	{
		pic_b[h1]=buffer[1440054-h2];
		pic_g[h1]=buffer[1440055-h2];
		pic_r[h1]=buffer[1440056-h2];
	}

	plik=fopen("picmap.bmp","rb");
	fread(&buffer,1440054,1,plik);
	fclose(plik);

	for (h1=0,h2=0;h1<480000;h1++,h2+=3)
	{
		bump_b[h1]=buffer[1440054-h2];
		bump_g[h1]=buffer[1440055-h2];
		bump_r[h1]=buffer[1440056-h2];
	}

	plik=fopen("flare.bmp","rb");
	fread(&buffer,196662,1,plik);
	fclose(plik);

	for (h1=0,h2=0;h1<65536;h1++,h2+=3)
	{
		light_b[h1]=buffer[196662-h2];
		light_g[h1]=buffer[196661-h2];
		light_r[h1]=buffer[196660-h2];
	}


	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(2);
	}

	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(800, 600, 32,
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

		memcpy(vaddr_r,pic_r,480000);
		memcpy(vaddr_g,pic_g,480000);
		memcpy(vaddr_b,pic_b,480000);

	while ( SDL_PollEvent(NULL) == 0 ) {

		/* Lock the video surface */
		if ( SDL_LockSurface(screen) < 0 ) {
			continue;
		}
		memcpy(vaddr_r,pic_r,480000);
		memcpy(vaddr_g,pic_g,480000);
		memcpy(vaddr_b,pic_b,480000);

//		for (h1=0;h1<480000;h1++)
//		{
//			vaddr_r[h1]=(vaddr_r[h1]+pic_r[h1])>>1;	
//			vaddr_g[h1]=(vaddr_g[h1]+pic_g[h1])>>1;	
//			vaddr_b[h1]=(vaddr_b[h1]+pic_b[h1])>>1;	
//		}

//		x = (int)(W/2 * (1+0.8*cos(a*2 +i*b))  );
//		y = (int)(H/2 * (1+0.8*cos(a*1.4 +1.8*i*b))  );
		
put_light(400*(cos(la*1.2+lb*1.3)),300*(cos(la*1.4+lb*1.6)),(unsigned char*)light_r,(unsigned char*)light_g,(unsigned char*)light_b);
put_light(400*(cos(la*1.3+lb*1.4)),300*(cos(la*1.3+lb*1.5)),(unsigned char*)light_g,(unsigned char*)light_b,(unsigned char*)light_r);
put_light(400*(cos(la*1.4+lb*1.5)),300*(cos(la*1.2+lb*1.4)),(unsigned char*)light_b,(unsigned char*)light_r,(unsigned char*)light_g);
put_light(400*(cos(la*1.5+lb*1.6)),300*(cos(la*1.1+lb*1.3)),(unsigned char*)light_r,(unsigned char*)light_b,(unsigned char*)light_g);

put_light(400*(cos(la*1.6+lb*1.3)),300*(cos(la*1.5+lb*1.6)),(unsigned char*)light_r,(unsigned char*)light_g,(unsigned char*)light_b);
put_light(400*(cos(la*1.7+lb*1.4)),300*(cos(la*1.6+lb*1.5)),(unsigned char*)light_g,(unsigned char*)light_b,(unsigned char*)light_r);
put_light(400*(cos(la*1.8+lb*1.5)),300*(cos(la*1.7+lb*1.4)),(unsigned char*)light_b,(unsigned char*)light_r,(unsigned char*)light_g);
put_light(400*(cos(la*1.9+lb*1.6)),300*(cos(la*1.8+lb*1.3)),(unsigned char*)light_r,(unsigned char*)light_b,(unsigned char*)light_g);


//	put_light(400,300,(unsigned char*)light_b,(unsigned char*)light_g,(unsigned char*)light_r);


		la=la+0.05;
		lb=lb+0.05;

		for (int h1=0;h1<600;h1++)
		{
			for (h2=0;h2<800;h2++)
			{
				((long*)screen->pixels)[800*h1+h2] = (vaddr_r[800*h1+h2]<<screen->format->Rshift)|
				  					 (vaddr_g[800*h1+h2]<<screen->format->Gshift)|
									 (vaddr_b[800*h1+h2]<<screen->format->Bshift);
			}
		}	
		/* Unlock and update the screen */
		SDL_UnlockSurface(screen);
//		SDL_Flip(screen);
	}




	exit(0);
}
