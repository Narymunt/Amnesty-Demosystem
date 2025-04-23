// EVTSound.cpp: implementation of the CEVTSound class.
//
//////////////////////////////////////////////////////////////////////

#include "EVTSound.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEVTSound::CEVTSound(char cFilename[])
{
	// wczytanie pliku .bnd
	// - naglowek

	// - typ 

	// - ile dzwiekow, alokacja 

	m_pSoundList = new CSound*[1];

	// nazwy plikow z dzwiekami

	for (unsigned int h1=0; h1<m_uiCount; h1++)
	{
//		m_pSoundList[h1] = new CSound(
	}
}

CEVTSound::~CEVTSound()
{

	for (unsigned int h1=0; h1<m_uiCount; h1++)
	{
//		if (m_pSoundList[h1]!=NULL) 
//			delete m_pSoundList[h1];
	}

	if (m_pSoundList!=NULL)
	{
//		delete[] m_pSoundList;
//		m_pSoundList = NULL;
	}

}

void CEVTSound::Play(CAudio *pAudio)
{
	DWORD g_Time = timeGetTime();

	if (m_ucType==0) // losowo
	{
		m_pSoundList[g_Time%m_uiCount]->Play(pAudio,0,0,0);
	}

	if (m_ucType==1)	// po kolei
	{

	}
}