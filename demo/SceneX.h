
#ifndef _SCENEX_
#define _SCENEX_

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <d3d8.h>
#include "sprite.h"

class CSceneX
{

public:
	CSceneX();
	virtual ~CSceneX();

	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(IDirect3DDevice8 *pDevice, long lTimer);


private:

	bool						m_bActive;

	CSprite						*m_pBackground;
	CSprite						*m_pRamka;

};

#endif 
