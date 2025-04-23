
// general include

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dmusicc.h>
#include <dmusici.h>
//#include <iostream.h>

// dodatkowe

#include "Font.h"
#include "LoadScreen.h"
#include "Scene3d.h"
#include "MetaballsFX.h"
#include "RadialZoom.h"
#include "FXTemp.h"
#include "Sprite.h"


// flaga debug 

#include "Debug.h"

// bass player

#include "bassplayer.h"

// ikonki, okienka 

#include "Resource.h"

//=== opoznienie

long lDelay = 0;

//=== zmienne dla directx

IDirect3D8			*pDirect3D;		// handler
IDirect3DDevice8	*pDevice;		// urzadzenie

HRESULT				hr;				// wynik operacji

HWND				hWnd;			// dla okna
HINSTANCE			hInstance;


bool				bFullScreen;	// czy pelny ekran ? 

char CurrentDirectory[512]		= {0,};	// aktualny katalog

//=== dla timera

int					g_FrameCount		= 0;
int					g_StartFrameCount	= 0;
unsigned long		g_StartTime			= timeGetTime();
unsigned long		g_DemoTime			= 0;
unsigned long		g_EndTime			= g_StartTime;
unsigned long		g_CurrentTime		= 0;
int					g_FrameRate			= 0;
long				lTTimer=0;


//=== czcionki

CFont				*pFont=NULL;			// jakas czcionka

//=== efekty

CLoadScreen			*pLoadScreen;	// ekran wczytywania + progress
CRadialZoom			*pRadialZoom = NULL;	// logo spd
CMetaballsFX		*pMetaballs = NULL;
CScene3d			*pMeczet = NULL;
CScene3d			*pTuba3d = NULL;		// wewnatrz torusa
CScene3d			*pKolumny = NULL;		// kolumny ze œwiat³em
CScene3d			*pWalec = NULL;			// walec z fur
CScene3d			*pWieza = NULL;			// wierza
CScene3d			*pSpider = NULL;		// pajaczek
CScene3d			*pHelikopter = NULL;	// wiadomo :)
CScene3d			*pTunel3d = NULL;		// tunel wolny
CScene3d			*pRing = NULL;			// pierscienie
CScene3d			*pAbstract01 = NULL;	// abstrakcja
CScene3d			*pMiasto = NULL;		// miasteczko 
CScene3d			*pAbstract03 = NULL;	// abstrakcja
CScene3d			*pFur = NULL;			// FUR shading :)
CScene3d			*pAbstract02 = NULL;	// abstrakcja - woda
CScene3d			*pTornado = NULL;		// tornado na pustyni
CScene3d			*pFurBall = NULL;		// kostka z kolcami

CSprite				*pTheEnd = NULL;
CSprite				*pPost01 = NULL;
CSprite				*pPost02 = NULL;
CSprite				*pPost03 = NULL;
CSprite				*pPost04 = NULL;
CSprite				*pPost05 = NULL;
CSprite				*pPost07 = NULL;


//CScene3d			*pTheEnd = NULL;		// morph + the end

//=== screen loading

CSprite				*pWaitScreen=NULL;

//=== fade screens

CSprite				*pWhiteScreen=NULL;
CSprite				*pBlackScreen=NULL;

//=== scrolls

CSprite				*m_pLeftScroll=NULL;

//=== kadry

CSprite				*pKadr=NULL;

//=== switch efektów

// LOADSCREEN - bitmapa, wczytuja sie dane dema
// RADIALZOOM - zoom logo suspend
// MECZET - scenka meczetu


// END - koniec dema

enum STAN_DEMO		{
						LOADSCREEN,
						RADIALZOOM,				
						ABSTRACT00,
						MECZET,
						METABALLS,
						TUBA3D,
						POMNIK,
						KOLUMNY,
						WALEC,
						WIEZA,
						SPIDER,
						HELIKOPTER,
						TUNEL3D,
						RING,
						ABSTRACT01,
						MIASTO,
						ABSTRACT03,
						FUR,
						ABSTRACT02,
						TORNADO,
						FURBALL,
						THEEND,
						END
					} eStanDemo;

//=== player

CBassPlayer			*pBassPlayer;

//=== dodatkowe

int					iLicznik;
long				h1,h2,h3,h4;	// do petli
float				f1,f2,f3;

