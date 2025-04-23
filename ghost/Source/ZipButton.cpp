// ZipButton.cpp: implementation of the CZipButton class.
//
//////////////////////////////////////////////////////////////////////

#include <ZipButton.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//=== tworzy przycisk na podstawie trzech bitmap ===

CZipButton::CZipButton(char cFileSystem[], char cPassword[], char clicked_name[], 
		char onButton_name[], char m_pImage_name[],
		IDirect3DDevice8	*pDevice)
{
//	CFileSystem	*pTempFilesystem;	// tylko do wczytania danych

//	pTempFilesystem = new CFileSystem(cFileSystem);

//	pTempFilesystem->Load(clicked_name);
	
//	m_pClicked = new CSprite(255,255,255,255);
//	m_pClicked->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
//		pTempFilesystem->Search(clicked_name),pDevice);
/*		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/

	m_pClicked = new CZipSprite(cFileSystem,clicked_name,cPassword,255,255,255,255,pDevice);


//	pTempFilesystem->Load(onZipButton_name);
	
//	m_pOnZipButton = new CSprite(255,255,255,255);
//	m_pOnZipButton->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
//		pTempFilesystem->Search(onZipButton_name),pDevice);
/*		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/

	m_pOnButton = new CZipSprite(cFileSystem,onButton_name,"nanoczytn1k",255,255,255,255,pDevice);

//	pTempFilesystem->Load(m_pImage_name);

//	m_pImage = new CSprite(255,255,255,255);
//	m_pImage->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
//		pTempFilesystem->Search(m_pImage_name),pDevice);
/*		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/

	m_pImage = new CZipSprite(cFileSystem,m_pImage_name,"nanoczytn1k",255,255,255,255,pDevice);

	// bez hotx i hoty

	m_lXStart = 0;
	m_lYStart = 0;
	m_lXEnd = 0; 
	m_lYEnd = 0;

	m_bFE = false;

}

//=== destruktor zwalnia pamiec po obiektach ===

CZipButton::~CZipButton()
{
	if (m_pClicked!=NULL)
	{
		delete (m_pClicked);
		m_pClicked = NULL;
	}

	if (m_pImage!=NULL) 
	{
		delete (m_pImage);
		m_pImage = NULL;
	}

	if (m_pOnButton!=NULL) 
	{
		delete (m_pOnButton);
		m_pOnButton = NULL;
	}
}

//=== rysuje przycisk w zaleznosci od stanu myszy ===

int CZipButton::Draw(float mousex, float mousey, bool left, bool center, bool right)
{

	// sprawdzamy czy przycisk jest wcisniety na podstawie goracych wspolrzednych
	// jezeli jest to wyswietlamy bitmape w odpowiednim stanie

	if ((mousex>m_lXStart)&&
	   (mousex<m_lXEnd)&&
	   (mousey>m_lYStart)&&
	   (mousey<m_lYEnd))
	{
		if (right)
		{
			m_pClicked->Render();	// przycisk wcisniety prawym klawiszem
			return 4;
		}

		if (center)
		{
			m_pClicked->Render();
			return 3;
		}
		
		if (left)
		{
			m_pClicked->Render();
			return 2;
		}

		m_pOnButton->Render();
		return 1;
	}

	m_pImage->Render();
	return 0;

}

//=== ustawia wspolrzedne przycisku ===

void CZipButton::SetPosition(long lX, long lY)
{
	m_pClicked->SetTranslation((float)lX,(float)lY);
	m_pImage->SetTranslation((float)lX,(float)lY);
	m_pOnButton->SetTranslation((float)lX,(float)lY);
}

//=== ustawia wspolrzedne w obrebie ktorych przycisk jest aktywny

void CZipButton::SetHotCoords(long lXStart, long lYStart, long lXEnd, long lYEnd)
{
	m_lXStart = lXStart;
	m_lYStart = lYStart;
	m_lXEnd = lXEnd;
	m_lYEnd = lYEnd;

	m_pClicked->SetTranslation((float)lXStart,(float)lYStart);
	m_pImage->SetTranslation((float)lXStart,(float)lYStart);
	m_pOnButton->SetTranslation((float)lXStart,(float)lYStart);

}

//=== end ===