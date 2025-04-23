// EventEVT.h: interface for the CEventEVT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTEVT_H__62C6E761_8B7B_4422_8717_072F23252E78__INCLUDED_)
#define AFX_EVENTEVT_H__62C6E761_8B7B_4422_8717_072F23252E78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprite.h"
#include "Event.h"

class CEventEVT  
{
public:
	CEventEVT(char m_SourceFOX[], char m_cEventName[], IDirect3DDevice8 *pDevice);
	virtual ~CEventEVT();

	// odtwarzaj wg trasy

	virtual void Put(unsigned int m_uiNrKlatki, unsigned int m_uiNrEventu);
	virtual void PutByName(unsigned int uiNrKlatki, char cNazwa[]);

	// tutaj stawiamy na ekran bez odczytywania trasy

	virtual void Put(int m_iXScreen, int m_iYScreen, 
		unsigned int m_uiNrKlatki, unsigned int m_uiNrEventu);	// rysuje na ekranie
	virtual void PutByName(int iXScreen, int iYScreen,
		unsigned int uiNrKlatki, char cNazwa[]);

	virtual void PutScaled(float fScaleX, float fScaleY, 
		unsigned int m_uiNrKlatki, unsigned int m_uiNrEventu);	// rysuje na ekranie

	int		iGetEventFrameCount(int iEventNo);

	void	Convert2EVT2();


private:

	unsigned char		m_ucVersion;		// wersja pliku
	
	unsigned int		m_uiIloscKlatek;	// ile klatek w tym obiekcie 

	CSprite				**pEventFrames;		// tutaj klatki

	unsigned int		m_uiIloscZdarzen;	// ile zdarzen w tym sprite

	CEvent				**pEvent;			// zdarzenia

};

#endif // !defined(AFX_EVENTEVT_H__62C6E761_8B7B_4422_8717_072F23252E78__INCLUDED_)
