// Scene3d.cpp: implementation of the CScene3d class.
//
//////////////////////////////////////////////////////////////////////

//#include <iostream.h>
#include "Scene3d.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CScene3d::CScene3d(IDirect3DDevice8 *pDevice, char cFilename [])
{

	m_pBackground=NULL;
	m_pBlackScreen=NULL;
	m_pWhiteScreen=NULL;

	m_pBackground = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground->InitializeEx(cFilename,pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
	D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	
	m_bActive = false;
	m_bFogEnable = false;
}

CScene3d::CScene3d(IDirect3DDevice8 *pDevice)
{
	m_pBackground=NULL;
	m_pBlackScreen=NULL;
	m_pWhiteScreen=NULL;
	
	m_bActive = false;
}


CScene3d::~CScene3d()
{
	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground=NULL;
	}

	if (m_pBlackScreen!=NULL)
	{
		delete m_pBlackScreen;
		m_pBlackScreen=NULL;
	}

	if (m_pWhiteScreen!=NULL)
	{
		delete m_pWhiteScreen;
		m_pWhiteScreen=NULL;
	}
}

void CScene3d::DeInitialize(void)
{
	scene->CloseScene();
	
	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground=NULL;
	}

	if (m_pBlackScreen!=NULL)
	{
		delete m_pBlackScreen;
		m_pBlackScreen=NULL;
	}

	if (m_pWhiteScreen!=NULL)
	{
		delete m_pWhiteScreen;
		m_pWhiteScreen=NULL;
	}

}

void CScene3d::Initialize(IDirect3DDevice8 *pDevice,char cFilename[])
{

		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}

