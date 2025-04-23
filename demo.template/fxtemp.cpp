// FXTemp.cpp: implementation of the CFXTemp class.
//
//////////////////////////////////////////////////////////////////////

#include "FXTemp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFXTemp::CFXTemp()
{
	m_bActive = false;

	m_pBackground = NULL;
	m_pFadeScreenIn = NULL;
	m_pFadeScreenOut = NULL;

}

CFXTemp::~CFXTemp()
{
	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground = NULL;
	}

	if (m_pFadeScreenIn!=NULL)
	{
		delete m_pFadeScreenIn;
		m_pFadeScreenIn = NULL;
	}

	if (m_pFadeScreenOut!=NULL)
	{
		delete m_pFadeScreenOut;
		m_pFadeScreenOut = NULL;
	}

}

//=== ustawianie aktywnosci - czy dane sa w pamieci

void CFXTemp::SetActive(bool bState)
{
	m_bActive = bState;
}

//=== zwraca informacje czy dane sa wczytane

bool CFXTemp::bGetActive(void)
{
	return m_bActive;
}

//=== aktualizuje i wczytuje dane

void CFXTemp::Initialize(IDirect3DDevice8 *pDevice)
{

	m_pBackground = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground->InitializeEx("data\\back.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	
	m_pFadeScreenIn = new CSprite(255,255,255,255);
	m_pFadeScreenIn->InitializeEx("data\\black.jpg",pDevice,0,0,0,0,D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pFadeScreenOut = new CSprite(255,255,255,255);
	m_pFadeScreenOut->InitializeEx("data\\white.jpg",pDevice,0,0,0,0,D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	m_pFadeScreenOut->SetModulate(0,255,255,255);

	m_bActive = true;	// dane wczytane
}

//=== deaktualizuje, ale trzyma znaczniki (zwalnia tylko textury)

void CFXTemp::DeInitialize(void)
{
	
	// zwolnij tlo jezeli uzywane
	
	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground=NULL;
	}

	if (m_pFadeScreenIn!=NULL)
	{
		delete m_pFadeScreenIn;
		m_pFadeScreenIn = NULL;
	}

	if (m_pFadeScreenOut!=NULL)
	{
		delete m_pFadeScreenOut;
		m_pFadeScreenOut = NULL;
	}
}

//=== rysuje pojedyncza klatke

int CFXTemp::DrawScene(long lTimer)
{

	m_pBackground->Render();	// rysuj tlo

	return 0;

}