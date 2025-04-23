// Fox v0.8 [alpha]
// by Jaros³aw Ro¿yñski
//============================

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dmusicc.h>
#include <dmusici.h>

#include "Audio.h"
#include "Font.h"
#include "Sound.h"
#include "Sprite.h"
#include "FileSystem.h"
#include "BankSFX.h"
#include "Event.h"
#include "EventEVT.h"

//=== zmienne dla directx

IDirect3D8			*pDirect3D;		// handler
IDirect3DDevice8	*pDevice;		// urzadzenie

HRESULT				hr;				// wynik operacji

HWND				hWnd;			// dla okna
HINSTANCE			hInstance;

bool				bFullScreen;	// czy pelny ekran ? 

//=== to dla myszki 

bool				leftButton = false;		// przyciski myszki
bool				rightButton = false;
bool				centerButton = false;

float				mouseX;			// wspolrzedne myszy
float				mouseY;

//=== aktualny katalog

char CurrentDirectory[200]		= {0,};	

//=== systemy plikow

CFileSystem			*pMouseFile;

//=== dla timera

int					g_FrameCount		= 0;
int					g_StartFrameCount	= 0;
DWORD				g_StartTime			= timeGetTime();
DWORD				g_EndTime			= g_StartTime;
int					g_FrameRate			= 0;

//=== dla systemu dzwiekowego

CAudio				*pAudio;

//=== sample

CSound				*pPik;
CSound				*pBounce;

//=== czcionki

CFont				*pFont;			// jakas czcionka

//=== sprite

CSprite				*pBubble;		// babelek ;>
CSprite				*pBackground;	// tlo

//=== EVT

CEventEVT			*pBobul;

//=== pomocnicza, inicjalizuje direct3d i ustawia rozne dodatki

bool Direct3DInit()
{

	pDirect3D = Direct3DCreate8(D3D_SDK_VERSION);

	if (pDirect3D==NULL) 
	{
		MessageBox(0,"B³¹d inicjalizacji DirectX 8.1 !","B³¹d!",MB_OK);
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
			MessageBox(0,"Nie mo¿na pobraæ informacji o aktualnym trybie DirectX","B³¹d!",MB_OK);
			return false;
		}
	}
	else
	{
		displayMode.Width = 800;				//iWidth;
		displayMode.Height = 600;				//iHeight;
		displayMode.RefreshRate = 0; 
		displayMode.Format = D3DFMT_R5G6B5;		// tu mozna zmienic
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

	// stworz urzadzenie

	hr=pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &presentParameters, &pDevice ); 

	if (FAILED(hr))	
	{
		MessageBox(0,"Nie mo¿na stworzyæ urz¹dzenia DirectX!","B³¹d!",MB_OK);		
		return false;
	}

	SetCursor(NULL);					// ukryj kursor myszy, rysujemy wlasny
	pDevice->ShowCursor(FALSE);			// przeslij do urzadzenia
	
	// ustaw kursor myszy na srodku ekranu

	pDevice->SetCursorPosition(400,300,D3DCURSOR_IMMEDIATE_UPDATE );	

	// ustaw macierze 

	D3DXMATRIX mat;

	// tutaj lepiej ustawic bez rzutowania typu (bez float)

	D3DXMatrixPerspectiveFovLH(&mat, D3DX_PI/4, (float)800/600,	1.0, 100.0);
	
	pDevice->SetTransform(D3DTS_PROJECTION, &(D3DMATRIX)mat);

	D3DXMatrixIdentity(&mat);

	// ustaw transformacje

	pDevice->SetTransform(D3DTS_WORLD, &(D3DMATRIX)mat);
	pDevice->SetTransform(D3DTS_VIEW, &(D3DMATRIX)mat);

    pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	// backface culling off
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			// swiatlo
	
	// blending
	
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// alpha channel
	
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	// macierz widoku

	D3DXMatrixTranslation(&mat, 0, 0, 4/3);
	pDevice->SetTransform(D3DTS_VIEW, &(D3DMATRIX)mat);

	// dodatkowe obiekty - powinno byc jak najmniej

	pFont = new CFont();
	pFont->Initialize(pDevice,(HFONT)GetStockObject(SYSTEM_FONT),D3DCOLOR_XRGB(0,255,255));

	pMouseFile = new CFileSystem("mouse.fox");
	pMouseFile->Load("bubble.tga");

	pBubble = new CSprite(255,255,255,255);
	//pBubble->Initialize(CurrentDirectory,pDevice,"combat.tga");

	pBubble->InitializeTGAinMemory((unsigned int*)pMouseFile->pDataBuffer,
		pMouseFile->Search("bubble.tga"),pDevice);
	
	pBubble->SetRotationCenter(16,16);

	pBackground = new CSprite(255,255,255,255);
	pMouseFile->Load("swamp.bmp");
	pBackground->InitializeTGAinMemory((unsigned int*)pMouseFile->pDataBuffer,
		pMouseFile->Search("swamp.bmp"),pDevice);

	return true;
}

