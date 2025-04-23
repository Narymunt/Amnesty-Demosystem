// LoadScreen.cpp: implementation of the CLoadScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "LoadScreen.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadScreen::CLoadScreen()
{
	m_bActive = false;

	m_pBackground = NULL;
	m_pFadeScreenIn = NULL;
	m_pFadeScreenOut = NULL;
	m_pWhiteScreen = NULL;

}

CLoadScreen::~CLoadScreen()
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

	if (m_pWhiteScreen!=NULL)
	{
		delete m_pWhiteScreen;
		m_pWhiteScreen=NULL;
	}

}



//=== ustawianie aktywnosci - czy dane sa w pamieci

void CLoadScreen::SetActive(bool bState)
{
	m_bActive = bState;
}

//=== zwraca informacje czy dane sa wczytane

bool CLoadScreen::bGetActive(void)
{
	return m_bActive;
}

//=== aktualizuje i wczytuje dane

void CLoadScreen::Initialize(IDirect3DDevice8 *pDevice)
{

	m_pBackground = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground->InitializeEx("data\\load.jpg",pDevice,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	
	m_pFadeScreenIn = new CSprite(255,255,255,255);
	m_pFadeScreenIn->InitializeEx("data\\black.jpg",pDevice,0,0,0,0,D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pFadeScreenOut = new CSprite(255,255,255,255);
	m_pFadeScreenOut->InitializeEx("data\\back002.jpg",pDevice,0,0,0,0,D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	m_pFadeScreenOut->SetModulate(0,255,255,255);

	m_pWhiteScreen= new CSprite(255,255,255,255);
	m_pWhiteScreen->InitializeEx("data\\white.jpg",pDevice,0,0,0,0,D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
	m_pWhiteScreen->SetModulate(0,255,255,255);

	dDa=0.0f;
	dDb=0.0f;

	m_bActive = true;	// dane wczytane
}

//=== deaktualizuje, ale trzyma znaczniki (zwalnia tylko textury)

void CLoadScreen::DeInitialize(void)
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

	if (m_pWhiteScreen!=NULL)
	{
		delete m_pWhiteScreen;
		m_pWhiteScreen=NULL;
	}

}

//=== rysuje pojedyncza klatke

int CLoadScreen::DrawScene(long lTimer)
{

	float fScale;
	
	dDa = (float)lTimer/50;
	dDb = (float)lTimer/90;

	fScale=(float)1+((1+0.7*sin(dDa*0.5 +0.9*dDb))  );	
	m_pBackground->SetTranslation(400-((800*fScale)/2),300-((600*fScale)/2));
	m_pBackground->SetScale(fScale,fScale);
	m_pBackground->SetRotationCenter(400*fScale,300*fScale);
	m_pBackground->SetRotation(dDb);
	m_pBackground->Render();	// rysuj tlo

	lTimer+=100;
	dDa = (float)lTimer/50;
	dDb = (float)lTimer/90;

	fScale=(float)1+((1+0.7*sin(dDa*0.5 +0.9*dDb))  );	
	m_pBackground->SetTranslation(400-((800*fScale)/2),300-((600*fScale)/2));
	m_pBackground->SetScale(fScale,fScale);
	m_pBackground->SetRotationCenter(400*fScale,300*fScale);
	m_pBackground->SetRotation(dDb);
	m_pBackground->SetModulate(192,255,255,255);
	m_pBackground->Render();	// rysuj tlo

	lTimer+=100;
	dDa = (float)lTimer/50;
	dDb = (float)lTimer/90;

	fScale=(float)1+((1+0.7*sin(dDa*0.5 +0.9*dDb))  );	
	m_pBackground->SetTranslation(400-((800*fScale)/2),300-((600*fScale)/2));
	m_pBackground->SetScale(fScale,fScale);
	m_pBackground->SetRotationCenter(400*fScale,300*fScale);
	m_pBackground->SetRotation(dDb);
	m_pBackground->SetModulate(128,255,255,255);
	m_pBackground->Render();	// rysuj tlo

	lTimer+=100;
	dDa = (float)lTimer/50;
	dDb = (float)lTimer/90;

	fScale=(float)1+((1+0.7*sin(dDa*0.5 +0.9*dDb))  );	
	m_pBackground->SetTranslation(400-((800*fScale)/2),300-((600*fScale)/2));
	m_pBackground->SetScale(fScale,fScale);
	m_pBackground->SetRotationCenter(400*fScale,300*fScale);
	m_pBackground->SetRotation(dDb);
	m_pBackground->SetModulate(96,255,255,255);
	m_pBackground->Render();	// rysuj tlo

/*
	if (lTimer>2000)
	{
//		m_pBackground->SetModulate((unsigned char)(2156-lTimer),255,255,255);
		m_pFadeScreenOut->SetModulate((unsigned char)(lTimer-2000),255,255,255);
	}




//	m_pFadeScreenIn->SetModulate((unsigned char)(128+(fScale*64)),255,255,255);


	if (lTimer<255)
	{
		m_pFadeScreenIn->SetModulate((unsigned char)(255-lTimer),255,255,255);
	}

	m_pFadeScreenIn->Render();
	m_pFadeScreenOut->Render();


	if (lTimer>1500 && lTimer<1628)
	{
		m_pWhiteScreen->SetModulate((unsigned char)(1628-lTimer),255,255,255);
		m_pWhiteScreen->Render();
	}
*/

	dDa = dDa+ ((lTimer-lLastTimer)*0.002f);
	dDb = dDb+ ((lTimer-lLastTimer)*0.004f);

	lLastTimer=lTimer;

	return 0;

}