// Flare.h: interface for the CFlare class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FLARE_
#define _FLARE_

#include <windows.h>
#include <d3dx8.h>
#include "Sprite.h"

class CFlare  
{
public:
	CFlare(IDirect3DDevice8 *pDevice,char cFilename[]);
	virtual ~CFlare();
	virtual void Render(int iX, int iY, float fZ);

protected:

		int			m_iX;
		int			m_iY;
		float		m_fZ;
	
		CSprite		*m_pFlare;

};

#endif 