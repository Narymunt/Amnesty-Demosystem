// BitmapMask.cpp: implementation of the CBitmapMask class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "BitmapMask.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmapMask::CBitmapMask()
{

}

CBitmapMask::~CBitmapMask()
{

	if (mBitmapMaskData!=NULL) free(mBitmapMaskData);

}

// od razu zaalokuj pamiec i wczytaj dane z pliku 

CBitmapMask::CBitmapMask(unsigned long size, char filename[])
{
	
	FILE *bmp_file;

	mBitmapMaskData = (unsigned char *) malloc (size);

	if ((bmp_file=fopen(filename,"rb"))==NULL) MessageBox(0,"error","error",MB_OK);
	fread(mBitmapMaskData,size,1,bmp_file);
	fclose(bmp_file);

}

// od razu zaalokuj pamiec

CBitmapMask::CBitmapMask(unsigned long size)
{
	mBitmapMaskData = (unsigned char *) malloc (size);
}
