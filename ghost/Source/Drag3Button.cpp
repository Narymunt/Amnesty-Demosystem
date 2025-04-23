// Drag3Button.cpp: implementation of the CDrag3Button class.
//
//////////////////////////////////////////////////////////////////////

#include <Drag3Button.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//=== tworzy przycisk na podstawie czterech bitmap ===

CDrag3Button::CDrag3Button(char cFileSystem[], char clicked_name[], 
		char onDrag3Button_name[], char m_pImage_name[], char onmove_name[],
		IDirect3DDevice8	*pDevice)
{
	CFileSystem	*pTempFilesystem;	// tylko do wczytania danych

	m_pDevice = pDevice;
	strcpy(m_cFilesystem,cFileSystem);

	pTempFilesystem = new CFileSystem(cFileSystem);

	pTempFilesystem->Load(clicked_name);
	
	m_pClicked = new CSprite(255,255,255,255);
	m_pClicked->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(clicked_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);

	pTempFilesystem->Load(onDrag3Button_name);
	
	m_pOnDrag3Button = new CSprite(255,255,255,255);
	m_pOnDrag3Button->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(onDrag3Button_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);

	pTempFilesystem->Load(m_pImage_name);

	m_pImage = new CSprite(255,255,255,255);
	m_pImage->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(m_pImage_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);

	pTempFilesystem->Load(onmove_name);

	m_pOnMove = NULL;
	strcpy(m_cOnMoveFilename,onmove_name);

	m_pDropped = NULL;


	// bez hotx i hoty

	m_lXStart = 0;
	m_lYStart = 0;
	m_lXEnd = 0; 
	m_lYEnd = 0;

	m_bFE = false;
	m_bPressed = false;
}

//=== tworzy przycisk na podstawie czterech bitmap, zmienia ksztalt po przesunieciu ===

CDrag3Button::CDrag3Button(char cFileSystem[], char clicked_name[], 
		char onDrag3Button_name[], char m_pImage_name[], char onmove_name[],
		char dropped_name[],IDirect3DDevice8	*pDevice)
{
	CFileSystem	*pTempFilesystem;	// tylko do wczytania danych

	m_pDevice = pDevice;
	strcpy(m_cFilesystem,cFileSystem);

	pTempFilesystem = new CFileSystem(cFileSystem);

	pTempFilesystem->Load(clicked_name);
	
	m_pClicked = new CSprite(255,255,255,255);
	m_pClicked->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(clicked_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);

	pTempFilesystem->Load(onDrag3Button_name);
	
	m_pOnDrag3Button = new CSprite(255,255,255,255);
	m_pOnDrag3Button->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(onDrag3Button_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);

	pTempFilesystem->Load(m_pImage_name);

	m_pImage = new CSprite(255,255,255,255);
	m_pImage->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(m_pImage_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);

	m_pOnMove = NULL;
	strcpy(m_cOnMoveFilename,onmove_name);

	m_pDropped = NULL;
	strcpy(m_cDroppedFilename,dropped_name);

	pTempFilesystem->Load(dropped_name);

	m_pDropped = new CSprite(255,255,255,255);
	m_pDropped->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
		pTempFilesystem->Search(dropped_name),pDevice,
		D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);

	// bez hotx i hoty

	m_lXStart = 0;
	m_lYStart = 0;
	m_lXEnd = 0; 
	m_lYEnd = 0;

	m_bFE = false;

}


//=== destruktor zwalnia pamiec po obiektach ===

CDrag3Button::~CDrag3Button()
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

	if (m_pOnDrag3Button!=NULL) 
	{
//		delete (m_pOnDrag3Button);
		m_pOnDrag3Button = NULL;
	}

	if (m_pOnMove!=NULL)
	{
//		delete (m_pOnMove);
		m_pOnMove = NULL;
	}

}

//=== rysuje przycisk w zaleznosci od stanu myszy ===

