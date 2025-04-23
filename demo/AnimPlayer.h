// LoadScreen.h: interface for the CLoadScreen class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ANIMPLAYER_
#define _ANIMPLAYER_

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <ctype.h>
#include "Sprite.h"
#include "Event.h"
#include "EventEVT.h"

#define	EVT_LIMIT	100

// przykladowy skrypt, bez znaku nawiasow
//<ANIMATION>
//data/gazeta.bin
//0 (numer eventu)
//0 2000 2100	(load start end)
//24 (fps)
//10 10 2 2 (x y scaleX scaleY)
//NO_FRAME

class CAnimPlayer
{
public:
	CAnimPlayer();
	virtual ~CAnimPlayer();

	virtual void Initialize(IDirect3DDevice8 *pDevice);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(IDirect3DDevice8 *pDevice, long lTimer);

private:

	bool		m_bActive[EVT_LIMIT];	// ktore animacje sa wczytane, to jest znacznik czy mamy dla tego licznika
										// sprawdzac pozostale parametry

	char		m_cAnimationType[EVT_LIMIT];		// typ animacji :
								// LOOP 0 powtarza sie
								// LAST_FRAME 1 zatrzymuje sie na ostatniej klatce
								// NO_FRAME 2 gdy konczy sie timer to animacja nie jest wyswietlana

	long		m_lTimerLoad[EVT_LIMIT];	// kiedy animacja ma byc wczytana
	long		m_lTimerStart[EVT_LIMIT];	// kiedy animacja ma sie odpalic
	long		m_lTimerEnd[EVT_LIMIT];	// kiedy animacja ma sie zakonczyc

	float		m_fFPS[EVT_LIMIT];			// frame rate, ile klatek na sekunde

	char		m_cAnimFilename[EVT_LIMIT][256];	// nazwy plikow dla .bin 
	char		m_cEventNo[EVT_LIMIT];	// numer zdarzenia

	long		m_lPositionX[EVT_LIMIT];	// gdzie na ekranie ma byc postawiona animacja
	long		m_lPositionY[EVT_LIMIT];

	int			m_iScaleX[EVT_LIMIT];	// skala obrazu
	int			m_iScaleY[EVT_LIMIT];

	CEventEVT	*m_pPlayer[EVT_LIMIT];	// tutaj animacje

};

#endif 
