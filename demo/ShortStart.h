
#ifndef _SHORTSTART_
#define _SHORTSTART_

// po prostu wyswietla dwa rysunki

#include <windows.h>
#include <d3d8.h>
#include <ghost.h>

class CShortStart  
{

public:
	CShortStart();
	virtual ~CShortStart();

	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(long lTimer);

private:

	bool	m_bActive;

	CSprite	*m_pBackground01;
	CSprite *m_pBackground02;
	CSprite *m_pBackground03;
	CSprite *m_pBackground04;

	CFileSystem	*m_pFile;

};

#endif 