int CDrag3Button::Draw(float mousex, float mousey, bool left, bool center, bool right)
{	

	CFileSystem	*pTempFilesystem;	// tylko do wczytania danych

// przycisk jest ciagniety za myszka

	if (m_cState>2 && m_pOnMove == NULL)
	{
		pTempFilesystem = new CFileSystem(m_cFilesystem);

		pTempFilesystem->Load(m_cOnMoveFilename);

		m_pOnMove = new CSprite(255,255,255,255);
		m_pOnMove->InitializeTGAinMemoryEx((unsigned int*)pTempFilesystem->pDataBuffer,
			pTempFilesystem->Search(m_cOnMoveFilename),m_pDevice,
			D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
			0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,NULL,NULL);
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
			m_pClicked->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
			m_pClicked->Render();	// przycisk wcisniety prawym klawiszem
			return 4;
		}

		if (center)
		{
			m_cState = 2; // kursor myszy nad obiektem, przycisk wcisniety
			m_pClicked->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
			m_pClicked->Render();
			return 3;
		}
		
		if (left)
		{
			m_cState=2; // kursor myszy nad obiektem, przycisk wcisniety
			m_pClicked->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
			m_pClicked->Render();
			return 2;
		}

		m_cState=1; // kursor myszy nad obiektem
		m_pOnDrag3Button->SetTranslation((float)m_lXDefault,(float)m_lYDefault);
		m_pOnDrag3Button->Render();
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

void CDrag3Button::SetDefaultPosition(long lX, long lY)
{
	m_lXDefault = lX;
	m_lYDefault = lY;
}

void CDrag3Button::SetCurrentPosition(long lX, long lY)
{
	m_lXCurrent = lX;
	m_lYCurrent = lY;
}

//=== pobieranie wspolrzednych

long CDrag3Button::lGetDefaultPositionX(void)
{
	return m_lXDefault;
}

long CDrag3Button::lGetDefaultPositionY(void)
{
	return m_lYDefault;
}

long CDrag3Button::lGetCurrentPositionX(void)
{
	return m_lXCurrent;
}

long CDrag3Button::lGetCurrentPositionY(void)
{
	return m_lYCurrent;
}


//=== ustawia wspolrzedne przycisku ===

void CDrag3Button::SetPosition(long lX, long lY)
{
	m_pClicked->SetTranslation((float)lX,(float)lY);
	m_pImage->SetTranslation((float)lX,(float)lY);
	m_pOnDrag3Button->SetTranslation((float)lX,(float)lY);
}

//=== ustawia wspolrzedne w obrebie ktorych przycisk jest aktywny

void CDrag3Button::SetHotCoords(long lXStart, long lYStart, long lXEnd, long lYEnd)
{
	m_lXStart = lXStart;
	m_lYStart = lYStart;
	m_lXEnd = lXEnd;
	m_lYEnd = lYEnd;

	m_pClicked->SetTranslation((float)lXStart,(float)lYStart);
	m_pImage->SetTranslation((float)lXStart,(float)lYStart);
	m_pOnDrag3Button->SetTranslation((float)lXStart,(float)lYStart);

}

//=== gdzie mozna upuscic przycisk

void CDrag3Button::SetDropArea(long x1, long y1, long x2, long y2)	
{
	m_lXDropStart = x1;
	m_lYDropStart = y1;
	m_lXDropEnd = x2;
	m_lYDropEnd = y2;
}

bool CDrag3Button::bIsDrop(long x, long y)
{
	if (x>m_lXDropStart && x<m_lXDropEnd &&
		y>m_lYDropStart && y<m_lYDropEnd) return true;
	else return false;
}


//=== gdzie mozna zwrocic przycisk

void CDrag3Button::SetReturnArea(long x1, long y1, long x2, long y2)
{
	m_lXReturnStart = x1;
	m_lYReturnStart = y1;
	m_lXReturnEnd = x2;
	m_lYReturnEnd = y2;
}

// mouse state 

void CDrag3Button::SetState(char cState)
{
	m_cState = cState;
}

char CDrag3Button::cGetState(void)
{
	return m_cState;
}

// czy obiekt wraca do pola macierzystego

void CDrag3Button::SetMovable(bool bMove)
{
	m_bMovable = bMove;
}

bool CDrag3Button::bGetMovable(void)
{
	return m_bMovable;
}

// wartosci centrujace

void CDrag3Button::SetAddPosition(long x, long y)
{
	m_lXAdd = x;
	m_lYAdd = y;
}

long CDrag3Button::lGetAddXPosition(void)
{
	return m_lXAdd;
}

long CDrag3Button::lGetAddYPosition(void)
{
	return m_lYAdd;
}

bool CDrag3Button::bIsReturn(long x, long y)	// true - w pozycji zwrotu, false - poza nia
{
	if (x>m_lXReturnStart && x<m_lXReturnEnd &&
		y>m_lYReturnStart && y<m_lYReturnEnd) return true;
	else return false;
}

//=== wartosci jednostkowe

void CDrag3Button::SetFE(bool bState)
{
	m_bFE = bState;
}

bool CDrag3Button::bGetFE(void)
{
	return m_bFE;
}

void CDrag3Button::SetPress(bool bState)
{
	m_bPressed = bState;
}
	
bool CDrag3Button::bGetPress(void)
{
	return m_bPressed;
}

//=== ustaw rotacje

void CDrag3Button::SetRotation(float newRotation)
{
	m_pOnMove->SetRotation(newRotation);
}
	
//=== pobierz rotacje

float CDrag3Button::fGetRotation(void)
{
	return m_pOnMove->GetRotation();
}

//=== dodaj lub odejmij rotacje

void CDrag3Button::AddRotation(float addValue)
{
	m_pOnMove->AddRotation(addValue);
}

//=== ustaw punkt rotacji w obiekcie

void CDrag3Button::SetRotationCenter(float x, float y)
{
	m_pOnMove->SetRotationCenter(x,y);
}

//=== ustaw skalê obiektu

void CDrag3Button::SetScale(float fValueX, float fValueY)
{
	m_pOnMove->SetScale(fValueX,fValueY);
}

//=== ustaw skalê obiektu - stretching

void CDrag3Button::SetScaleX(float fValueX)
{
	m_pOnMove->SetScaleX(fValueX);
}

//=== ustaw skalê obiektu - stretching

void CDrag3Button::SetScaleY(float fValueY)
{
	m_pOnMove->SetScaleY(fValueY);
}

//=== pobierz skalê X

float CDrag3Button::fGetScaleX(void)
{
	return m_pOnMove->GetScaleX();
}

//=== pobierz skalê Y
	
float CDrag3Button::fGetScaleY(void)
{
	return m_pOnMove->GetScaleY();
}

//=== dodaj skalê

void CDrag3Button::AddScale(float fValueX, float fValueY)
{
	m_pOnMove->AddScale(fValueX,fValueY);
}

//=== dodaj skalê X

void CDrag3Button::AddScaleX(float fValueX)
{
	m_pOnMove->AddScaleX(fValueX);
}

//=== dodaj skalê Y

void CDrag3Button::AddScaleY(float fValueY)
{
	m_pOnMove->AddScaleY(fValueY);
}


//=== end ===