// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "Sprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//=== inicjalizacja obiektu

CSprite::CSprite(unsigned char alpha, unsigned char r, unsigned char g, unsigned char b)
{
	m_pSprite = 0;
	m_pTexture = 0;
	
	m_bInitialized = false;
	
	m_RotCenter.x = 0.0f;
	m_RotCenter.y = 0.0f;
	
	m_Translation.x = 0.0f;
	m_Translation.y = 0.0f;
	
	m_Scaling.x = 1.0f;
	m_Scaling.y = 1.0f;
	
	m_ucModulateA = alpha;
	m_ucModulateR = r;
	m_ucModulateG = g;
	m_ucModulateB = b;

	m_ModulateColor = D3DCOLOR_ARGB(m_ucModulateA, m_ucModulateR, m_ucModulateG, m_ucModulateB);	// lub RGBA
	
	m_Rotation = 0.0f;

	m_pSprite = NULL;	// nie aktywny
	m_pTexture = NULL;
}

//=== usuniecie obiektu

CSprite::~CSprite()
{
	if (m_pSprite!=NULL)	// usuwamy jezeli zaalokowany
	{
		m_pSprite->Release();
		m_pSprite = NULL;
	}

	if (m_pTexture!=NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=== inicjalizuj obiekt - stworz ze zwyklego pliku

HRESULT CSprite::Initialize(char pCurrentDirectory[200],IDirect3DDevice8 *g_pDevice, char *path)
{

	char bufor[300];

	HRESULT hr = 0;

	wsprintf(bufor, "%s\\Resource\\Bitmaps\\%s", pCurrentDirectory,path);

	hr = D3DXCreateTextureFromFile(g_pDevice, bufor, &m_pTexture);

	D3DXCreateSprite(g_pDevice, &m_pSprite);

	m_bInitialized = TRUE;

	return S_OK;
}

//=== inicjalizuj obiekt, który jest ju¿ w pamiêci jako TGA

HRESULT CSprite::InitializeTGAinMemory(unsigned int *pSource, unsigned long m_ulSize, IDirect3DDevice8 *g_pDevice)
{
	
	HRESULT hr = 0;
	
	hr = D3DXCreateTextureFromFileInMemory(g_pDevice, pSource, m_ulSize, &m_pTexture);

	D3DXCreateSprite(g_pDevice, &m_pSprite);

	m_bInitialized = TRUE;

	return S_OK;
}

//=== inicjalizuj obiekt, wszystkie parametry podajemy recznie

HRESULT CSprite::InitializeTGAinMemoryEx(unsigned int *pSource, unsigned long m_ulSize, IDirect3DDevice8 *g_pDevice,
										 unsigned int uiWidth, unsigned int uiHeight, unsigned int MipLevels, 
										 DWORD dUsage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, 
										 D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette)
{
	HRESULT hr=0;

	hr = D3DXCreateTextureFromFileInMemoryEx(g_pDevice, pSource, m_ulSize, uiWidth, uiHeight, MipLevels, dUsage,
											Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, &m_pTexture);

	D3DXCreateSprite(g_pDevice, &m_pSprite);

	m_bInitialized = TRUE;

	return S_OK;
}

//=== renderuj sprite

HRESULT CSprite::Render()
{
	if (!m_bInitialized)	return E_FAIL;

	if (m_bVisible == false) return S_OK;	// po prostu nie rysuje niewidocznego

	HRESULT hr = 0;

	m_ModulateColor = D3DCOLOR_ARGB(m_ucModulateA, m_ucModulateR, m_ucModulateG, m_ucModulateB);	// lub RGBA

	m_pSprite->Begin();

	hr = m_pSprite->Draw(m_pTexture, NULL,
						 &m_Scaling, &m_RotCenter, m_Rotation, &m_Translation, 
						 m_ModulateColor);

	m_pSprite->End();
	
	return S_OK;
}

//=== ustaw rotacje

void CSprite::SetRotation(float newRotation)
{
	m_Rotation = newRotation;
}
	
//=== pobierz rotacje

float CSprite::GetRotation(void)
{
	return m_Rotation;
}

//=== dodaj lub odejmij rotacje

void CSprite::AddRotation(float addValue)
{
	m_Rotation+=addValue;
}

//=== ustaw przesuniecie obiektu

void CSprite::SetTranslation(float x, float y)
{
	m_Translation.x = x;
	m_Translation.y = y;
}

//=== pobierz przesuniecie x

float CSprite::GetTranslationX(void)
{
	return m_Translation.x;
}

//=== pobierz przesuniecie y

float CSprite::GetTranslationY(void)
{
	return m_Translation.y;
}

//=== dodaj lub odejmij przesuniecie

void CSprite::AddTranslation(float x, float y)
{
	m_Translation.x += x;
	m_Translation.y += y;
}

//=== ustaw punkt rotacji w obiekcie

void CSprite::SetRotationCenter(float x, float y)
{
	m_RotCenter.x = x;
	m_RotCenter.y = y;
}

//=== zwroc wartosc kata w stopniach

float CSprite::GetDegree(float fVector)
{
	return D3DXToDegree(fVector);// * (180.0f / D3DX_PI));
}

//=== zwroc wartosc kata w radianach

float CSprite::GetRadian(float fVector)
{
	return D3DXToRadian((fVector) * (D3DX_PI / 180.0f));
}

//=== ustaw skalê obiektu

void CSprite::SetScale(float fValueX, float fValueY)
{
	m_Scaling.x=fValueX;
	m_Scaling.y=fValueY;
}

//=== ustaw skalê obiektu - stretching

void CSprite::SetScaleX(float fValueX)
{
	m_Scaling.x=fValueX;
}

//=== ustaw skalê obiektu - stretching

void CSprite::SetScaleY(float fValueY)
{
	m_Scaling.y=fValueY;
}

//=== pobierz skalê X

float CSprite::GetScaleX(void)
{
	return m_Scaling.x;
}

//=== pobierz skalê Y
	
float CSprite::GetScaleY(void)
{
	return m_Scaling.y;
}

//=== dodaj skalê

void CSprite::AddScale(float fValueX, float fValueY)
{
	m_Scaling.x += fValueX;
	m_Scaling.y += fValueY;
}

//=== dodaj skalê X

void CSprite::AddScaleX(float fValueX)
{
	m_Scaling.x += fValueX;
}

//=== dodaj skalê Y

void CSprite::AddScaleY(float fValueY)
{
	m_Scaling.y += fValueY;
}

//=== ustawia widocznosc ===

void CSprite::SetVisible(bool bState)
{
	m_bVisible = bState;
}

//=== zwraca znacznik widocznosci ===

bool CSprite::GetVisible(void)
{
	return m_bVisible; 
}

//=== stan inicjalizacji (czy byl zainicjalizowany, czy tylko klasa)

void CSprite::SetState(bool bState)
{
	// ta klasa nigdy nie powinna byc wywolywana
	// inicjalizacja powinna byc poprzez InitializeTGAInMemory
	
	m_bInitialized = bState;
}

//=== sprawdz czy zainicjalizowany

bool CSprite::GetState(void)
{
	return m_bInitialized;
}

//=== ustaw modulacje

void CSprite::SetModulate(unsigned char alpha, unsigned char r, unsigned char g, unsigned char b)
{
	m_ucModulateA = alpha; 
	m_ucModulateR = r;
	m_ucModulateG = g;
	m_ucModulateB = b;
	//	m_ModulateColor = D3DCOLOR_ARGB(alpha,r,g,b);	// lub RGBA
}

//=== blokowanie textury 

void CSprite::Lock(void)
{
	m_pTexture->LockRect(0,&LockedRect,NULL,0);
	
	m_pTextureBuffer = (DWORD *)LockedRect.pBits;
}

//=== odblokowanie textury

void CSprite::Unlock(void)
{
	m_pTexture->UnlockRect(0);
}

//=== rysowanie na zablokowanej texturze

void CSprite::Paint(int iX, int iY, int iA, int iR, int iG, int iB)
{
	long offset = iX + iY * (LockedRect.Pitch>>2);
	m_pTextureBuffer[offset] = D3DCOLOR_ARGB(iA,iR,iG,iB);
}

void CSprite::PaintAdd(int iX, int iY, int iA, int iR, int iG, int iB)
{
	long offset = iX + iY * (LockedRect.Pitch>>2);
	unsigned char r,g,b;
	unsigned long color;

	color = m_pTextureBuffer[offset];
	r= (unsigned char)color>>8;
	g= (unsigned char)color>>16;
	b= (unsigned char)color>>24;

	iR+=r; iG+=g; iB+=b; 

	if (iR>255) iR=255;
	if (iG>255) iG=255;
	if (iB>255) iB=255;

	m_pTextureBuffer[offset] = D3DCOLOR_ARGB(iA,iR,iG,iB);
}


//=== pobieranie skladowych na zablokowanej texturze

unsigned char CSprite::ucGetA(int iX, int iY)
{
	long offset = iX + iY * (LockedRect.Pitch>>2);
	long color;
	color = m_pTextureBuffer[offset];
	return (unsigned char)color;
}

unsigned char CSprite::ucGetR(int iX, int iY)
{
	long offset = iX + iY * (LockedRect.Pitch>>2);
	long color;
	color = m_pTextureBuffer[offset];
	return (unsigned char)color>>8;
}

unsigned char CSprite::ucGetG(int iX, int iY)
{
	long offset = iX + iY * (LockedRect.Pitch>>2);
	long color;
	color = m_pTextureBuffer[offset];
	return (unsigned char)color>>16;
}

unsigned char CSprite::ucGetB(int iX, int iY)
{
	long offset = iX + iY * (LockedRect.Pitch>>2);
	long color;
	color = m_pTextureBuffer[offset];
	return (unsigned char)color>>24;
}

void CSprite::InitDesc(void)
{
	if (m_pTexture!=NULL)
	{
		m_pTexture->GetSurfaceLevel(0,&m_pSurface);
		m_pSurface->GetDesc(&m_pDesc);
		m_iXSize = m_pDesc.Width;
		m_iYSize = m_pDesc.Height;
	}
}


int CSprite::iGetYSize(void)
{
	return m_iYSize;
}

int CSprite::iGetXSize(void)
{
	return m_iXSize;
}

unsigned char CSprite::ucGetModulateA(void)
{
	return m_ucModulateA;
}

unsigned char CSprite::ucGetModulateR(void)
{
	return m_ucModulateR;
}

unsigned char CSprite::ucGetModulateG(void)
{
	return m_ucModulateG;
}

unsigned char CSprite::ucGetModulateB(void)
{
	return m_ucModulateB;
}
// end