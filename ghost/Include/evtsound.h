// EVTSound.h: interface for the CEVTSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVTSOUND_H__796338EC_6428_46FB_99B0_ACD22E2DCD2E__INCLUDED_)
#define AFX_EVTSOUND_H__796338EC_6428_46FB_99B0_ACD22E2DCD2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sound.h"

// zdarzenie dzwiekowe np "gada", powoduje, ze albo odtwarza jeden dzwiek, a jezeli jest
// ich wiecej (zawsze wiecej niz jeden bo dla jednego jest CSound), to wybieramy typ:
// - po kolei
// - losowo

class __declspec(dllexport) CEVTSound  
{

public:
	CEVTSound(char cFilename[]);
	virtual ~CEVTSound();
	virtual void Play(CAudio *pAudio);				// odtwarza zdarzenie dzwiekowe

private:
	unsigned char	m_ucType;		// 0 losowo, 1 po kolei
	unsigned int	m_uiCount;		// ile dzwiekow

	char			*m_cFilename;	// nazwa pliku *.bnd

	CSound			**m_pSoundList;		// odtwarzane dzwieki
};

#endif // !defined(AFX_EVTSOUND_H__796338EC_6428_46FB_99B0_ACD22E2DCD2E__INCLUDED_)
