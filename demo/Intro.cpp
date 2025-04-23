// Intro.cpp: implementation of the CIntro class.
//
//////////////////////////////////////////////////////////////////////

#include "Intro.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntro::CIntro()
{
	m_bActive = false;

	m_pBackground = NULL;
	m_pNapis1 = NULL;
	m_pFlare001 = NULL;

	m_pPengo = NULL;

	m_pFile = NULL;

}

CIntro::~CIntro()
{

	if (m_pPengo != NULL)
	{
		delete m_pPengo;
		m_pPengo = NULL;
	}

	if (m_pFlare001 != NULL)
	{
		delete m_pFlare001;
		m_pFlare001 = NULL;
	}

	if (m_pNapis1 != NULL)
	{
		delete m_pNapis1;
		m_pNapis1 = NULL;
	}

	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground = NULL;
	}

	if (m_pFile!=NULL)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

	m_bActive = false;
}

//=== ustawianie aktywnosci - czy dane sa w pamieci

void CIntro::SetActive(bool bState)
{
	m_bActive = bState;
}

//=== zwraca informacje czy dane sa wczytane

bool CIntro::bGetActive(void)
{
	return m_bActive;
}

//=== aktualizuje i wczytuje dane

void CIntro::Initialize(IDirect3DDevice8 *pDevice)
{
	
	m_pFile = new CFileSystem("demo.bin");	// otworz archiwum
	m_pFile->Load("intro.jpg");	// wczytujemy tlo

	m_pBackground = new CSprite(255,255,255,255);	// nowy sprite

	m_pBackground->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
		m_pFile->Search("intro.jpg"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	
	m_pFile->Load("napis1.tga");	// wczytujemy 

	m_pNapis1 = new CSprite(255,255,255,255);	// nowy sprite

	m_pNapis1->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
		m_pFile->Search("napis1.tga"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);


	m_pFile->Load("flara1.tga");	// wczytujemy 

	m_pFlare001 = new CSprite(255,255,255,255);	// nowy sprite

	m_pFlare001->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
		m_pFile->Search("flara1.tga"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pFile->Load("pengo.tga");	// wczytujemy 

	m_pPengo = new CSprite(255,255,255,255);	// nowy sprite

	m_pPengo->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
		m_pFile->Search("pengo.tga"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_bActive = true;	// dane wczytane

	// koniec
	
	delete m_pFile;	// zwolnij system plikow
	m_pFile = NULL;	// zeruj wskaznik

	dDa=0.0f;
	dDb=0.0f;

}

//=== deaktualizuje, ale trzyma znaczniki (zwalnia tylko textury)

void CIntro::DeInitialize(void)
{

	if (m_pPengo != NULL)
	{
		delete m_pPengo;
		m_pPengo = NULL;
	}

	if (m_pFlare001 != NULL)
	{
		delete m_pFlare001;
		m_pFlare001 = NULL;
	}

	if (m_pNapis1!=NULL)
	{
		delete m_pNapis1;
		m_pNapis1 = NULL;
	}
	
	// zwolnij tlo jezeli uzywane
	
	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground=NULL;
	}

	// zwolnij archiwum jezeli uzywane

	if (m_pFile!=NULL)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

}

//=== rysuje pojedyncza klatke

int CIntro::DrawScene(long lTimer)
{
	
	float iScale;
	float fX, fY;

	m_pBackground->Render();	// rysuj tlo

	iScale = (float)(1.5 * (1+0.7*sin(dDa*0.3 +0.7*dDb))  ); 

	m_pNapis1->SetTranslation(10,400);
	m_pNapis1->SetRotationCenter(128,128);
	m_pNapis1->AddRotation(0.01f);
	m_pNapis1->SetScale(iScale,iScale);
	m_pNapis1->Render();

	fX = (float)(400 * (1+0.8*cos(dDa*2 +0.4*dDb))  );
	fY = (float)(300 * (1+0.7*sin(dDa*1.3 +1.7*dDb))  ); 
	
	m_pFlare001->SetTranslation(fX,fY);
	m_pFlare001->SetRotationCenter(128,128);
	m_pFlare001->SetScale(iScale, iScale);
	m_pFlare001->SetRotationCenter((iScale*256)/2,(iScale*256)/2);
	m_pFlare001->AddRotation(iScale);
	m_pFlare001->Render();

	dDa = dDa + 0.01f;
	dDb = dDb + 0.02f;


	if (lTimer>500 && lTimer<840)
	{
	m_pPengo->SetTranslation((800-(512*iScale))/2,(600-(512*iScale))/2);
	m_pPengo->SetRotationCenter(256,256);
	m_pPengo->SetScale(iScale, iScale);
	m_pPengo->SetModulate((unsigned char)(iScale*90),255,255,255);
	m_pPengo->Render();
	}

	
	return 0;

}
//end
