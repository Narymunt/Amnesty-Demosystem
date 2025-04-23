
#ifndef _INTRO_
#define _INTRO_

#include <windows.h>
#include <d3d8.h>
#include <ghost.h>

class CIntro  
{

public:
	CIntro();
	virtual ~CIntro();

	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(long lTimer);

private:

	float	dDa, dDb;	// dla petli

	bool	m_bActive;

	CSprite	*m_pBackground;

	CSprite	*m_pNapis1;

	CSprite *m_pFlare001;

	CSprite *m_pPengo;

	CFileSystem	*m_pFile;

};

#endif 
