// Ytable.cpp: implementation of the CYtable class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Ytable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYtable::CYtable()
{

}

CYtable::~CYtable()
{
	if (mLine!=NULL) free(mLine);		
}

CYtable::CYtable(unsigned long size, unsigned long resolution)
{

	unsigned long t1;

	if ( (mLine=(unsigned long*)malloc (size))==NULL) MessageBox(0,"error","Za duza Ytable",MB_OK);

	for (t1=0;t1<size;t1++)
	{
		mLine[t1]= resolution * t1;		// 1*800, 2*800, 3*800
	}

}
