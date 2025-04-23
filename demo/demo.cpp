// TODO : 
// - scenka odtwarzana w wielu przedzialach czasu
// - obraz wyswietlany w wielu przedzialach czasu
// - wywalic evt_limit i sprite_player_limit, niech sam zlicza automatycznie ze skryptu
// - metaballs wyswietlane w wielu przedzialach czasu
// - scene player wyrzucic jako osobna klase
// - sprite player wyrzucic jako osobna klase
// - .ase pliki
// - kamera dla plikow .ase
// - zip sprite
// - zip animo


// general include

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <ctype.h>

// dodatkowe
#include "c:\eprojects\g3d\include\texture.h"

#include "Font.h"
#include "LoadScreen.h"
#include "Scene3d.h"
#include "MetaballsFX.h"
#include "RadialZoom.h"
#include "FXTemp.h"
#include "VShader.h"
#include "Event.h"
#include "EventEVT.h"
#include "Zip.h"
#include "Unzip.h"

#include "ZipSprite.h"
#include "ZipButton.h"
#include "ZipDragButton.h"

#include "GLizer.h"
#include "Matrix.h"
#include "Vector.h"
#include "textureRenderTarget.h"		// dla motion blur
#include "VShaderPlayer.h"				// odtwarza scenki z vshaderem
#include "AnimPlayer.h"					// player do animacji
#include "Debug.h"						// flaga debug
#include "bassplayer.h"					// bass player
#include "Resource.h"					// ikonki, okienka 

#include "texture.h"
#include "3ds.h"

#define FIELD_OF_VIEW 45.f			// angle in vertical direction
#define Z_NEAR_VIEW_PLANE 0.1f
#define Z_FAR_VIEW_PLANE 1000.f


//=== testowe

CEventEVT *pAnim1;
CAnimPlayer *pAnimPlayer;

//=== opoznienie

long lDelay = 0;

//=== zmienne dla directx

D3DLIGHT8			m_light;

IDirect3D8			*pDirect3D;		// handler
IDirect3DDevice8	*pDevice;		// urzadzenie

HRESULT				hr;				// wynik operacji

HWND				hWnd;			// dla okna
HINSTANCE			hInstance;


bool				bFullScreen;	// czy pelny ekran ? 
bool				bLoadScreen;	// czy ekran wczytywania ? 

char CurrentDirectory[512]		= {0,};	// aktualny katalog

char				cLoadScreenName[256];	// nazwa pliku do wczytania ekranu

bool				bScreenSection;
int					iXScreen;
int					iYScreen;
int					iRefresh;
int					iDisplayMode;

//=== dla timera

int					g_FrameCount		= 0;
int					g_StartFrameCount	= 0;
unsigned long		g_StartTime			= timeGetTime();
unsigned long		g_DemoTime			= 0;
unsigned long		g_EndTime			= g_StartTime;
unsigned long		g_CurrentTime		= 0;
int					g_FrameRate			= 0;
long				lTimer=0;
long				lTimer10=0;
long				lLocalTimer = 0;	// timer okresla ktora scene odtwarzac, a local timer klatke
long				lEndTimer = 0;		// jak timer osiagnie ta pozycje to program konczy sie


//=== czcionki

CFont				*pFont=NULL;			// jakas czcionka

//=== efekty

CMetaballsFX		*pMetaballs = NULL;
CVShaderPlayer		*pVShaderPlayer = NULL;		// odtwarzacz dla scenek .x
CRadialZoom			*pRadialZoom = NULL;


//=== player scenek 3d

CScene3d			*pScenePlayer[50];
int					iStartPlayer[50];
int					iEndPlayer[50];
char				cScenePlayerFilename[50][256];

CSprite				*pBlur;

//=== player do obrazkow

CSprite				*pSpritePlayer[256];

int					iSpritePlayerStart[256];	// od kiedy ma sie pojawic obrazek
int					iSpritePlayerEnd[256];		// do kiedy ma sie pojawiæ obrazek
int					iSpritePlayerLoadTime[256];	// kiedy ma zostaæ wczytany obrazek

int					iSpritePlayerX[256];	// poczatkowa wspolrzedna X
int					iSpritePlayerY[256];	// poczatkowa wspolrzedna Y
int					iSpritePlayerXZ[256];	// poczatkowa skala X
int					iSpritePlayerYZ[256];	// poczatkowa skala Y

