// FlareTrack.cpp: implementation of the CFlareTrack class.
//
//////////////////////////////////////////////////////////////////////

#include "FlareTrack.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlareTrack::CFlareTrack(IDirect3DDevice8 *pDevice, FILE *pFile, char cFlareFilename[])
{

	unsigned long ulFlaresPerFrame, ulIDFlare;

	int iXFlare, iYFlare;
	float fZFlare;

	fread(&m_ulFramesCount,1,sizeof(unsigned long), pFile);	// ile klatek

	m_ppFlaresFrames = new CFlareFrame*[m_ulFramesCount];

	for (unsigned long ulTemp=0; ulTemp<m_ulFramesCount; ulTemp++)
	{
		fread(&ulFlaresPerFrame,1,sizeof(unsigned long),pFile);	// numer klatki
		fread(&ulFlaresPerFrame,1,sizeof(unsigned long),pFile); // ile flarek
 
		m_ppFlaresFrames[ulTemp] = new CFlareFrame(ulFlaresPerFrame);

		for (unsigned long ulTemp2=0; ulTemp2<ulFlaresPerFrame; ulTemp2++)
		{
			fread(&iXFlare,1,sizeof(int),pFile);
			fread(&iYFlare,1,sizeof(int),pFile);
			fread(&fZFlare,1,sizeof(float),pFile);
			fread(&ulIDFlare,1,sizeof(unsigned long), pFile);

			m_ppFlaresFrames[ulTemp]->AddFlare(iXFlare,iYFlare,fZFlare, ulIDFlare, ulTemp2);
		}

	}

	m_pFlare = new CFlare(pDevice,cFlareFilename);
}

CFlareTrack::~CFlareTrack()
{
	
	for (unsigned long ulTemp=0; ulTemp<m_ulFramesCount; ulTemp++)
	{
		delete[] m_ppFlaresFrames[ulTemp];
	}

	delete m_ppFlaresFrames;

	delete m_pFlare;

}

void CFlareTrack::Render(unsigned long ulFrameNo, long lTimer)
{

	unsigned long ulTemp;

	// dodatkowe sprawdzenie - jezeli nie ma trasy flarek to po prostu przeskakujemy

	if (m_ulFramesCount!=0)
	{

		if (ulFrameNo>=m_ulFramesCount)
		{
			ulFrameNo=m_ulFramesCount-1;
		}

		for (ulTemp=0; ulTemp<m_ppFlaresFrames[ulFrameNo]->ulGetFlaresCount(); ulTemp++)
		{
			m_pFlare->Render(m_ppFlaresFrames[ulFrameNo]->iGetFlareX(ulTemp), 
					 m_ppFlaresFrames[ulFrameNo]->iGetFlareY(ulTemp), 
					 m_ppFlaresFrames[ulFrameNo]->fGetFlareZ(ulTemp),
					 lTimer);
		}

	}

}
