// Bitmap.cpp: implementation of the CBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "Bitmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmap::CBitmap()
{

	

}

CBitmap::~CBitmap()
{
	if (mBitmapDataR!=NULL) free(mBitmapDataR);
	if (mBitmapDataG!=NULL) free(mBitmapDataG);
	if (mBitmapDataB!=NULL) free(mBitmapDataB);
}

CBitmap::CBitmap(unsigned long size)
{
	mBitmapDataR = (unsigned char*)malloc(size);
	mBitmapDataG = (unsigned char*)malloc(size);
	mBitmapDataB = (unsigned char*)malloc(size);
}

CBitmap::CBitmap(unsigned long size, char filename[])
{
	unsigned char *mTemp;
	FILE *bmp_file;
	
	unsigned long b1, b2;

	mTemp = (unsigned char *)malloc(54+(size*3));

	mBitmapDataR = (unsigned char*)malloc(size);
	mBitmapDataG = (unsigned char*)malloc(size);
	mBitmapDataB = (unsigned char*)malloc(size);

	if ((bmp_file=fopen(filename,"rb"))==NULL) MessageBox(0,"error","error",MB_OK);
	fread(mTemp,54+(size*3),1,bmp_file);
	fclose(bmp_file);

	for (b1=0, b2=0; b1<size; b1++, b2+=3)
	{
		mBitmapDataB[b1]=mTemp[51+(size*3)-b2];
		mBitmapDataG[b1]=mTemp[52+(size*3)-b2];
		mBitmapDataR[b1]=mTemp[53+(size*3)-b2];
	}

}