//=== fog

float			m_fFogStart=5.0f;	// 5.0f
float			m_fFogEnd=5000.0f;	// 200.0f

//unsigned int	m_uiFogColor=0x00B3A483;	// must be set, default 0x000f0f0f
unsigned int	m_uiFogColor=0x00000000;	// must be set, default 0x000f0f0f

//=== pomocnicza, inicjalizuje direct3d i ustawia rozne dodatki

bool Direct3DInit()
{
	pDirect3D = Direct3DCreate8(D3D_SDK_VERSION);

	if (pDirect3D==NULL) 
	{
		MessageBox(0,"Cannot init DirectX 9.0 !","Error!",MB_OK);
		return false;
	}

	// ustaw w odpowiedni sposob tryb sprawdzajac czy wyswietlamy w oknie czy
	// na calym ekranie

	D3DDISPLAYMODE displayMode;

	if (bFullScreen==false)
	{
		hr = pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
		
		if (FAILED(hr)) 
		{
			MessageBox(0,"Cannot init DirectX!","Error!",MB_OK);
			return false;
		}
	}
	else
	{
		displayMode.Width = 800;				//iWidth;
		displayMode.Height = 600;				//iHeight;
		//displayMode.RefreshRate = 6;
		displayMode.Format = D3DFMT_A8R8G8B8;	//R5G6B5;		// tu mozna zmienic
	}

	// ustaw/zapamietaj aktualne parametry wyswietlania

	D3DPRESENT_PARAMETERS presentParameters;

	memset(&presentParameters, 0, sizeof(D3DPRESENT_PARAMETERS));

	if (bFullScreen==false)
	{
		presentParameters.Windowed = TRUE;
	}
	else
	{
		presentParameters.Windowed = FALSE;
		presentParameters.BackBufferWidth = displayMode.Width;
		presentParameters.BackBufferHeight = displayMode.Height;
	}

	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = displayMode.Format;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;

	// stworz urzadzenie

	hr=pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &presentParameters, &pDevice ); 

	if (FAILED(hr))	
	{
		MessageBox(0,"Cannot create DirectX device !","Error!",MB_OK);		
		return false;
	}

	SetCursor(NULL);					// ukryj kursor myszy, rysujemy wlasny
	pDevice->ShowCursor(FALSE);			// przeslij do urzadzenia
	
	// ustaw kursor myszy na srodku ekranu

	pDevice->SetCursorPosition(400,300,D3DCURSOR_IMMEDIATE_UPDATE );	

	// ustaw macierze 

	D3DXMATRIX mat;

	// tutaj lepiej ustawic bez rzutowania typu (bez float)

	D3DXMatrixPerspectiveFovLH(&mat, D3DX_PI/4, (float)800/600,	-100.0, 100.0);
	
	pDevice->SetTransform(D3DTS_PROJECTION, &(D3DMATRIX)mat);

	//D3DXMatrixIdentity(&mat);

	// ustaw transformacje

