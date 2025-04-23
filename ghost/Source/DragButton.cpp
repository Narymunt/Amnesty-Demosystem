// DragButton.cpp: implementation of the CDragButton class.
//
//////////////////////////////////////////////////////////////////////

#include <DragButton.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//=== tworzy przycisk na podstawie czterech bitmap ===

CDragButton::CDragButton(char cFileSystem[], char clicked_name[], 
		char onDragButton_name[], char m_pImage_name[], char onmove_name[],
		IDirect3DDevice8	*pDevice)
{
	CFileSystem	*pTempFilesystem;	// tylko do wczytania danych

	m_pDevice = pDevice;
	strcpy(m_cFilesystem,cFileSystem);

	pTempFilesystem = new CFileSystem(cFileSystem);

	pTempFilesystem->Load(clicked_name);

	m_pClicked = NULL;
	m_pOnDragButton = NULL;

/*	m_pClicked = new CSprite(255,255,255,255);
	m_pClicked->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(clicked_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/
	pTempFilesystem->Load(onDragButton_name);
	
	m_pOnDragButton = new CSprite(255,255,255,255);
	m_pOnDragButton->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(onDragButton_name),pDevice);
/*	,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/
	m_pOnDragButton->InitDesc();
	m_iOnDragButtonSizeX = m_pOnDragButton->iGetXSize();
	m_iOnDragButtonSizeY = m_pOnDragButton->iGetYSize();
	

	pTempFilesystem->Load(m_pImage_name);

	m_pImage = new CSprite(255,255,255,255);
	m_pImage->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(m_pImage_name),pDevice);
/*		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/
	m_pImage->InitDesc();
	m_iImageSizeX = m_pImage->iGetXSize();
	m_iImageSizeY = m_pImage->iGetYSize();

	strcpy(m_cOnMoveFilename,onmove_name);

/*	pTempFilesystem->Load(onmove_name);
		m_pOnMove = new CSprite(255,255,255,255);
		m_pOnMove->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
			pTempFilesystem->Search(m_cOnMoveFilename),m_pDevice,
			D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
			0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
		m_pOnMove->InitDesc();
		m_iOnMoveSizeX = m_pOnMove->iGetXSize();
		m_iOnMoveSizeY = m_pOnMove->iGetYSize();
*/
	m_pOnMove = NULL;
	strcpy(m_cOnMoveFilename,onmove_name);

	m_pDropped = NULL;
	m_iDroppedSizeX = 0;
	m_iDroppedSizeY = 0;

	// bez hotx i hoty

	m_lXStart = 0;
	m_lYStart = 0;
	m_lXEnd = 0; 
	m_lYEnd = 0;

	m_bFE = false;
	m_bPressed = false;
}

//=== tworzy przycisk na podstawie czterech bitmap, zmienia ksztalt po przesunieciu ===

