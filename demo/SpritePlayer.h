//////////////////////////////////////////////////////////////////////

#ifndef _SPRITEPLAYER_
#define _SPRITEPLAYER_

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

#define	SPRITE_PLAYER_LIMIT	100

class CSpritePlayer
{
public:
	CSpritePlayer();
	virtual ~CSpritePlayer();

	virtual void Initialize(IDirect3DDevice8 *pDevice);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(IDirect3DDevice8 *pDevice, long lTimer);

private:

	bool		m_bActive[SPRITE_PLAYER_LIMIT];	// ktore animacje sa wczytane, to jest znacznik czy mamy dla tego licznika
										// sprawdzac pozostale parametry


};

#endif 
