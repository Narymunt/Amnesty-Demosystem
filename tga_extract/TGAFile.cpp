// TGAFile.cpp: implementation of the CTGAFile class.
//
//////////////////////////////////////////////////////////////////////

#include "TGAFile.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTGAFile::CTGAFile(char cFilename[])
{

	FILE *pFile;

	m_pBuffer=(unsigned char*)malloc(262144*sizeof(unsigned char));
	m_pHeader=(unsigned char*)malloc(18*sizeof(unsigned char));

	pFile = fopen(cFilename,"rb");
	fread(m_pHeader,18,1,pFile);
	fread(m_pBuffer,262144,1,pFile);
	fclose(pFile);

}

CTGAFile::~CTGAFile()
{
	free(m_pBuffer);
	free(m_pHeader);
}

void CTGAFile::ExtractAlpha(void)
{
	unsigned char *pBMPHeader;
	unsigned char *pAlphaChannel;
	unsigned char end[2];
	FILE *pFileIn, *pFileOut;

	pBMPHeader=(unsigned char*)malloc(1080*sizeof(unsigned char));
	pAlphaChannel=(unsigned char*)malloc(65536*sizeof(unsigned char));

	pFileIn=fopen("header.bin","rb");
	fread(pBMPHeader,1080,1,pFileIn);
	fread(pAlphaChannel,65536,1,pFileIn);
	
	pFileOut=fopen("alpha.bmp","wb");
	fwrite(pBMPHeader,1080,1,pFileOut);

	for (int iTemp=0; iTemp<65536; iTemp++)
	{
		pAlphaChannel[iTemp]=m_pBuffer[(iTemp*4)+3];
	}
	
	fwrite(pAlphaChannel,65536,1,pFileOut);
	fclose(pFileOut);

	fclose(pFileIn);


}