CDragButton::CDragButton(char cFileSystem[], char clicked_name[], 
		char onDragButton_name[], char m_pImage_name[], char onmove_name[],
		char dropped_name[],IDirect3DDevice8	*pDevice)
{
	CFileSystem	*pTempFilesystem;	// tylko do wczytania danych

	m_pDevice = pDevice;
	strcpy(m_cFilesystem,cFileSystem);

	pTempFilesystem = new CFileSystem(cFileSystem);

	pTempFilesystem->Load(clicked_name);
	
/*	m_pClicked = new CSprite(255,255,255,255);
	m_pClicked->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(clicked_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/

	pTempFilesystem->Load(onDragButton_name);
	
	m_pOnDragButton = new CSprite(255,255,255,255);
	m_pOnDragButton->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(onDragButton_name),pDevice);
/*		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/
	m_pOnDragButton->InitDesc();
	m_iOnDragButtonSizeX = m_pOnDragButton->iGetXSize();
	m_iOnDragButtonSizeY = m_pOnDragButton->iGetYSize();

	pTempFilesystem->Load(m_pImage_name);

	m_pImage = new CSprite(255,255,255,255);
	m_pImage->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(m_pImage_name),pDevice);
/*		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/
	m_pImage->InitDesc();
	m_iImageSizeX = m_pImage->iGetXSize();
	m_iImageSizeY = m_pImage->iGetYSize();

	m_pOnMove = NULL;
	m_iOnMoveSizeX = 0;
	m_iOnMoveSizeY = 0;
	strcpy(m_cOnMoveFilename,onmove_name);

	m_pDropped = NULL;
	strcpy(m_cDroppedFilename,dropped_name);

	pTempFilesystem->Load(dropped_name);

	m_pDropped = new CSprite(255,255,255,255);
	m_pDropped->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(dropped_name),pDevice);
/*		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/

	m_pDropped->InitDesc();
	m_iDroppedSizeX = m_pDropped->iGetXSize();
	m_iDroppedSizeY = m_pDropped->iGetYSize();

	// bez hotx i hoty

	m_lXStart = 0;
	m_lYStart = 0;
	m_lXEnd = 0; 
	m_lYEnd = 0;

	m_bFE = false;

}


//=== destruktor zwalnia pamiec po obiektach ===

CDragButton::~CDragButton()
{
	if (m_pClicked!=NULL)
	{
//		delete (m_pClicked);
		m_pClicked = NULL;
	}

	if (m_pImage!=NULL) 
	{
//		delete (m_pImage);
		m_pImage = NULL;
	}

	if (m_pOnDragButton!=NULL) 
	{
//		delete (m_pOnDragButton);
		m_pOnDragButton = NULL;
	}

	if (m_pOnMove!=NULL)
	{
//		delete (m_pOnMove);
		m_pOnMove = NULL;
	}

}

//=== rysuje przycisk w zaleznosci od stanu myszy ===

int CDragButton::Draw(float mousex, float mousey, bool left, bool center, bool right)
{	

	CFileSystem	*pTempFilesystem;	// tylko do wczytania danych

// przycisk jest ciagniety za myszka

	if (m_cState>2 && m_pOnMove == NULL)
	{
		pTempFilesystem = new CFileSystem(m_cFilesystem);

		pTempFilesystem->Load(m_cOnMoveFilename);

		m_pOnMove = new CSprite(255,255,255,255);
		m_pOnMove->InitializeTGAinMemory((unsigned int*)pTempFilesystem->pDataBuffer,
			pTempFilesystem->Search(m_cOnMoveFilename),m_pDevice);
/*			D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
			0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
*/
		m_pOnMove->InitDesc();
		m_iOnMoveSizeX = m_pOnMove->iGetXSize();
		m_iOnMoveSizeY = m_pOnMove->iGetYSize();
		m_lXAdd = -m_iOnMoveSizeX/2;
		m_lYAdd = -m_iOnMoveSizeY/2;
	}

	if (m_cState==3)
	{
		m_lXCurrent = (long)mousex;	// zapamietaj aktualne pozycje
		m_lYCurrent = (long)mousey;
		m_pOnMove->SetTranslation(mousex+m_lXAdd,mousey+m_lYAdd);
		SetRotationCenter((float)-m_lXAdd,(float)-m_lYAdd);	// dla obrotu tez na srodku
		m_pOnMove->Render();
		return 5;
	}

	if (m_cState==4)
	{
		m_pOnMove->SetTranslation((float)(m_lXCurrent+m_lXAdd),(float)(m_lYCurrent+m_lYAdd));
		SetRotationCenter((float)-m_lXAdd,(float)-m_lYAdd);	// dla obrotu tez na srodku
	//	m_pOnMove->Render();
		return 6;
	}

	// sprawdzamy czy przycisk jest wcisniety na podstawie goracych wspolrzednych
	// jezeli jest to wyswietlamy bitmape w odpowiednim stanie

