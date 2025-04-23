// Sprite.h: interface for the CSprite class.
//
// zwykly duszek - jako jeden obiekt, jedna klatka
//////////////////////////////////////////////////////////////////////

#ifndef _SPRITE_
#define _SPRITE_

#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>

class CSprite  
{

public:
	
	CSprite(unsigned char alpha, unsigned char r, unsigned char g, unsigned char b);
	virtual ~CSprite();

	virtual HRESULT		Initialize(char *path,IDirect3DDevice8 *g_pDevice);
	virtual HRESULT		InitializeEx(char *path, IDirect3DDevice8 *g_pDevice, 
									 unsigned int uiWidth, unsigned int uiHeight, 
									 unsigned int MipLevels, DWORD dUsage, D3DFORMAT Format, 
									 D3DPOOL Pool, DWORD Filter, DWORD MipFilter, 
									 D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette);
	virtual HRESULT		InitializeTGAinMemory(unsigned int *pSource, unsigned long m_ulSize, IDirect3DDevice8 *g_pDevice);
	virtual	HRESULT		InitializeTGAinMemoryEx(unsigned int *pSource, unsigned long m_ulSize, IDirect3DDevice8 *g_pDevice,
										 unsigned int uiWidth, unsigned int uiHeight, unsigned int MipLevels, 
										 DWORD dUsage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, 
										 D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette);
	
	virtual HRESULT		Render();
	
	//-=- modulacja -=-

	virtual void		SetModulate(unsigned char alpha, unsigned char r, unsigned char g, unsigned char b);

	//-=- obrót -=-

	virtual void		SetRotation(float newRotation);			// ustaw rotacje
	virtual void		SetRotationCenter(float x, float y);	// ustaw punkt rotacji
	virtual float		GetRotation(void);						// pobierz rotacje
	virtual void		AddRotation(float addValue);			// dodaj lub odejmij 

	//-=- przesuniêcie -=-

	virtual void		SetTranslation(float x, float y);		// ustaw przesuniecie
	virtual float		GetTranslationX(void);
	virtual float		GetTranslationY(void);
	virtual void		AddTranslation(float x, float y);

	//-=- konwersje k¹tów -=-

	virtual float		GetDegree(float fVector);				// podaj w katach
	virtual float		GetRadian(float fVector);				// podaj w radianach

	//-=- skalowanie -=-

	virtual void		SetScale(float fValueX, float fValueY);	// ustaw skalê obiektu
	
	virtual void		SetScaleX(float fValueX);				// ustaw skalê
	virtual void		SetScaleY(float fValueY);

	virtual float		GetScaleX(void);						// pobierz wspó³rzêdne
	virtual float		GetScaleY(void);		

	virtual void		AddScale(float fValueX, float fValueY);	// dodaj skalê

	virtual void		AddScaleX(float fValueX);
	virtual void		AddScaleY(float fValueY);

	//=== widzialnosci

	virtual void		SetVisible(bool bState);
	virtual bool		GetVisible(void);

	//=== stan inicjalizacji (czy byl zainicjalizowany, czy tylko klasa)

	virtual void		SetState(bool bState);
	virtual bool		GetState(void);

	//=== do blokowania i rysowania bezposrednio na texturze

	LPD3DXSPRITE		m_pSprite;
	
	LPDIRECT3DTEXTURE8	m_pTexture;

private:
	
	bool				m_bVisible;			// czy widoczny, czy ma byc renderowany

	
	D3DXVECTOR2			m_RotCenter;		// w pixelach
	D3DXVECTOR2			m_Translation;		// przesuniecie we float 
	D3DXVECTOR2			m_Scaling;			// scala we float 

	float				m_Rotation;			// we float
	
	D3DCOLOR			m_ModulateColor;	// color 

	bool				m_bInitialized;

};

#endif 
 