void CScene3d::InitializeOut(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pBlackScreen = new CSprite(255,255,255,255);
		m_pBlackScreen->InitializeEx("data\\black.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}


void CScene3d::InitializeBlack(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pBlackScreen = new CSprite(255,255,255,255);
		m_pBlackScreen->InitializeEx("data\\black.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}

void CScene3d::InitializeWhite(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pWhiteScreen = new CSprite(255,255,255,255);
		m_pWhiteScreen->InitializeEx("data\\white.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}

void CScene3d::InitializeWhiteEnd(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pWhiteScreen = new CSprite(255,255,255,255);
		m_pWhiteScreen->InitializeEx("data\\white.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}

void CScene3d::InitializeBlackEnd(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pBlackScreen = new CSprite(255,255,255,255);
		m_pBlackScreen->InitializeEx("data\\black.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}

void CScene3d::InitializeBlackBlack(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pBlackScreen = new CSprite(255,255,255,255);
		m_pBlackScreen->InitializeEx("data\\black.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}

void CScene3d::InitializeWhiteWhite(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pWhiteScreen = new CSprite(255,255,255,255);
		m_pWhiteScreen->InitializeEx("data\\white.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}


void CScene3d::InitializeWhiteBlack(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pWhiteScreen = new CSprite(255,255,255,255);
		m_pWhiteScreen->InitializeEx("data\\white.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_pBlackScreen = new CSprite(255,255,255,255);
		m_pBlackScreen->InitializeEx("data\\black.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);


		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}

void CScene3d::InitializeBlackWhite(IDirect3DDevice8 *pDevice,char cFilename[])
{
		m_pWhiteScreen = new CSprite(255,255,255,255);
		m_pWhiteScreen->InitializeEx("data\\white.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

		m_pBlackScreen = new CSprite(255,255,255,255);
		m_pBlackScreen->InitializeEx("data\\black.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);


		m_bActive = true;	// dane wczytane
		
		scene = new G3D_CScene;
		scene->Initialize( pDevice );
	
		scene->Load(pDevice,cFilename);	

		scene->zMax = 50000.0f;
		scene->zMin = 5.0f;
		maxFrame = scene->dwFramesCount + 1;		
}

//=== ustawianie aktywnosci - czy dane sa w pamieci

void CScene3d::SetActive(bool bState)
{
	m_bActive = bState;
}

//=== zwraca informacje czy dane sa wczytane

bool CScene3d::bGetActive(void)
{
	return m_bActive;
}

int CScene3d::DrawScene(IDirect3DDevice8 *pDevice, long lTimer)
{

//		if (m_bFogEnable)	// mg³a w³¹czona
//		{
//			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
// 			pDevice->SetRenderState(D3DRS_FOGCOLOR, m_uiFogColor);
//			pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
//			pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&m_fFogStart));
//			pDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&m_fFogEnd));
//		}
//		else
//		{
//			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
//		}


	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	return 0;
		
}

int CScene3d::DrawSceneOn(IDirect3DDevice8 *pDevice, long lTimer)
{

	scene->RenderFrame( (float)lTimer );

	return 0;
		
}

int CScene3d::DrawSceneOutBlack(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart)
{

//		if (m_bFogEnable)	// mg³a w³¹czona
//		{
//			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
// 			pDevice->SetRenderState(D3DRS_FOGCOLOR, m_uiFogColor);
//			pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
//			pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&m_fFogStart));
//			pDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&m_fFogEnd));
//		}
//		else
//		{
//			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
//		}


	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	if (lTimer>lBlackOutStart)
	{
		m_pBlackScreen->SetModulate((unsigned char)(lTimer-lBlackOutStart),255,255,255);
		m_pBlackScreen->Render();

	}

//	if (lTimer>37750)
//	{
//		m_pBlackOutScreen->SetModulate((unsigned char)(lTimer-37750),255,255,255);
//		m_pBlackOutScreen->Render();
//	}

	return 0;
		
}

int CScene3d::DrawSceneOutWhite(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart)
{

	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	if (lTimer>=lBlackOutStart)
	{
		m_pWhiteScreen->SetModulate((unsigned char)(lTimer-lBlackOutStart),255,255,255);
		m_pWhiteScreen->Render();
	}

	return 0;
		
}

int CScene3d::DrawSceneInWhite(IDirect3DDevice8 *pDevice, long lTimer)
{

	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	if (lTimer<255)
	{
		m_pWhiteScreen->SetModulate((unsigned char)(255-lTimer),255,255,255);
		m_pWhiteScreen->Render();
	}

	return 0;
}

int CScene3d::DrawSceneInBlack(IDirect3DDevice8 *pDevice, long lTimer)
{

	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	if (lTimer<255)
	{
		m_pBlackScreen->SetModulate((unsigned char)(255-lTimer),255,255,255);
		m_pBlackScreen->Render();
	}

	return 0;
		
}

int CScene3d::DrawSceneBlackBlack(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart)
{

	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	if (lTimer<255)
	{
		m_pBlackScreen->SetModulate((unsigned char)(255-lTimer),255,255,255);
		m_pBlackScreen->Render();
	}

	if (lTimer>=lBlackOutStart)
	{
		m_pBlackScreen->SetModulate((unsigned char)(lTimer-lBlackOutStart),255,255,255);
		m_pBlackScreen->Render();
	}

	return 0;	
}

int CScene3d::DrawSceneWhiteWhite(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart)
{

	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	if (lTimer<255)
	{
		m_pWhiteScreen->SetModulate((unsigned char)(255-lTimer),255,255,255);
		m_pWhiteScreen->Render();
	}

	if (lTimer>=lBlackOutStart)
	{
		m_pWhiteScreen->SetModulate((unsigned char)(lTimer-lBlackOutStart),255,255,255);
		m_pWhiteScreen->Render();
	}

	return 0;	
}

int CScene3d::DrawSceneWhiteBlack(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart)
{

	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	if (lTimer<255)
	{
		m_pWhiteScreen->SetModulate((unsigned char)(255-lTimer),255,255,255);
		m_pWhiteScreen->Render();
	}

	if (lTimer>=lBlackOutStart)
	{
		m_pBlackScreen->SetModulate((unsigned char)(lTimer-lBlackOutStart),255,255,255);
		m_pBlackScreen->Render();
	}

	return 0;	
}

int CScene3d::DrawSceneBlackWhite(IDirect3DDevice8 *pDevice, long lTimer, long lBlackOutStart)
{

	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (m_pBackground!=NULL) m_pBackground->Render();	// rysuj tlo

	scene->RenderFrame( (float)lTimer );

	if (lTimer<255)
	{
		m_pBlackScreen->SetModulate((unsigned char)(255-lTimer),255,255,255);
		m_pBlackScreen->Render();
	}

	if (lTimer>=lBlackOutStart)
	{
		m_pWhiteScreen->SetModulate((unsigned char)(lTimer-lBlackOutStart),255,255,255);
		m_pWhiteScreen->Render();
	}

	return 0;	
}


bool CScene3d::bGetFogEnable(void)
{
	return m_bFogEnable;
}

void CScene3d::SetFog(bool bState)
{
	m_bFogEnable=bState;
}

float CScene3d::fGetFogStart(void)
{
	return m_fFogStart;
}

void CScene3d::SetFogStart(float fFogStart)
{
	m_fFogStart=fFogStart;
}

float CScene3d::fGetFogEnd(void)
{
	return m_fFogEnd;
}

void CScene3d::SetFogEnd(float fFogEnd)
{
	m_fFogEnd=fFogEnd;
}

unsigned int CScene3d::uiGetFogColor(void)
{
	return m_uiFogColor;
}

void CScene3d::SetFogColor(unsigned int uiFogColor)
{
	m_uiFogColor=uiFogColor;
}


