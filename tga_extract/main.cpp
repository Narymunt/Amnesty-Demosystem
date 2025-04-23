
// tga extractor

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "TGAFile.h"

CTGAFile *pRysunek;

int main(void)
{

	pRysunek = new CTGAFile("screen.tga");
	pRysunek->ExtractAlpha();

	return 0;

}