//	pDevice->SetTransform(D3DTS_WORLD, &(D3DMATRIX)mat);
	pDevice->SetTransform(D3DTS_VIEW, &(D3DMATRIX)mat);

    pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	// backface culling off
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );			// swiatlo
	pDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );		// dithering
	pDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_SOLID);
	pDevice->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_GOURAUD);

	// blending
	
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_ANISOTROPIC);
	pDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_ANISOTROPIC);   
	pDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_ANISOTROPIC);   
	pDevice->SetTextureStageState(0, D3DTSS_MAXANISOTROPY,4);

	// alpha channel
	
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	// macierz widoku

	D3DXMatrixTranslation(&mat, 0, 0, 3);
	pDevice->SetTransform(D3DTS_VIEW, &(D3DMATRIX)mat);

	// dodatkowe obiekty - powinno byc jak najmniej

	pFont = new CFont();
	pFont->Initialize(pDevice,(HFONT)GetStockObject(SYSTEM_FONT),D3DCOLOR_XRGB(0,255,255));

	pWaitScreen = new CSprite(255,255,255,255);	// nowy sprite
	pWaitScreen->InitializeEx("data\\loading.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pWhiteScreen = new CSprite(255,255,255,255);	// nowy sprite
	pWhiteScreen->InitializeEx("data\\white.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pTheEnd = new CSprite(255,255,255,255);	// nowy sprite
	pTheEnd->InitializeEx("data\\the_end.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pBlackScreen = new CSprite(255,255,255,255);	// nowy sprite
	pBlackScreen->InitializeEx("data\\black.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pKadr = new CSprite(255,255,255,255);	// nowy sprite
	pKadr->InitializeEx("data\\kadr.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pPost01 = new CSprite(255,255,255,255);	// nowy sprite
	pPost01->InitializeEx("data\\post01.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pPost02 = new CSprite(255,255,255,255);	// nowy sprite
	pPost02->InitializeEx("data\\post02.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pPost03 = new CSprite(255,255,255,255);	// nowy sprite
	pPost03->InitializeEx("data\\post03.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pPost04 = new CSprite(255,255,255,255);	// nowy sprite
	pPost04->InitializeEx("data\\post04.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pPost05 = new CSprite(255,255,255,255);	// nowy sprite
	pPost05->InitializeEx("data\\post05.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pPost07 = new CSprite(255,255,255,255);	// nowy sprite
	pPost07->InitializeEx("data\\post01.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);


	pDevice->BeginScene();
	pWaitScreen->Render();

	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);	

	if (pWaitScreen!=NULL)
	{
		delete pWaitScreen;
		pWaitScreen=NULL;
	}

	// scrolls

	m_pLeftScroll= new CSprite(255,255,255,255);
	m_pLeftScroll->InitializeEx("data\\blobl.tga",pDevice,0,0,0,0,D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);


	// wczytywanie 

	SetTexturePath("data");
	InitializeTextureSystem( pDevice );		

	pLoadScreen = new CLoadScreen();
	pLoadScreen->Initialize(pDevice);

	pRadialZoom = new CRadialZoom();
	pRadialZoom->Initialize(pDevice);

	pMeczet = new CScene3d(pDevice);
//	pMeczet->SetFog(true);
//	pMeczet->SetFogColor(0x00ffffff);
//	pMeczet->SetFogStart(5.0f);
//	pMeczet->SetFogEnd(2500.0f);
	pMeczet->Initialize(pDevice,"data\\meczet.3d");	// wb

	pTuba3d = new CScene3d(pDevice);
	pTuba3d->Initialize(pDevice,"data\\tuba.3d"); //wb

	pKolumny = new CScene3d(pDevice);
	pKolumny->Initialize(pDevice,"data\\kolumny.3d"); //wb

	pWieza = new CScene3d(pDevice);
	pWieza->Initialize(pDevice,"data\\wieza.3d");

	pHelikopter = new CScene3d(pDevice);
	pHelikopter->Initialize(pDevice,"data\\hind.3d");

	pTunel3d = new CScene3d(pDevice,"data\\tunelb.jpg");
	pTunel3d->Initialize(pDevice,"data\\tunel.3d"); //w

	pRing = new CScene3d(pDevice,"data\\ring.jpg");
	pRing->Initialize(pDevice,"data\\ring.3d");

	pAbstract01 = new CScene3d(pDevice,"data\\abstr01b.jpg");
	pAbstract01->Initialize(pDevice,"data\\abstr01.3d");

	pMiasto = new CScene3d(pDevice);
	pMiasto->Initialize(pDevice,"data\\miasto.3d");

	pAbstract03 = new CScene3d(pDevice,"data\\abstr03b.jpg");
	pAbstract03->Initialize(pDevice,"data\\abstr03.3d");

	pFur = new CScene3d(pDevice,"data\\fur_back.jpg");
	pFur->Initialize(pDevice,"data\\fur.3d");

	pAbstract02 = new CScene3d(pDevice,"data\\abstr02b.jpg");
	pAbstract02->Initialize(pDevice,"data\\abstr02.3d");

	pTornado = new CScene3d(pDevice);
	pTornado->Initialize(pDevice,"data\\tornado.3d"); //wb

	pFurBall = new CScene3d(pDevice,"data\\furball.jpg");
	pFurBall->Initialize(pDevice,"data\\furball.3d");

//	ShowWindow( hWnd, SW_SHOWDEFAULT );
//	UpdateWindow( hWnd );		

	return true;
}

// aktualizuj stan dema

void Refresh(unsigned long ulTimer)
{
	eStanDemo = LOADSCREEN;
	if (ulTimer>750) eStanDemo = RADIALZOOM;
	if (ulTimer>1550) eStanDemo	= ABSTRACT00;
	if (ulTimer>2300) eStanDemo = ABSTRACT02;
	if (ulTimer>3000) eStanDemo = TUBA3D;
	if (ulTimer>4600) eStanDemo = MECZET;
	if (ulTimer>5300) eStanDemo = KOLUMNY;
	if (ulTimer>5400) eStanDemo = MECZET;
	if (ulTimer>5500) eStanDemo = KOLUMNY;
	if (ulTimer>5600) eStanDemo = MECZET;
	if (ulTimer>5700) eStanDemo = KOLUMNY;
	if (ulTimer>5800) eStanDemo = MECZET;
	if (ulTimer>5900) eStanDemo = KOLUMNY;
	if (ulTimer>6000) eStanDemo = WIEZA;
	if (ulTimer>6100) eStanDemo = MECZET;
	if (ulTimer>6200) eStanDemo = KOLUMNY;
	if (ulTimer>6300) eStanDemo = WIEZA;
	if (ulTimer>6400) eStanDemo = MECZET;
	if (ulTimer>6500) eStanDemo = KOLUMNY;
	if (ulTimer>6600) eStanDemo = WIEZA;
	if (ulTimer>6700) eStanDemo = MECZET;
	if (ulTimer>6800) eStanDemo = KOLUMNY;
	if (ulTimer>6900) eStanDemo = WIEZA;
	if (ulTimer>7000) eStanDemo = HELIKOPTER;
	if (ulTimer>7100) eStanDemo = MECZET;
	if (ulTimer>7200) eStanDemo = KOLUMNY;
	if (ulTimer>7300) eStanDemo = WIEZA;
	if (ulTimer>7400) eStanDemo = HELIKOPTER;
	if (ulTimer>7500) eStanDemo = MECZET;
	if (ulTimer>7600) eStanDemo = KOLUMNY;
	if (ulTimer>7700) eStanDemo = WIEZA;
	if (ulTimer>7800) eStanDemo = HELIKOPTER;
	if (ulTimer>8000) eStanDemo = TUNEL3D;
	if (ulTimer>9000) eStanDemo = RING;
	if (ulTimer>9100) eStanDemo = HELIKOPTER;
	if (ulTimer>9200) eStanDemo = RING;
	if (ulTimer>9300) eStanDemo = HELIKOPTER;
	if (ulTimer>9400) eStanDemo = RING;
	if (ulTimer>9500) eStanDemo = HELIKOPTER;
	if (ulTimer>9600) eStanDemo = RING;
	if (ulTimer>9700) eStanDemo = HELIKOPTER;
	if (ulTimer>9800) eStanDemo = RING;
	if (ulTimer>9900) eStanDemo = HELIKOPTER;
	if (ulTimer>10000) eStanDemo = ABSTRACT01;
	if (ulTimer>10100) eStanDemo = RING;
	if (ulTimer>10200) eStanDemo = HELIKOPTER;
	if (ulTimer>10300) eStanDemo = ABSTRACT01;
	if (ulTimer>10400) eStanDemo = RING;
	if (ulTimer>10500) eStanDemo = HELIKOPTER;
	if (ulTimer>10600) eStanDemo = ABSTRACT01;
	if (ulTimer>10700) eStanDemo = RING;
	if (ulTimer>10800) eStanDemo = HELIKOPTER;
	if (ulTimer>10900) eStanDemo = ABSTRACT03;
	if (ulTimer>11000) eStanDemo = MIASTO;
	if (ulTimer>11100) eStanDemo = RING;
	if (ulTimer>11200) eStanDemo = HELIKOPTER;
	if (ulTimer>11300) eStanDemo = ABSTRACT02;
	if (ulTimer>11400) eStanDemo = MIASTO;
	if (ulTimer>11500) eStanDemo = RING;
	if (ulTimer>11600) eStanDemo = HELIKOPTER;
	if (ulTimer>11700) eStanDemo = ABSTRACT03;
	if (ulTimer>11800) eStanDemo = MIASTO;
	if (ulTimer>12000) eStanDemo = ABSTRACT01;
	if (ulTimer>12100) eStanDemo = FUR;
	if (ulTimer>12200) eStanDemo = MIASTO;
	if (ulTimer>12300) eStanDemo = FUR;
	if (ulTimer>12400) eStanDemo = ABSTRACT03;
	if (ulTimer>12500) eStanDemo = FUR;
	if (ulTimer>12600) eStanDemo = MIASTO;
	if (ulTimer>12700) eStanDemo = FUR;
	if (ulTimer>12800) eStanDemo = ABSTRACT03;
	if (ulTimer>12900) eStanDemo = FUR;
	if (ulTimer>13000) eStanDemo = MIASTO;
	if (ulTimer>13100) eStanDemo = FUR;
	if (ulTimer>13200) eStanDemo = ABSTRACT01;
	if (ulTimer>13300) eStanDemo = FUR;
	if (ulTimer>13400) eStanDemo = ABSTRACT03;
	if (ulTimer>13500) eStanDemo = TORNADO;
	if (ulTimer>14500) eStanDemo = FURBALL;
	if (ulTimer>15200) eStanDemo = THEEND;
	if (ulTimer>16100) eStanDemo = END;
}


//=== rysuj scenke, tylko przygotowuje calosc, a potem odwoluje sie do odpowiedniej
//=== funkcji aktualizujacej w zaleznosci od CScene

void DrawScene()
{

	char str[200];

	if (g_DemoTime==0) g_DemoTime=timeGetTime();

	g_CurrentTime = lDelay + ((timeGetTime()-g_DemoTime)/10);

	pDevice->BeginScene();

	// sprawdz najpierw stan gry

	Refresh(g_CurrentTime);

	switch	(eStanDemo)
	{

	case	LOADSCREEN:

			pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
					
			if (!pLoadScreen->bGetActive())
			{
				pLoadScreen->Initialize(pDevice);
			}

			if (pLoadScreen->bGetActive())
			{
				pLoadScreen->DrawScene(g_CurrentTime);
			} 
			break;

	case	RADIALZOOM:
			pRadialZoom->DrawScene(g_CurrentTime);
			break;

	case	ABSTRACT00:
			pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

			lTTimer=(long)((g_CurrentTime-1550));

			pAbstract01->DrawScene(pDevice, (g_CurrentTime-1550));
			for (int i=1; i<5 ; i++) pAbstract01->DrawSceneOn(pDevice, (g_CurrentTime-1550+(i*8)));

			if (lTTimer<255)
			{
				pBlackScreen->SetModulate((unsigned char)(255-lTTimer),255,255,255);
				pBlackScreen->Render();
			}	

			break;

	case	ABSTRACT02:
 				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

					lTTimer=(long)((g_CurrentTime-2300)*2);

					pAbstract02->DrawScene(pDevice,lTTimer%1000);
					for (int i=1; i<5 ; i++) pAbstract02->DrawSceneOn(pDevice, (g_CurrentTime-2300+(i*8)));

					m_pLeftScroll->SetTranslation(64,(float)(-512+(lTTimer%512)));
					m_pLeftScroll->Render();
					m_pLeftScroll->SetTranslation(64,(float)((lTTimer%512)));
					m_pLeftScroll->Render();
					m_pLeftScroll->SetTranslation(64,(float)(512+(lTTimer%512)));
					m_pLeftScroll->Render();

					if (lTTimer<255)
					{
						pWhiteScreen->SetModulate((unsigned char)(255-lTTimer),255,255,255);
						pWhiteScreen->Render();
					}	

			break;


	case	TUBA3D:
					lTTimer=(long)((g_CurrentTime-3000)*4);
					
					pTuba3d->DrawScene(pDevice, lTTimer%2000); //wb 3345

					if (lTTimer<255)
					{
						pWhiteScreen->SetModulate((unsigned char)(255-lTTimer),255,255,255);
						pWhiteScreen->Render();
					}	

					if (lTTimer>=3000)
					{
						pBlackScreen->SetModulate((unsigned char)(lTTimer-3000),255,255,255);
						pBlackScreen->Render();
					}	
			break;

	case	MECZET:
					lTTimer = (long)((g_CurrentTime-4600)*3);
					
					pMeczet->DrawScene(pDevice,lTTimer%4000); //wb 4785
					if (lTTimer<255)
					{
						pWhiteScreen->SetModulate((unsigned char)(255-lTTimer),255,255,255);
						pWhiteScreen->Render();
					}	
			break;


	case	KOLUMNY:

					lTTimer=(long)((g_CurrentTime-5300)*2);

					pKolumny->DrawScene(pDevice, lTTimer%3000); //wb 2945

					if (lTTimer<255)
					{
						pWhiteScreen->SetModulate((unsigned char)(255-lTTimer),255,255,255);
						pWhiteScreen->Render();
					}	
			break;

	case	WIEZA:
					lTTimer=(long)((g_CurrentTime-6000));

					pWieza->DrawScene(pDevice, lTTimer%2000);

					if (lTTimer<255)
					{
						pWhiteScreen->SetModulate((unsigned char)(255-lTTimer),255,255,255);
						pWhiteScreen->Render();
					}	
			break;

	case	HELIKOPTER:
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

					lTTimer=(long)((g_CurrentTime-7000)*3);

					pHelikopter->DrawScene(pDevice,lTTimer%5000);

					if (lTTimer<255)
					{
						pBlackScreen->SetModulate((unsigned char)(255-lTTimer),255,255,255);
						pBlackScreen->Render();
					}	

			break;

	case	TUNEL3D:
				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
 				pDevice->SetRenderState(D3DRS_FOGCOLOR, m_uiFogColor);
				pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
				pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&m_fFogStart));
				pDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&m_fFogEnd));
					lTTimer=(long)((g_CurrentTime-8000)/4);
					pTunel3d->DrawScene(pDevice, lTTimer); //w
			break;

	case	RING:
				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

				lTTimer=(long)((g_CurrentTime-9000)*3);
					if (lTTimer<0) lTTimer = lTTimer * -1;

					pRing->DrawScene(pDevice, lTTimer%2000);

					m_pLeftScroll->SetTranslation(64,(float)(-512+(lTTimer%512)));
					m_pLeftScroll->Render();
					m_pLeftScroll->SetTranslation(64,(float)((lTTimer%512)));
					m_pLeftScroll->Render();
					m_pLeftScroll->SetTranslation(64,(float)(512+(lTTimer%512)));
					m_pLeftScroll->Render();
			break;

	case	ABSTRACT01:
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

					lTTimer=(long)((g_CurrentTime-10000)*2);
					if (lTTimer<0) lTTimer = lTTimer * -1;

					pAbstract01->DrawScene(pDevice, lTTimer%2000);
			break;

	case	MIASTO:
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
 				pDevice->SetRenderState(D3DRS_FOGCOLOR, m_uiFogColor);
				pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
				pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&m_fFogStart));
				pDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&m_fFogEnd));
//
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

				lTTimer=(long)((g_CurrentTime-11000)*3);
					if (lTTimer<0) lTTimer = lTTimer * -1;
					
				pMiasto->DrawScene(pDevice,lTTimer%4000);

			break;

	case	ABSTRACT03:
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

					lTTimer=(long)((g_CurrentTime-12400)*2);

					if (lTTimer<0) lTTimer = lTTimer * -1;

					pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
					pAbstract03->DrawScene(pDevice, lTTimer%3000);
			break;

	case	FUR:
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

					lTTimer=(long)((g_CurrentTime-12100)*2);

					if (lTTimer<0) lTTimer = lTTimer * -1;

					pFur->DrawScene(pDevice, lTTimer%1000);

			break;


	case	TORNADO:
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
 				pDevice->SetRenderState(D3DRS_FOGCOLOR, m_uiFogColor);
				pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
				pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&m_fFogStart));
				pDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&m_fFogEnd));