if (m_cState<3) // obiekt nie jest przeciagany
{
	if ((mousex>m_lXStart)&&
	   (mousex<m_lXEnd)&&
	   (mousey>m_lYStart)&&
	   (mousey<m_lYEnd))
	{
		if (right)
		{
			m_cState = 2;
			m_pImage->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
			m_pImage->Render();	// przycisk wcisniety prawym klawiszem
			return 4;
		}

		if (center)
		{
			m_cState = 2; // kursor myszy nad obiektem, przycisk wcisniety
			m_pImage->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
			m_pImage->Render();
			return 3;
		}
		
		if (left)
		{
			m_cState=2; // kursor myszy nad obiektem, przycisk wcisniety
			m_pImage->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
			m_pImage->Render();
			return 2;
		}

		m_cState=1; // kursor myszy nad obiektem
		m_pImage->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
		m_pImage->Render();
		return 1;
	}

	m_cState=0; // kursor myszy poza obiektem
	m_pImage->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
	m_pImage->Render();
	return 0;
}
	return 0;
}

//=== ustawianie wspolrzednych

void CDragButton::SetDefaultPosition(long lX, long lY)
{
	m_lXDefault = lX;
	m_lYDefault = lY;
}

void CDragButton::SetCurrentPosition(long lX, long lY)
{
	m_lXCurrent = lX;
	m_lYCurrent = lY;
}

//=== pobieranie wspolrzednych

long CDragButton::lGetDefaultPositionX(void)
{
	return m_lXDefault;
}

long CDragButton::lGetDefaultPositionY(void)
{
	return m_lYDefault;
}

long CDragButton::lGetCurrentPositionX(void)
{
	return m_lXCurrent;
}

long CDragButton::lGetCurrentPositionY(void)
{
	return m_lYCurrent;
}


//=== ustawia wspolrzedne przycisku ===

void CDragButton::SetPosition(long lX, long lY)
{
//	m_pClicked->SetTranslation((float)lX,(float)lY);
	m_pImage->SetTranslation((float)lX,(float)lY);
//	m_pOnDragButton->SetTranslation((float)lX,(float)lY);
}

//=== ustawia wspolrzedne w obrebie ktorych przycisk jest aktywny

void CDragButton::SetHotCoords(long lXStart, long lYStart, long lXEnd, long lYEnd)
{
	m_lXStart = lXStart;
	m_lYStart = lYStart;
	m_lXEnd = lXEnd;
	m_lYEnd = lYEnd;

//	m_pClicked->SetTranslation((float)lXStart,(float)lYStart);
	m_pImage->SetTranslation((float)lXStart,(float)lYStart);
//	m_pOnDragButton->SetTranslation((float)lXStart,(float)lYStart);

}

//=== gdzie mozna upuscic przycisk

void CDragButton::SetDropArea(long x1, long y1, long x2, long y2)	
{
	m_lXDropStart = x1;
	m_lYDropStart = y1;
	m_lXDropEnd = x2;
	m_lYDropEnd = y2;
}

bool CDragButton::bIsDrop(long x, long y)
{
	if (x>m_lXDropStart && x<m_lXDropEnd &&
		y>m_lYDropStart && y<m_lYDropEnd) return true;
	else return false;
}


//=== gdzie mozna zwrocic przycisk

void CDragButton::SetReturnArea(long x1, long y1, long x2, long y2)
{
	m_lXReturnStart = x1;
	m_lYReturnStart = y1;
	m_lXReturnEnd = x2;
	m_lYReturnEnd = y2;
}

// mouse state 

void CDragButton::SetState(char cState)
{
	m_cState = cState;
}

char CDragButton::cGetState(void)
{
	return m_cState;
}

// czy obiekt wraca do pola macierzystego

void CDragButton::SetMovable(bool bMove)
{
	m_bMovable = bMove;
}

bool CDragButton::bGetMovable(void)
{
	return m_bMovable;
}

