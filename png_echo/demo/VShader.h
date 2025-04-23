
#ifndef _VSSHADER_
#define _VSSHADER_

#include <windows.h>
#include <d3d8.h>
#include "Sprite.h"

class CVShader
{

public:
	CVShader();
	virtual ~CVShader();

	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(IDirect3DDevice8 *pDevice, long lTimer);


private:

	bool						m_bActive;

	CSprite						*m_pBackground;
	CSprite						*m_pRamka;
	
	double						dDa, dDb;
	double						fScale;
};

#endif 
