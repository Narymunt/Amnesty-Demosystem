// MetaballsFX.cpp: implementation of the CMetaballsFX class.
//
//////////////////////////////////////////////////////////////////////

//#include <iostream.h>
#include "MetaballsFX.h"
#include "Graphics.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMetaballsFX::CMetaballsFX()
{
	m_pMetaballs=NULL;
	m_bActive=false;
}

CMetaballsFX::~CMetaballsFX()
{
	if (m_pMetaballs!=NULL)
	{
		delete m_pMetaballs;
		m_pMetaballs=NULL;
	}

	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground=NULL;
	}

	m_bActive=false;

}

void CMetaballsFX::DeInitialize(void)
{
	if (m_pMetaballs!=NULL)
	{
		delete m_pMetaballs;
		m_pMetaballs=NULL;
	}

//	m_pIndexBuffer->Release();
//	m_pVertexBuffer->Release();

	// zwolnij tlo jezeli uzywane
	
	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground=NULL;
	}

	m_bActive=false;

}

void CMetaballsFX::Initialize(IDirect3DDevice8 *pDevice, char cFilename[])
{
	
	HRESULT	hr;

	

	m_pMetaballs = new CMetaballs();
	m_pMetaballs->SetGridSize(64);

	CMarchingCubes::BuildTables();

	CGraphics *pGfx = new CGraphics();
	pGfx->Initialize(pDevice);
	pGfx->CreateDevice(pDevice);

	IDirect3DDevice8 *pDev = pGfx->GetD3DDevice();

	hr = pGfx->CreateVertexBuffer(MAX_VERTICES, sizeof(SVertex), 
		                          FVF_VERTEX, D3DPOOL_DEFAULT);
	if( FAILED(hr) ) return;

	hr = pGfx->CreateIndexBuffer(MAX_INDICES, D3DPOOL_DEFAULT);
	
	if( FAILED(hr) ) return;

	D3DXMATRIX Mtx;
	D3DXMatrixPerspectiveLH(&Mtx, 0.3f, 0.3f*3/4, 0.5f, 10);
	pDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&Mtx);
	D3DXMatrixTranslation(&Mtx, 0,0,2.0f);
	pDev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&Mtx);
	pDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Ligthing
	D3DLIGHT8 Light;
	Light.Type         = D3DLIGHT_DIRECTIONAL;
	Light.Diffuse      = D3DXCOLOR(0.1f,0.01f,0.01f,1);
	Light.Specular     = D3DXCOLOR(0,0,0,0);
	Light.Ambient      = D3DXCOLOR(0,0,0,0);
	Light.Direction    = D3DXVECTOR3(0,0,5);
	pDev->SetLight(0, &Light);

	// Material
	D3DMATERIAL8 Mat;
	Mat.Diffuse  = D3DXCOLOR(0.1f,0.1f,0.1f,1);
	Mat.Specular = D3DXCOLOR(1,1,1,1);
	Mat.Ambient  = D3DXCOLOR(0,0,0,1);
	Mat.Emissive = D3DXCOLOR(0,0,0,1);
	Mat.Power = 25.0f;
	pDev->SetMaterial(&Mat);

	// Enable ligthing 
	pDev->LightEnable(0, TRUE);
	pDev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

	// Setup the lighting engine to fetch material properties from the material
//	pDev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE , D3DMCS_MATERIAL);
//	pDev->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
//	pDev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE , D3DMCS_MATERIAL);
//	pDev->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

	// Setup the texture for spheremapping
	hr = D3DXCreateTextureFromFile(pDevice, cFilename, &m_pTexture);
	if( SUCCEEDED(hr) )
		pDev->SetTexture(0, m_pTexture);

	pDev->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	pDev->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	pDev->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	pDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_ADD);
	pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	m_pBackground = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground->InitializeEx("data\\metaback.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	fDiff=0;

	m_fModifier=0.001f;

	m_bActive=true;

}

int CMetaballsFX::DrawScene(IDirect3DDevice8 *pDevice, long lTimer)
{

	fDiff = (float)(0.01f+(lTimer-lLastTimer)*0.03f);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
				           D3DCOLOR_XRGB(24,24,24), 1.0f, 0);


	
	m_pMetaballs->Update(0.1f);
	

	m_fModifier = m_fModifier + 0.001f;

	if (m_fModifier == 0.01f) 
	{
		m_fModifier = 0.001f;
	}
	
	m_pBackground->Render();
	m_pMetaballs->Render();
 
	lLastTimer=lTimer;


	return 0;
}

//=== ustawianie aktywnosci - czy dane sa w pamieci

void CMetaballsFX::SetActive(bool bState)
{
	m_bActive = bState;
}

//=== zwraca informacje czy dane sa wczytane

bool CMetaballsFX::bGetActive(void)
{
	return m_bActive;
}