// wartosci centrujace

void CDragButton::SetAddPosition(long x, long y)
{
	m_lXAdd = x;
	m_lYAdd = y;
}

long CDragButton::lGetAddXPosition(void)
{
	return m_lXAdd;
}

long CDragButton::lGetAddYPosition(void)
{
	return m_lYAdd;
}

bool CDragButton::bIsReturn(long x, long y)	// true - w pozycji zwrotu, false - poza nia
{
	if (x>m_lXReturnStart && x<m_lXReturnEnd &&
		y>m_lYReturnStart && y<m_lYReturnEnd) return true;
	else return false;
}

//=== wartosci jednostkowe

void CDragButton::SetFE(bool bState)
{
	m_bFE = bState;
}

bool CDragButton::bGetFE(void)
{
	return m_bFE;
}

void CDragButton::SetPress(bool bState)
{
	m_bPressed = bState;
}
	
bool CDragButton::bGetPress(void)
{
	return m_bPressed;
}

//=== ustaw rotacje

void CDragButton::SetRotation(float newRotation)
{
	m_pOnMove->SetRotation(newRotation);
}
	
//=== pobierz rotacje

float CDragButton::fGetRotation(void)
{
	return m_pOnMove->GetRotation();
}

//=== dodaj lub odejmij rotacje

void CDragButton::AddRotation(float addValue)
{
	m_pOnMove->AddRotation(addValue);
}

//=== ustaw punkt rotacji w obiekcie

void CDragButton::SetRotationCenter(float x, float y)
{
	m_pOnMove->SetRotationCenter(x,y);
}

//=== ustaw skalê obiektu

void CDragButton::SetScale(float fValueX, float fValueY)
{
	m_pOnMove->SetScale(fValueX,fValueY);
}

//=== ustaw skalê obiektu - stretching

void CDragButton::SetScaleX(float fValueX)
{
	m_pOnMove->SetScaleX(fValueX);
}

//=== ustaw skalê obiektu - stretching

void CDragButton::SetScaleY(float fValueY)
{
	m_pOnMove->SetScaleY(fValueY);
}

//=== pobierz skalê X

float CDragButton::fGetScaleX(void)
{
	return m_pOnMove->GetScaleX();
}

//=== pobierz skalê Y
	
float CDragButton::fGetScaleY(void)
{
	return m_pOnMove->GetScaleY();
}

//=== dodaj skalê

void CDragButton::AddScale(float fValueX, float fValueY)
{
	m_pOnMove->AddScale(fValueX,fValueY);
}

//=== dodaj skalê X

void CDragButton::AddScaleX(float fValueX)
{
	m_pOnMove->AddScaleX(fValueX);
}

//=== dodaj skalê Y

void CDragButton::AddScaleY(float fValueY)
{
	m_pOnMove->AddScaleY(fValueY);
}


int CDragButton::iGetClickedSizeX(void)
{
	return m_iClickedSizeX;
}

int CDragButton::iGetClickedSizeY(void)
{
	return m_iClickedSizeY;
}

int CDragButton::iGetDroppedSizeX(void)
{
	return m_iDroppedSizeX;
}

int CDragButton::iGetDroppedSizeY(void)
{
	return m_iDroppedSizeY;
}

int CDragButton::iGetImageSizeX(void)
{
	return m_iImageSizeX;
}

int CDragButton::iGetImageSizeY(void)
{
	return m_iImageSizeY;
}

int CDragButton::iGetOnDragButtonSizeX(void)
{
	return m_iOnDragButtonSizeX;
}

int CDragButton::iGetOnDragButtonSizeY(void)
{
	return m_iOnDragButtonSizeY;
}

int CDragButton::iGetOnMoveSizeX(void)
{
	return m_iOnMoveSizeX;
}

int CDragButton::iGetOnMoveSizeY(void)
{
	return m_iOnMoveSizeY;
}

//=== end ===