int					iSpritePlayerXAdd[256];		// o ile ma sie zwiekszac X na tik zegara
int					iSpritePlayerYAdd[256];		// o ile ma sie zwiekszac Y na tik zegara
int					iSpritePlayerXZAdd[256];	// o ile ma sie zwiekszac skala X na tik zegara
int					iSpritePlayerYZAdd[256];	// o ile ma sie zwiekszac skala Y na tik zegara


int					iSpritePlayerRotationAdd[256]; // o ile ma sie zwiekszac kat
int					iSpritePlayerRotationX[256];	// wspolrzedne na teksturze do obrotu
int					iSpritePlayerRotationY[256];	// wspolrzedne na teksturze do obrotu

int					iSpritePlayerCount=0;	// ile jest obrazkow
char				cSpritePlayerFilename[256][256];
int					iSpritePlayerFade[256];				

//=== standardowe nakladki

CSprite				*pBackground = NULL;
CSprite				*pForeground = NULL;

//=== screen loading

CSprite				*pWaitScreen=NULL;

//=== fade screens

CSprite				*pWhiteScreen=NULL;
CSprite				*pBlackScreen=NULL;

//=== kadry

CSprite				*pKadr=NULL;

//=== switch efektów

int	iCurrentScene=0;	// aktualnie odgrywana scenka

//=== player

CBassPlayer			*pBassPlayer;

//=== dodatkowe


int					error;
int					screen_x, screen_y;
bool				fullscreen;
int					software_vertexprocessing;

int					iLicznik;
long				h1,h2,h3,h4;	// do petli
float				f1,f2,f3;
long				lResetTime=300000;	// jak reset time > end time to demo nie jest powtarzane

//=== fog

float			m_fFogStart=5.0f;	// 5.0f
float			m_fFogEnd=5000.0f;	// 200.0f

unsigned int	m_uiFogColor=0x00000000;	// must be set, default 0x000f0f0f

// dla motion blur

textureRenderTarget render_target;

//=========================================================================================================================
// rysowanie kwadratowej textury na ekranie, potrzebne do blura
//=========================================================================================================================

void DrawQuad(vec p, vec n, vec s, vec t, vec4 color)
{
	GLizer gl;
	gl.glBegin(D3DPT_TRIANGLESTRIP, 1, 1, 1);    // 3  4  poradie bodov
		gl.glNormal(n);                            // 1  2
		gl.glColor(color);
		gl.glTexCoord(0,0); gl.glVertex( p );
		gl.glTexCoord(1,0); gl.glVertex( p+s );
		gl.glTexCoord(0,1); gl.glVertex( p+t );
		gl.glTexCoord(1,1); gl.glVertex( p+s+t );
	gl.glEnd(pDevice);
}

//=========================================================================================================================
// rozszerzenie textury do wielkosci ekranu
//=========================================================================================================================

void ReSizeDXScene(int width, int height)
{

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, FIELD_OF_VIEW*PI180, (float)width/(float)height, Z_NEAR_VIEW_PLANE, Z_FAR_VIEW_PLANE );
	pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

//=== zgrywa zawartosc ekranu na dysk =======================================================================

void TakeScreenShot(IDirect3DDevice8* device, char *file_name, int screenx, int screeny, long lTimer)
{
		IDirect3DSurface8 *frontbuf;	// wskaznik do pamieci, tutaj kopia ekranu
		RECT	pMyRect;
		D3DDISPLAYMODE mode;
		char	cFilename[100];


		if (FAILED(hr=device->GetDisplayMode(&mode)))
		return ;

		device->CreateImageSurface(mode.Width,mode.Height,D3DFMT_A8R8G8B8,&frontbuf);

		// kopiowanie bufora do surface

		HRESULT hr = device->GetFrontBuffer(frontbuf);		
		
		// sprawdzenie czy sie udalo

		if (hr!=D3D_OK)
		{
			frontbuf->Release();
			return;
		}

		// zapisz nasz obraz na dysk
		// ostatnie dwa parametry sa NULL, poniewaz chcemy caly front buffer

		pMyRect.left = 0; 
		pMyRect.top = 0;
		pMyRect.right = 1024;
		pMyRect.bottom = 768;

		sprintf(cFilename,"image%d.bmp",lTimer);

		D3DXSaveSurfaceToFile(cFilename,D3DXIFF_BMP, frontbuf, NULL,NULL);

		// zwolnij surface

		frontbuf->Release();
}


