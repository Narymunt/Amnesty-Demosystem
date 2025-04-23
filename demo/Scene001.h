
#ifndef _SCENE001_
#define _SCENE001_

#include <windows.h>
#include <d3d8.h>
#include <ghost.h>

class CScene001
{

public:
	CScene001();
	virtual ~CScene001();

	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(IDirect3DDevice8 *pDevice, long lTimer);


private:

	bool						m_bActive;

	CSprite						*m_pBackground;
	CSprite						*m_pRamka;

	CFileSystem					*m_pFile;

	

};

#endif 