//=== aktualizuj scenke, to ma byc wymienione i podzielone na czesci

void UpdateScene()
{
	char	str[20];	// tymczasowy

	pBackground->Render();
	pBubble->SetTranslation(mouseX,mouseY);
	pBubble->Render();

	if (rightButton) pBubble->AddRotation(0.05f);

	pBobul->Put(100,300,(g_FrameCount>>2)%50,1);
	pBobul->Put(200,300,(g_FrameCount>>2)%60,1);
	pBobul->Put(300,300,(g_FrameCount>>2)%70,1);
	pBobul->Put(400,300,(g_FrameCount>>2)%80,1);

	pBobul->Put((g_FrameCount>>1)%100,1);

	pFont->OutputText("FPS: ",10,10);
	
	_itoa(g_FrameRate,str,10);		// fps
	pFont->OutputText(str,50,10);

}

//=== rysuj scenke, tylko przygotowuje calosc, a potem odwoluje sie do odpowiedniej
//=== funkcji aktualizujacej w zaleznosci od CScene

void DrawScene()
{
	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0 );
	
	pDevice->BeginScene();
	
	
	UpdateScene();	// to zamienic na game,background itepe
	
	g_EndTime = timeGetTime();

	if (g_EndTime - g_StartTime > 1000)
	{
		g_FrameRate = (g_FrameCount - g_StartFrameCount) * 1000 / (g_EndTime - g_StartTime);
		g_StartTime = g_EndTime;
		g_StartFrameCount = g_FrameCount;
	}

	g_FrameCount++;	
	
	pDevice->EndScene();	//pDirect3D->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);	//pDirect3D->Present();
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
		} break;

		case WM_KEYDOWN:
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage( 0 );
					break;
		
				case VK_F1:
					pPik->Play(pAudio,0,0,0);
					break;
			}
			break;

		// myszka

		case WM_MOUSEMOVE:
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			break;

		case WM_LBUTTONDOWN:
			leftButton = true;
			pBounce->Play(pAudio,(-400+mouseX)/200,0,0);
			 break;

		case WM_LBUTTONUP:
			leftButton = false; 
			break;

		case WM_RBUTTONDOWN:
			rightButton = true;
			break;

		case WM_RBUTTONUP:
			rightButton = false;
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

	// pobierz aktualny katalog
	
	GetCurrentDirectory( sizeof(CurrentDirectory), CurrentDirectory);

	// zapytaj o pelny ekran ? 

	if (MessageBox(NULL, "Fullscreen?", "Fullscreen?", MB_YESNO)==IDYES)
		bFullScreen=true;
	else 
		bFullScreen=false;

	// inicjalizacja okienka

		WNDCLASSEX winClass;

	// ustaw i zarejestruj klase okna
	
	winClass.cbSize         = sizeof(WNDCLASSEX); 
	winClass.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc	= BasicWindowProc; // nasza statyczna funkcja
	winClass.cbClsExtra		= 0;
	winClass.cbWndExtra		= 0; 
	winClass.hInstance		= hInstance; 
	winClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);	// NULL zamiast icon
	winClass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
	winClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH); 
	winClass.lpszMenuName	= NULL; 
	winClass.lpszClassName	= "Kozio³ek Mato³ek";	//szClassName; 
	winClass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);
	
	if (!RegisterClassEx(&winClass)) return -1;

	// stworz w odpowiedni sposob okno

	if (bFullScreen==false)	// zwykle okno
	{
				
		hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "Kozio³ek Mato³ek", "Kozio³ek Mato³ek", 
							 WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE, 
					 		 0, 0, 800, 600, NULL, NULL, hInstance, NULL);	
	}
	else	// fulscreen
	{
		hWnd = CreateWindowEx(NULL, "Kozio³ek Mato³ek", "Kozio³ek Mato³ek", WS_POPUP | WS_VISIBLE, 
					 		 0, 0, 800, 600, NULL, NULL, hInstance, NULL);	
	}

	CoInitialize(NULL);	// nie tylko jeden watek

//    m_bitmap = (HBITMAP)LoadImage( GetModuleHandle( NULL ),
//                                       "c:\\projects\\Fox\\screen.bmp", IMAGE_BITMAP,
//                                         800, 600, LR_LOADFROMFILE );



	Direct3DInit();		// usunac, bez funkcji pomocniczej

	// inicjalizacja systemu audio

	pAudio = new CAudio();

	if( FAILED( pAudio->Init(CurrentDirectory) ) )
	{
		MessageBox(0, "Nie mo¿na zainicjalizowaæ Direct Audio!", "B³¹d!", MB_OK);
		return 0;
	}

	pAudio->PlayMusic(L"001.wav");


	pPik = new CSound(CurrentDirectory,pAudio,L"pik.wav");
	
	pBounce = new CSound(CurrentDirectory,pAudio,L"bounce.wav");

	pBobul = new CEventEVT("bobul.fox","bobul.evt",pDevice);
	
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
	
	return 0;
}
