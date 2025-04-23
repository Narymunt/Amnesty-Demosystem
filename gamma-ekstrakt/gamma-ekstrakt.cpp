// gamma-ekstrakt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>


FILE *plik;

char header[54];
char obraz[196608];

unsigned char gamma_r[230];
unsigned char gamma_g[230];
unsigned char gamma_b[230];


int _tmain(int argc, _TCHAR* argv[])
{
	plik=fopen("s_gamma.bmp","rb");
	fread(&header,54,1,plik);
	fread(&obraz,196608,1,plik);
	fclose(plik);

	for (int h1=0;h1<230;h1++)
	{
		gamma_r[h1] = obraz[(256*h1)*3];
		gamma_g[h1] = obraz[(256*h1)*3+1];
		gamma_b[h1] = obraz[(256*h1)*3+2];
	}

	plik = fopen("gamma_r.bin","w");
	for (int i=0; i<230;i++) fprintf(plik,"%d,%c%c",gamma_r[i],0x0d,0x0a);
	fclose(plik);

	plik = fopen("gamma_g.bin","w");
	for (int i=0; i<230;i++) fprintf(plik,"%d,%c%c",gamma_g[i],0x0d,0x0a);
	fclose(plik);

	plik = fopen("gamma_b.bin","w");
	for (int i=0; i<230;i++) fprintf(plik,"%d,%c%c",gamma_g[i],0x0d,0x0a);
	fclose(plik);

	return 0;
}