//

					lTTimer=(long)((g_CurrentTime-13500)*4);
					if (lTTimer<0) lTTimer = lTTimer * -1;

					pTornado->DrawScene(pDevice, lTTimer%4000); //wb 4295

			break;

	case	FURBALL:

				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

					lTTimer=(long)((g_CurrentTime-15000));
					if (lTTimer<0) lTTimer = lTTimer * -1;

					pFurBall->DrawScene(pDevice, lTTimer); //bb 1245

			break;



	case	THEEND:

				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

				lTTimer=(long)((g_CurrentTime-16000));

				pTheEnd->Render();

			break;

	case	END:
			PostQuitMessage(0);
			break;
	}

	if ((g_CurrentTime%1024)<255)
	{
		pWhiteScreen->SetModulate((unsigned char)(255-(g_CurrentTime%1024)),255,255,255);
		pWhiteScreen->Render();
	}	

	if ((g_CurrentTime%600)<128)
	{
		pBlackScreen->SetModulate((unsigned char)(g_CurrentTime%600),255,255,255);
		pBlackScreen->Render();
	}	

	if ((g_CurrentTime%600)>127 && (g_CurrentTime%600)<255)
	{
		pBlackScreen->SetModulate((unsigned char)255-(g_CurrentTime%600),255,255,255);
		pBlackScreen->Render();
	}	

	if (g_CurrentTime<14000)
	{
		if ((g_CurrentTime%8000)>1000 && (g_CurrentTime%8000)<1500)	pPost01->Render();
		if ((g_CurrentTime%8000)>2000 && (g_CurrentTime%8000)<2500)	pPost02->Render();
		if ((g_CurrentTime%8000)>3000 && (g_CurrentTime%8000)<3500)	pPost03->Render();
		if ((g_CurrentTime%8000)>4000 && (g_CurrentTime%8000)<4500)	pPost04->Render();
		if ((g_CurrentTime%8000)>5000 && (g_CurrentTime%8000)<5500)	pPost05->Render();
		if ((g_CurrentTime%8000)>6000 && (g_CurrentTime%8000)<6500)	pPost07->Render();
	}

	// zawsze render kadru


	pKadr->Render();

	// wypisz fps
