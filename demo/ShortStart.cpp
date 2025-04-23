// ShortStart.cpp: implementation of the CShortStart class.
//
//////////////////////////////////////////////////////////////////////

#include "ShortStart.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShortStart::CShortStart()
{
	m_bActive = false;

	m_pBackground01 = NULL;
	m_pBackground02 = NULL;

	m_pFile = NULL;

}

CShortStart::~CShortStart()
{

	if (m_pBackground01!=NULL)
	{
		delete m_pBackground01;
		m_pBackground01 = NULL;
	}

	if (m_pBackground02!=NULL)
	{
		delete m_pBackground02;
		m_pBackground02 = NULL;
	}

	if (m_pBackground03!=NULL)
	{
		delete m_pBackground03;
		m_pBackground03 = NULL;
	}

	if (m_pBackground04!=NULL)
	{
		delete m_pBackground04;
		m_pBackground04 = NULL;
	}

	if (m_pFile!=NULL)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

	m_bActive = false;
}

//=== ustawianie aktywnosci - czy dane sa w pamieci

void CShortStart::SetActive(bool bState)
{
	m_bActive = bState;
}

//=== zwraca informacje czy dane sa wczytane

bool CShortStart::bGetActive(void)
{
	return m_bActive;
}

//=== aktualizuje i wczytuje dane

void CShortStart::Initialize(IDirect3DDevice8 *pDevice)
{
	
	m_pFile = new CFileSystem("demo.bin");	// otworz archiwum

	m_pFile->Load("pic0.jpg");	// wczytujemy tlo
	m_pBackground01 = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground01->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
		m_pFile->Search("pic0.jpg"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pFile->Load("pic1.jpg");	// wczytujemy tlo
	m_pBackground02 = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground02->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
		m_pFile->Search("pic1.jpg"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pFile->Load("pic2.jpg");	// wczytujemy tlo
	m_pBackground03 = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground03->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
		m_pFile->Search("pic2.jpg"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pFile->Load("pic3.jpg");	// wczytujemy tlo
	m_pBackground04 = new CSprite(255,255,255,255);	// nowy sprite
	m_pBackground04->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
		m_pFile->Search("pic3.jpg"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);


	m_bActive = true;	// dane wczytane

	// koniec
	
	delete m_pFile;	// zwolnij system plikow
	m_pFile = NULL;	// zeruj wskaznik


}

//=== deaktualizuje, ale trzyma znaczniki (zwalnia tylko textury)

void CShortStart::DeInitialize(void)
{

	
	// zwolnij tlo jezeli uzywane
	
	if (m_pBackground01!=NULL)
	{
		delete m_pBackground01;
		m_pBackground01=NULL;
	}

	if (m_pBackground02!=NULL)
	{
		delete m_pBackground02;
		m_pBackground02=NULL;
	}

	if (m_pBackground03!=NULL)
	{
		delete m_pBackground03;
		m_pBackground03 = NULL;
	}

	if (m_pBackground04!=NULL)
	{
		delete m_pBackground04;
		m_pBackground04 = NULL;
	}

	// zwolnij archiwum jezeli uzywane

	if (m_pFile!=NULL)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

}

//=== rysuje pojedyncza klatke

int CShortStart::DrawScene(long lTimer)
{

	if (lTimer<100)
	{
		m_pBackground01->Render();	// rysuj tlo
	}
	else if (lTimer>100 && lTimer<220)
	{
		m_pBackground02->Render();
	}
	else if (lTimer>220 && lTimer<350)
	{
		m_pBackground03->Render();
	}
	else if (lTimer>350)
	{
		m_pBackground04->Render();
	}
	
	return 0;

}
//end
