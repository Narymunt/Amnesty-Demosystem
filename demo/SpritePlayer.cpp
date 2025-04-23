
#include "SpritePlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpritePlayer::CSpritePlayer()
{
	for (int i=0; i<SPRITE_PLAYER_LIMIT; i++)	// czyszczenie tablic
	{
		m_bActive[i] = false;
	}

}

CSpritePlayer::~CSpritePlayer()
{
	for (int i=0; i<SPRITE_PLAYER_LIMIT; i++)
	{
		if (m_bActive[i])
		{
			
			m_bActive[i] = false;
		}
	}
}

void CSpritePlayer::DeInitialize(void)
{
	for (int i=0; i<SPRITE_PLAYER_LIMIT; i++)
	{
		if (m_bActive[i])
		{
			
			m_bActive[i] = false;
		}
	}
}

void CSpritePlayer::Initialize(IDirect3DDevice8 *pDevice)
{

}

int CSpritePlayer::DrawScene(IDirect3DDevice8 *pDevice, long lTimer)
{
	return 0; 
}