/*
	pFont->OutputText("FPS: ",10,10);
	_itoa(g_FrameRate,str,10);		// fps
	pFont->OutputText(str,50,10);

	_itoa(g_FrameCount,str,10);
	pFont->OutputText(str,10,50);

	pFont->OutputText("timer: ",10,100);
	_ui64toa(g_CurrentTime,str,10);
	pFont->OutputText(str,50,100);
*/
	// pomiary czasu

	g_EndTime = timeGetTime();

	if (g_EndTime - g_StartTime > 1000)
	{
		g_FrameRate = (g_FrameCount - g_StartFrameCount) * 1000 / (g_EndTime - g_StartTime);
		g_StartTime = g_EndTime;
		g_StartFrameCount = g_FrameCount;
	}

	g_FrameCount++;	
	
	// koniec rysowania

	pDevice->EndScene();	
	pDevice->Present(NULL, NULL, NULL, NULL);	
}

//=== obsluga okna

LRESULT CALLBACK BasicWindowProc(HWND wpHWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{	
		case WM_DESTROY: 
		{ 
			PostQuitMessage(0);
			return 0;
		} 
		break;

		case WM_KEYDOWN:
			switch( wParam ) 
			{
				case VK_ESCAPE:
					PostQuitMessage( 0 );
					break;
			}
		break;

		case WM_SETCURSOR:
            // wylacz kursor
            SetCursor(NULL);
        return TRUE;


		default:break; 
	} 

	return DefWindowProc(wpHWnd, msg, wParam, lParam);
}

