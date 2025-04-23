// Suwak.cpp: implementation of the CSuwak class.
//
//////////////////////////////////////////////////////////////////////

#include <Suwak.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//=== tworzy przycisk na podstawie trzech bitmap ===

CSuwak::CSuwak(char cFileSystem[], char cImage[], 
		int iX, int iStartY, int iEndY,
		IDirect3DDevice8	*pDevice)
{
	CFileSystem	*pTempFilesystem;	// tylko do wczytania danych

	pTempFilesystem = new CFileSystem(cFileSystem);

	pTempFilesystem->Load(cImage);
	
	m_pImage = new CSprite(255,255,255,255);
	m_pImage->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(cImage),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);

	// bez hotx i hoty

	m_iYPosition = iStartY+255;
	m_iXPosition = iX;

	m_iStartY = iStartY;
	m_iEndY = iEndY;

	

}

//=== destruktor zwalnia pamiec po obiektach ===

CSuwak::~CSuwak()
{
	if (m_pImage!=NULL)
	{
//		delete (m_pImage);
		m_pImage = NULL;
	}

}

//=== rysuje przycisk w zaleznosci od stanu myszy ===

int CSuwak::Draw(float mousex, float mousey, bool left, bool center, bool right)
{

	if (mousex>(m_iXPosition-8) && mousex<(m_iXPosition+24) && mousey>m_iStartY && mousey<m_iEndY && left)
		m_iYPosition = (int)mousey;
	
	m_pImage->SetTranslation((float)m_iXPosition-8,(float)m_iYPosition);
	m_pImage->Render();

	return 0;
}


int CSuwak::iGetValue(void)
{
	return m_iYPosition - m_iStartY;
}

void CSuwak::SetValue(int iValue)
{
	m_iYPosition = m_iStartY+iValue;
}
//=== end ===