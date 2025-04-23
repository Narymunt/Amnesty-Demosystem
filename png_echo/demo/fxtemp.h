// FXTemp.h: interface for the CFXTemp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FXTEMP_
#define _FXTEMP_

#include <windows.h>
#include <d3d8.h>
#include "Sprite.h"

class CFXTemp  
{
public:
	CFXTemp();
	virtual ~CFXTemp();

	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(long lTimer);

private:

	bool		m_bActive;
	CSprite		*m_pBackground;
	CSprite		*m_pFadeScreenIn;
	CSprite		*m_pFadeScreenOut;

	CSprite		*m_pJihadLogo;
	CSprite		*m_pFremenLogo;
	CSprite		*m_pGinnLogo;
	CSprite		*m_pMimeLogo;
	CSprite		*m_pSzudiLogo;

};

#endif 