// parsowanie skryptu

void UpOnly(char str[])
{
	int i=0;
	char c;
	
	while (str[i])
	{
		c = str[i];
		str[i]= toupper(c);
		i++;
	}
}


void Parse(void)
{
	FILE	*plik;	
	char	linia[1024];
	int		iSceneCount = 0;

	// ustaw domyslne parametry

	bLoadScreen = false;
	bScreenSection = false;

	// sprawdz czy mozna odczytac plik

	if ((plik=fopen("demo.ini","r"))==NULL)
	{
		MessageBox(NULL,"ERROR! Brak pliku demo.ini!","ERROR", MB_OK);
		PostQuitMessage( 0 );
	}

	while (strcmp(linia,"</DEMO>"))
	{
		fscanf(plik,"%s\n",&linia);	// wczytaj linie pliku
		UpOnly(linia);

		// rozpoczecie skryptu
		if (!strcmp(linia,"<DEMO>"))
		{
			fscanf(plik,"%s\n",&linia);
			UpOnly(linia);
			
			if (!strcmp(linia,"DEBUG"))
			{
				bFullScreen=false;
			}
			else
			{
				bFullScreen=true;
			}

		} // <DEMO>

		if (!strcmp(linia,"<RESET>"))	// kiedy demo sie resetuje
		{
			fscanf(plik,"%d\n",&lResetTime);
		}

		if (!strcmp(linia,"<END>"))	// koniec odtwarzania
		{
			fscanf(plik,"%d\n",&lEndTimer);
		}

		// ekran wczytywania	
	
		if (!strcmp(linia,"<LOADSCREEN>"))
		{
			fscanf(plik,"%s\n",&cLoadScreenName);
			bLoadScreen = true;			
		} // <LOADSCREEN>

		// ustawienia wyswietlania

		if (!strcmp(linia,"<SCREEN>"))
		{
			fscanf(plik,"%d\n",&iXScreen);
			fscanf(plik,"%d\n",&iYScreen);

			fscanf(plik,"%s\n",&linia);
			UpOnly(linia);

			if (!strcmp(linia,"AUTO"))
			{
				iRefresh = 0; 
			}
			else
			{
				fscanf(plik,"%d\n",&iRefresh);
			}

			fscanf(plik,"%d\n",&iDisplayMode);
			
			bScreenSection = true;
		}  // <SCREEN>
	} // <DEMO>
}

