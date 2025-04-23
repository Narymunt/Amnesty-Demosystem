// Scene3d.h: interface for the CScene3d class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCENE3D_
#define _SCENE3D_

#include <windows.h>
#include <d3d8.h>

#include "utils.h"
#include "object.h"
#include "keyframer.h"
#include "texture.h"
#include <scene.h>
#include "sprite.h"

#ifndef BARSNUMBER
	#define	BARSNUMBER		35
#endif

class CScene3d  
{
public:
	CScene3d(IDirect3DDevice8 *pDevice, char cFilename []);
	CScene3d(IDirect3DDevice8 *pDevice);
	void ParseFromFile(IDirect3DDevice8 *pDevice, char cScriptFile[]);

	virtual ~CScene3d();


	void UpOnly(char str[]);
	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice,char cFilename[]);
	virtual void InitializeOut(IDirect3DDevice8 *pDevice,char cFilename[]);
	virtual void DeInitialize(void);
	
	virtual void InitializeBlack(IDirect3DDevice8 *pDevice, char cFilename[]);
	virtual void InitializeWhite(IDirect3DDevice8 *pDevice, char cFilename[]);
	virtual void InitializeBlackEnd(IDirect3DDevice8 *pDevice, char cFilename[]);
	virtual void InitializeWhiteEnd(IDirect3DDevice8 *pDevice, char cFilename[]);
	virtual void InitializeBlackBlack(IDirect3DDevice8 *pDevice, char cFilename[]);
	virtual void InitializeWhiteWhite(IDirect3DDevice8 *pDevice, char cFilename[]);
	virtual void InitializeBlackWhite(IDirect3DDevice8 *pDevice, char cFilename[]);
	virtual void InitializeWhiteBlack(IDirect3DDevice8 *pDevice, char cFilename[]);
	
	virtual int	 DrawScene(IDirect3DDevice8 *pDevice, long lTimer);
	virtual int	 DrawSceneOn(IDirect3DDevice8 *pDevice, long lTimer);
	virtual int	 DrawSceneOutBlack(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart);	// withblackout
	virtual int	 DrawSceneOutWhite(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart);	// withblackout

	virtual int	 DrawSceneInBlack(IDirect3DDevice8 *pDevice, long lTimer);
	virtual int	 DrawSceneInWhite(IDirect3DDevice8 *pDevice, long lTimer);

	virtual int  DrawSceneBlackBlack(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart);
	virtual int  DrawSceneWhiteWhite(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart);
	virtual int  DrawSceneBlackWhite(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart);
	virtual int  DrawSceneWhiteBlack(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart);

	virtual	bool bGetFogEnable(void);
	virtual void SetFog(bool bState);

	virtual float	fGetFogStart(void);
	virtual void	SetFogStart(float fFogStart);

	virtual float	fGetFogEnd(void);
	virtual void	SetFogEnd(float fFogEnd);

	virtual unsigned int uiGetFogColor(void);
	virtual void	SetFogColor(unsigned int m_iFogColor);

	// jezeli dynamiczna to wczytywana dopiero przed wywolaniem

	virtual void	SetDynamic(bool bState);
	virtual bool	bGetDynamic(void);

private:

	char			m_cFilename[256];	// nazwa pliku ze scenka

	bool			m_bDynamic;			// czy dynamiczna, czy wczytywana od razu

	bool			m_bActive;


	DWORD			maxFrame;
	DWORD			frame;

	G3D_CScene		*scene;
	
	bool			m_bBackground;		// czy ma wyswietlac tlo
	char			m_cBackgroundFilename[256];	// tlo wczytywane tuz przed wyswietleniem jezeli DYNAMIC
	CSprite			*m_pBackground;		// to jest renderowane przed scena
	
	CSprite			*m_pBlackScreen;	// to jest renderowane gdy jest fade
	CSprite			*m_pWhiteScreen;	// to jest renderowane gdy jest fade

	CSprite			*m_pForeground;		// to jest renderowane po renderowaniu sceny

	unsigned char	m_ucClearA;
	unsigned char	m_ucClearR;
	unsigned char	m_ucClearG;
	unsigned char	m_ucClearB;

	//=== FOG ===
	
	bool			m_bLoop;

	bool			m_bFogEnable;	

	float			m_fFogStart;	// 5.0f
	float			m_fFogEnd;	// 200.0f

	unsigned int	m_uiFogColor;	// must be set, default 0x000f0f0f

	float			m_fPlayRate;

	//=== oswietlenie

	bool			m_bLighting;

	//=== motion blur

	bool			m_bBlur;
	int				m_iBlurCount; // ile razy rysowaæ t¹ sam¹ scenkê
	int				m_iBlurSkip;	// co ile klatek odtwarzac render

};

#endif 