//######################
//### start programu ###
//######################

int WINAPI WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine, int nCmdShow) 
{

	// tak wyglada na poczatku

	eStanDemo = LOADSCREEN;

	// pobierz aktualny katalog
	
	GetCurrentDirectory(sizeof(CurrentDirectory), CurrentDirectory);

	// zapytaj o pelny ekran ? 

	if (MessageBox(NULL, "Fullscreen?", "Fullscreen?", MB_YESNO)==IDYES)
		bFullScreen=true;
	else 
		bFullScreen=false;

//	bFullScreen = true;

	// inicjalizacja okienka

		WNDCLASSEX winClass;

	// ustaw i zarejestruj klase okna
	
	winClass.cbSize         = sizeof(WNDCLASSEX); 
	winClass.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc	= BasicWindowProc; // nasza statyczna funkcja
	winClass.cbClsExtra		= 0;
	winClass.cbWndExtra		= 0; 
	winClass.hInstance		= hInstance; 
	winClass.hIcon			= ::LoadIcon(hInstance, MAKEINTRESOURCE(DEMO_ICON));	// NULL zamiast icon
	winClass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
	winClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH); 
	winClass.lpszMenuName	= NULL; 
	winClass.lpszClassName	= "Demo";	//szClassName; 
	winClass.hIconSm        = ::LoadIcon(hInstance, MAKEINTRESOURCE(DEMO_ICON));	// NULL zamiast icon
	
	if (!RegisterClassEx(&winClass)) return -1;

	// stworz w odpowiedni sposob okno

	if (bFullScreen==false)	// zwykle okno
	{
				
		hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "Demo", "Demo", 
							 WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE, 
					 		 0, 0, 800, 600, NULL, NULL, hInstance, NULL);	
	}
	else	// fulscreen
	{
		hWnd = CreateWindowEx(NULL, "Demo", "Demo", WS_POPUP | WS_VISIBLE, 
					 		 0, 0, 800, 600, NULL, NULL, hInstance, NULL);	
	}

	CoInitialize(NULL);	// nie tylko jeden watek

	Direct3DInit();		// usunac, bez funkcji pomocniczej

	

	// inicjalizacja systemu audio

	pBassPlayer = new CBassPlayer(hWnd);

	pBassPlayer->LoadMP3("data\\demo.mp3");
	pBassPlayer->PlayMP3();

	// petla przechwytywania komunikatow

	MSG msg; 

	PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

	while(msg.message != WM_QUIT)
	{
       if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
       {
                   TranslateMessage( &msg );
                   DispatchMessage( &msg );
       }

		DrawScene();	// rysuj klatke 
	} 


	pBassPlayer->StopMP3();
	delete pBassPlayer;
	delete pFont;	// fonty
	
	if (pLoadScreen!=NULL)
	{
		delete pLoadScreen;	// start
		pLoadScreen=NULL;
	}
	
	if (pRadialZoom!=NULL)
	{
		delete pRadialZoom; // logo spd
		pRadialZoom = NULL;
	}

	if (pMetaballs!=NULL)
	{
		delete pMetaballs;
		pMetaballs = NULL;
	}

	if (pMeczet!=NULL)
	{
		delete pMeczet;
		pMeczet=NULL;
	}

	if (pTuba3d!=NULL)
	{
		delete pTuba3d;
		pTuba3d=NULL;
	}

	if (pKolumny!=NULL)
	{
		delete pKolumny;
		pKolumny=NULL;
	}

	if (pWalec!=NULL)
	{
		delete pWalec;
		pWalec=NULL;
	}

	if (pWieza!=NULL)
	{
		delete pWieza;
		pWieza=NULL;
	}

	if (pSpider!=NULL)
	{
		delete pSpider;
		pSpider=NULL;
	}
	
	if (pHelikopter!=NULL)
	{
		delete pHelikopter;
		pHelikopter=NULL;
	}

	if (pTunel3d!=NULL)
	{
		delete pTunel3d;
		pTunel3d=NULL;
	}

	if (pRing!=NULL)
	{
		delete pRing;
		pRing=NULL;
	}

	if (pAbstract01!=NULL)
	{
		delete pAbstract01;
		pAbstract01=NULL;
	}
	
	if (pMiasto!=NULL)
	{
		delete pMiasto;
		pMiasto=NULL;
	}

	if (pAbstract03!=NULL)
	{
		delete pAbstract03;
		pAbstract03=NULL;
	}

	if (pFur!=NULL)
	{
		delete pFur;
		pFur=NULL;
	}

	if (pAbstract02!=NULL)
	{
		delete pAbstract02;
		pAbstract02=NULL;
	}

	if (pTornado!=NULL)
	{
		delete pTornado;
		pTornado=NULL;
	}

	if (pFurBall!=NULL)
	{
		delete pFurBall;
		pFurBall=NULL;
	}

	if (pTheEnd!=NULL)
	{
		delete pTheEnd;
		pTheEnd=NULL;
	}

	// screeny

	if (pWaitScreen!=NULL)
	{
		delete pWaitScreen;
		pWaitScreen=NULL;
	}

	if (pWhiteScreen!=NULL)
	{
		delete pWhiteScreen;
		pWhiteScreen=NULL;
	}

	if (pBlackScreen!=NULL)
	{
		delete pBlackScreen;
		pBlackScreen=NULL;
	}

	if (pTheEnd!=NULL)
	{
		delete pTheEnd;
		pTheEnd=NULL;
	}

	// scrolls

	if (m_pLeftScroll!=NULL)
	{
		delete m_pLeftScroll;
		m_pLeftScroll=NULL;
	}
	

	pDevice->Release();
	pDirect3D->Release();

	return 0;
}