void ParseScenes(void)
{
	FILE	*plik;	
	char	linia[1024];
	int		iSceneCount = 0;

	// najpierw czyszczenie wskaznikow

	for (int i=0; i<50; i++)
	{
		pScenePlayer[i] = NULL;
	}

	for (int i=0; i<256; i++)
	{
		pSpritePlayer[i] = NULL;
	}

	// teraz otwieramy plik

	if ((plik=fopen("demo.ini","r"))==NULL)
	{
		MessageBox(NULL,"ERROR! Brak pliku demo.ini!","ERROR", MB_OK);
		PostQuitMessage( 0 );
	}

	while (strcmp(linia,"</DEMO>"))
	{
		fscanf(plik,"%s\n",&linia);	// wczytaj linie pliku
		UpOnly(linia);

		// teraz jakie scenki

		if (!strcmp(linia,"<3D_SCENE>"))
		{
			fscanf(plik,"%s\n",&linia);
			UpOnly(linia);
		
			if (!strcmp(linia,"STATIC"))
			{
				fscanf(plik,"%s\n",&linia);	//nazwa pliku
				pScenePlayer[iSceneCount] = new CScene3d(pDevice); // nowa scenka
				pScenePlayer[iSceneCount]->SetDynamic(false);	// mozna wczytac od razu
				pScenePlayer[iSceneCount]->ParseFromFile(pDevice,linia); // wczytanie scenki na podstawie pliku.ini
				strcpy(cScenePlayerFilename[iSceneCount],linia);	// skopiowanie nazwy scenki na potem
				fscanf(plik,"%d\n",&iStartPlayer[iSceneCount]);
				fscanf(plik,"%d\n",&iEndPlayer[iSceneCount]);
				iSceneCount++;	// zwiekszamy liczbe scenek
			}

			if (!strcmp(linia,"DYNAMIC"))
			{
				fscanf(plik,"%s\n",&linia);	//nazwa pliku
				pScenePlayer[iSceneCount] = new CScene3d(pDevice); // nowa scenka
				pScenePlayer[iSceneCount]->SetDynamic(true);	// mozna wczytac od razu
				pScenePlayer[iSceneCount]->ParseFromFile(pDevice,linia); // wczytanie scenki na podstawie pliku.ini
				strcpy(cScenePlayerFilename[iSceneCount],linia);	// skopiowanie nazwy scenki na potem
				fscanf(plik,"%d\n",&iStartPlayer[iSceneCount]);
				fscanf(plik,"%d\n",&iEndPlayer[iSceneCount]);
				iSceneCount++;	// zwiekszamy liczbe scenek
			}

		} //3D_SCENE

		if (!strcmp(linia,"<IMAGE>"))		// nakladka
		{
			fscanf(plik,"%s\n",&cSpritePlayerFilename[iSpritePlayerCount]);


			// wspolrzedne X i Y, skala XZ, skala YZ

			fscanf(plik,"%d %d %d %d\n",&iSpritePlayerX[iSpritePlayerCount], &iSpritePlayerY[iSpritePlayerCount],
				&iSpritePlayerXZ[iSpritePlayerCount], &iSpritePlayerYZ[iSpritePlayerCount]);

			// inkrementacja X i Y

			fscanf(plik,"%d %d %d %d\n",&iSpritePlayerXAdd[iSpritePlayerCount], &iSpritePlayerYAdd[iSpritePlayerCount],
				&iSpritePlayerXZAdd[iSpritePlayerCount], &iSpritePlayerYZAdd[iSpritePlayerCount]);

			// inkrementacja rotacji

			fscanf(plik,"%d %d %d\n",&iSpritePlayerRotationAdd[iSpritePlayerCount],
				&iSpritePlayerRotationX[iSpritePlayerCount],
				&iSpritePlayerRotationY[iSpritePlayerCount]);

			// poczatek i koniec wyswietlania

			fscanf(plik,"%d\n",&iSpritePlayerStart[iSpritePlayerCount]);	// timer - poczatek wyswietlania
			fscanf(plik,"%d\n",&iSpritePlayerEnd[iSpritePlayerCount]);		// timer - koniec wyswietlania
			fscanf(plik,"%d\n",&iSpritePlayerLoadTime[iSpritePlayerCount]);	// timer - kiedy wczytujemy dane

			// czy jest fade

			fscanf(plik,"%s\n",&linia);	//nazwa pliku
			if (!strcmp(linia,"NOFADE")) iSpritePlayerFade[iSpritePlayerCount]=0;		// nakladka
			if (!strcmp(linia,"INFADE")) iSpritePlayerFade[iSpritePlayerCount]=1;		// nakladka
			if (!strcmp(linia,"OUTFADE")) iSpritePlayerFade[iSpritePlayerCount]=2;		// nakladka

			iSpritePlayerCount++;
		}

	} // <DEMO>
}



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
		if (bScreenSection)
		{
			displayMode.Width = iXScreen;
			displayMode.Height = iYScreen;
			
			if (iRefresh!=0) displayMode.RefreshRate = iRefresh;

			switch(iDisplayMode)
			{
			case 1:
				displayMode.Format = D3DFMT_R8G8B8;
				break;
			case 2:
				displayMode.Format = D3DFMT_A8R8G8B8;
				break;
			case 3:
				displayMode.Format = D3DFMT_X8R8G8B8;
				break;
			case 4:
				displayMode.Format = D3DFMT_R5G6B5;
				break;
			case 5:
				displayMode.Format = D3DFMT_X1R5G5B5;
				break;
			case 6:
				displayMode.Format = D3DFMT_A1R5G5B5;
				break;
			case 7:
				displayMode.Format = D3DFMT_A4R4G4B4;
				break;
			case 8:
				displayMode.Format = D3DFMT_A8;
				break;
			case 9:
				displayMode.Format = D3DFMT_R3G3B2;
				break;
			case 10:
				displayMode.Format = D3DFMT_A8R3G3B2;
				break;
			case 11:
				displayMode.Format = D3DFMT_X4R4G4B4;
				break;
			case 12:
				displayMode.Format = D3DFMT_A2B10G10R10;
				break;
			case 13:
				displayMode.Format = D3DFMT_G16R16;
				break;
			case 14:
				displayMode.Format = D3DFMT_A8P8;
				break;
			case 15:
				displayMode.Format = D3DFMT_P8;
				break;
			case 16:
				displayMode.Format = D3DFMT_L8;
				break;
			case 17:
				displayMode.Format = D3DFMT_A8L8;
				break;
			case 18:
				displayMode.Format = D3DFMT_A4L4;
				break;
			}
		}
		else
		{
			displayMode.Width = iXScreen;				//iWidth;
			displayMode.Height = iYScreen;				//iHeight;
			displayMode.Format = D3DFMT_A8R8G8B8;	//R5G6B5;		// tu mozna zmienic
		}
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
	presentParameters.BackBufferCount = 1;
	presentParameters.EnableAutoDepthStencil = TRUE;			// to blokuje Z-Buffer
	presentParameters.AutoDepthStencilFormat = D3DFMT_D24X8;
	presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;

	// stworz urzadzenie

	hr=pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_HARDWARE_VERTEXPROCESSING,
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

	D3DXMatrixPerspectiveLH(&mat, 2.3f, 3.3f*3/4, 0.5f, 10);	
	pDevice->SetTransform(D3DTS_PROJECTION, &(D3DMATRIX)mat);

	D3DXMatrixIdentity(&mat);

	// ustaw transformacje

	pDevice->SetTransform(D3DTS_WORLD, &(D3DMATRIX)mat);
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

	// jezeli jest ekran wczytywania to dajemy

	if (bLoadScreen)
	{
		pWaitScreen = new CSprite(255,255,255,255);	// nowy sprite
		pWaitScreen->InitializeEx(cLoadScreenName,pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	}

	pWhiteScreen = new CSprite(255,255,255,255);	// nowy sprite
	pWhiteScreen->InitializeEx("data\\white.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pBlackScreen = new CSprite(255,255,255,255);	// nowy sprite
	pBlackScreen->InitializeEx("data\\black.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	pKadr = new CSprite(255,255,255,255);	// nowy sprite
	pKadr->InitializeEx("data\\kadr.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	// tylko kiedy ma byc ekran wczytywania

	if (bLoadScreen)
	{
		pDevice->BeginScene();
		pWaitScreen->Render();	// jedno wyswietlenie

		pDevice->EndScene();
		pDevice->Present(NULL, NULL, NULL, NULL);	

		if (pWaitScreen!=NULL)
		{
			delete pWaitScreen;
			pWaitScreen=NULL;
		}
	}

	// wczytywanie 

	SetTexturePath("data");
	InitializeTextureSystem( pDevice );		

	// inicjalizacja scenek

	ParseScenes();

//=== inicjalizacja rotozoom

	pRadialZoom = new CRadialZoom();
	pRadialZoom->Initialize(pDevice);

//============================================================= inicjalizacja metaballs


//	pMetaballs = new CMetaballsFX();
//	pMetaballs->Initialize(pDevice,"data\\metaballs.jpg");

	// inicjalizacja scene X

	pVShaderPlayer = new CVShaderPlayer();
	pVShaderPlayer->Parse("demo.ini");

	pAnimPlayer = new CAnimPlayer();
	pAnimPlayer->Initialize(pDevice);

	pBlur = new CSprite(128,0,0,0);
	pBlur->InitializeEx("data\\kadr.png",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	if(!render_target.Init(hWnd,pDevice,1024,1024,24))
	{
		MessageBox(NULL,"ERROR! Cannot init reder_target texture","ERROR", MB_OK);			
	}

	return true;
}

//============================================================================================== aktualizuj stan dema
//=== rysuj scenke, tylko przygotowuje calosc, a potem odwoluje sie do odpowiedniej
//=== funkcji aktualizujacej w zaleznosci od CScene

void DrawScene()
{

	char str[200];

	if (g_DemoTime==0) g_DemoTime=timeGetTime();

	if (lTimer>=lResetTime) g_DemoTime= g_CurrentTime;

	g_CurrentTime = lDelay + ((timeGetTime()-g_DemoTime)/10);

	pDevice->BeginScene();
	
	render_target.Begin(pDevice);
	// sprawdz najpierw stan gry

	lTimer = (long)((g_CurrentTime));
	lTimer10 = lTimer/5;

	// sprawdz wszystkie scenki czy teraz pora na rysowanie

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE );
	for (int i=0; i<50; i++)
	{
		if (pScenePlayer[i]!=NULL && lTimer>=iStartPlayer[i] && lTimer<iEndPlayer[i]) // czy teraz odtwarzamy scenke?
		{
			lLocalTimer = lTimer - iStartPlayer[i];
			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
			pScenePlayer[i]->DrawScene(pDevice,lLocalTimer);
		}
	}

	// zwolnij scenki ktore nie sa juz uzywane

	for (int i=0; i<50; i++)
	{
		if (lTimer > iEndPlayer[i])
		{
			delete pScenePlayer[i];
			pScenePlayer[i] = NULL;
		}
	}

	//=== radial zoom

	if (lTimer<1631) pRadialZoom->DrawScene(lTimer);

//============================================================ wtracenie

	// metaballs
	
//	if (lTimer>1630 && lTimer<3250)
//	{
//		pMetaballs->DrawScene(pDevice,lTimer);
//	}


//============================================================= koniec wtracenia

	pVShaderPlayer->Play(lTimer,pDevice);

	// narysuj obrazki ktore sa potrzebne

	for (int i=0; i<256; i++)
	{
		if (lTimer>iSpritePlayerEnd[i] && pSpritePlayer[i]!=NULL) // jezeli minal czas to zwalniamy pamiec
		{
			delete pSpritePlayer[i];
			pSpritePlayer[i] = NULL;
		}

		if (lTimer>iSpritePlayerLoadTime[i] && lTimer<iSpritePlayerEnd[i] && pSpritePlayer[i]==NULL)	// czy pora na wczytanie obrazka ?
		{
				pSpritePlayer[i] = new CSprite(255,255,255,255);
				pSpritePlayer[i]->InitializeEx(cSpritePlayerFilename[i],pDevice,
				0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
		}

		if (lTimer>=iSpritePlayerStart[i] && lTimer<iSpritePlayerEnd[i])
		{
			if (pSpritePlayer[i]==NULL) // doczytanie obrazka jezeli jeszcze nie ma w pamieci
			{
				pSpritePlayer[i] = new CSprite(255,255,255,255);
				pSpritePlayer[i]->InitializeEx(cSpritePlayerFilename[i],pDevice,
				0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
			}
			
			// zmiana polozenia

			pSpritePlayer[i]->SetTranslation((float)iSpritePlayerX[i]+(((float)(iSpritePlayerXAdd[i]*(lTimer-iSpritePlayerStart[i])))/100),
				(float)iSpritePlayerY[i]+(((float)(iSpritePlayerYAdd[i]*(lTimer-iSpritePlayerStart[i])))/100));

			// zmiana skali

			pSpritePlayer[i]->SetScale((float)(iSpritePlayerXZ[i]/100)+(((float)(iSpritePlayerXZAdd[i]*(lTimer-iSpritePlayerStart[i])))/10000),
				(float)(iSpritePlayerYZ[i]/100)+(((float)(iSpritePlayerYZAdd[i]*(lTimer-iSpritePlayerStart[i])))/10000));

			// zmiana kata

			pSpritePlayer[i]->SetRotationCenter(iSpritePlayerRotationX[i],iSpritePlayerRotationY[i]);
			pSpritePlayer[i]->SetRotation(((float)(iSpritePlayerRotationAdd[i]*(lTimer-iSpritePlayerStart[i])*0.01))/360);

			if (iSpritePlayerFade[i]==0) pSpritePlayer[i]->SetModulate(255,255,255,255);

			// fade in

			if (iSpritePlayerFade[i]==1) 
			{
				if ((lTimer-iSpritePlayerStart[i])<255)
				{
					pSpritePlayer[i]->SetModulate((unsigned char)lTimer-iSpritePlayerStart[i],255,255,255);
				}
				else
				{
					pSpritePlayer[i]->SetModulate(255,255,255,255);
			
				}
			}

			// out fade

			if (iSpritePlayerFade[i]==2) 
			{
				if ((255-(lTimer-iSpritePlayerStart[i]))>0)
				{
					pSpritePlayer[i]->SetModulate(255-(lTimer-iSpritePlayerStart[i]),
						255-(lTimer-iSpritePlayerStart[i]),255-(lTimer-iSpritePlayerStart[i]),
						255-(lTimer-iSpritePlayerStart[i]));
				}
				else
				{
					pSpritePlayer[i]->SetModulate(0,0,0,0);
				}
			}

			pSpritePlayer[i]->Render();
		}
	}

	// jezeli sa jakies animacje to teraz je odtwarzamy

	pAnimPlayer->DrawScene(pDevice,lTimer);

	// zawsze render kadru

	pKadr->Render();

	// wypisz fps

	if (!bFullScreen)
	{
		pFont->OutputText("FPS: ",10,10);
		_itoa(g_FrameRate,str,10);		// fps
		pFont->OutputText(str,50,10);

		_itoa(g_FrameCount,str,10);
		pFont->OutputText(str,10,50);

		pFont->OutputText("timer: ",10,100);
		_ui64toa(g_CurrentTime,str,10);
		pFont->OutputText(str,50,100);

		// wypisz numer scenki, jezeli teraz jakas odtwarzamy

		for (int i=0; i<50; i++)
		{
			if (pSpritePlayer[i]!=NULL && lTimer>=iSpritePlayerStart[i] && lTimer<iSpritePlayerEnd[i]) // czy teraz odtwarzamy scenke?
			{
				char tmp[16];

				pFont->OutputText(cSpritePlayerFilename[i],10,700);

				sprintf(tmp,"%d",iSpritePlayerXAdd[i]);
				pFont->OutputText(tmp,100,700);
				sprintf(tmp,"%d",iSpritePlayerYAdd[i]);
				pFont->OutputText(tmp,150,700);

			}

			if (pScenePlayer[i]!=NULL && lTimer>=iStartPlayer[i] && lTimer<iEndPlayer[i]) // czy teraz odtwarzamy scenke?
			{
				pFont->OutputText(cScenePlayerFilename[i],512,700);
			}
		} // for

	}

	// pomiary czasu

	g_EndTime = timeGetTime();

	if (g_EndTime - g_StartTime > 1000)
	{
		g_FrameRate = (g_FrameCount - g_StartFrameCount) * 1000 / (g_EndTime - g_StartTime);
		g_StartTime = g_EndTime;
		g_StartFrameCount = g_FrameCount;
	}

	g_FrameCount++;	

	// sprawdzamy czy to koniec

	if (lTimer>=lEndTimer) PostQuitMessage(0);

	if (lTimer>=lResetTime) lTimer = 0;

	// koniec rysowania

	render_target.End();

	pDevice->EndScene();	
	pDevice->Present(NULL, NULL, NULL, NULL);	

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		           D3DCOLOR_XRGB(24,24,24), 1.0f, 0);

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
				
				case VK_F4:
					TakeScreenShot(pDevice,"xxx",iXScreen, iYScreen,lTimer);
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

	error = 0;	// ta zmienna jest uzywana przez klasy zewnetrzne
	screen_x = 1024;
	screen_y = 768;
	fullscreen = true;
	software_vertexprocessing = 0;

	// pobierz aktualny katalog
	
	GetCurrentDirectory(sizeof(CurrentDirectory), CurrentDirectory);

	// parsuj skrypt

	Parse();

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
					 		 0, 0, iXScreen, iYScreen, NULL, NULL, hInstance, NULL);	
	}
	else	// fulscreen
	{
		hWnd = CreateWindowEx(NULL, "Demo", "Demo", WS_POPUP | WS_VISIBLE, 
					 		 0, 0, iXScreen, iYScreen, NULL, NULL, hInstance, NULL);	
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

	for (int i=0; i<50; i++)
	{
		if (pScenePlayer[i]!=NULL)
		{
			delete pScenePlayer[i];
			pScenePlayer[i]=NULL;
		}
	}

	for (int i=0; i<256; i++)
	{
		if (pSpritePlayer[i]!=NULL)
		{
			delete pSpritePlayer[i];
			pSpritePlayer[i]=NULL;
		}
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

	delete pRadialZoom;

//	delete pMetaballs;

	delete pVShaderPlayer;
	delete pAnimPlayer;

	pDevice->Release();
	pDirect3D->Release();

	return 0;
}
