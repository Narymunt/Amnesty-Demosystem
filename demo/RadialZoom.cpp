// LoadScreen.cpp: implementation of the CRadialZoom class.
//
//////////////////////////////////////////////////////////////////////

#include "RadialZoom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRadialZoom::CRadialZoom()
{
	m_bActive = false;

	m_pBackground = NULL;
	m_pFadeScreenIn = NULL;
	m_pFadeScreenOut = NULL;
}

CRadialZoom::~CRadialZoom()
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

void CRadialZoom::SetActive(bool bState)
{
	m_bActive = bState;
}

//=== zwraca informacje czy dane sa wczytane

bool CRadialZoom::bGetActive(void)
{
	return m_bActive;
}

//=== aktualizuje i wczytuje dane

void CRadialZoom::Initialize(IDirect3DDevice8 *pDevice)
{

	m_pBackground = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground->InitializeEx("data\\zoom.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	
	m_pFadeScreenIn = new CSprite(255,255,255,255);
	m_pFadeScreenIn->InitializeEx("data\\fc1.jpg",pDevice,0,0,0,0,D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pFadeScreenOut = new CSprite(255,255,255,255);
	m_pFadeScreenOut->InitializeEx("data\\metaback.jpg",pDevice,0,0,0,0,D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	m_pFadeScreenOut->SetModulate(0,255,255,255);

	dDa=0.0f;
	dDb=0.0f;


	m_bActive = true;	// dane wczytane
}

//=== deaktualizuje, ale trzyma znaczniki (zwalnia tylko textury)

void CRadialZoom::DeInitialize(void)
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

int CRadialZoom::DrawScene(long lTimer)
{

	float fScale;
	float fDa, fDb; 

	fScale=(float)1+((1+0.7*sin(dDa*0.4 +0.9*dDb))  );

//	m_pBackground->SetTranslation(0,0);
//	m_pBackground->SetScale(1.0f,1.0f);
//	m_pBackground->SetModulate(255,255,255,255);
//	m_pFadeScreenIn->Render();

	fDa=(float)lTimer/200; fDb=(float)lTimer/300;

	for (int iTemp=0; iTemp<32; iTemp++)
	{

	fScale=(float)0.75f+((1+0.7*sin(fDa*0.3 +0.7*fDb))  );

//	fScale=(float)(lTimer*5)*iTemp/2000;

	m_pBackground->SetTranslation((1024-(1024*fScale))/2,(768-(768*fScale))/2);
	m_pBackground->SetScale(fScale,fScale);
	m_pBackground->SetRotationCenter(512*fScale,384*fScale);
//	m_pBackground->SetRotation(fScale);
	m_pBackground->SetModulate((unsigned char)iTemp*4,255,255,255);
	m_pBackground->Render();	// rysuj tlo

	fDa = fDa + 0.03f;
	fDb = fDb + 0.05f;

	}

	dDa = dDa+ ((lTimer-lLastTimer)*0.002f);
	dDb = dDb+ ((lTimer-lLastTimer)*0.004f);

	if (lTimer>1375)
	{
		m_pFadeScreenOut->SetModulate((unsigned char)(lTimer-1375),255,255,255);
		m_pFadeScreenOut->Render();
	}


	lLastTimer=lTimer;

	return 0;

}