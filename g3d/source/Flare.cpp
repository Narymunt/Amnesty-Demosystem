// Flare.cpp: implementation of the CFlare class.
//
//////////////////////////////////////////////////////////////////////

#include "Flare.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlare::CFlare(IDirect3DDevice8 *pDevice,char cFilename[])
{
	m_pFlare = new CSprite(255,255,255,255);	// nowy sprite
	m_pFlare->Initialize(cFilename,pDevice);//,0, 0, 0, 0, D3DFMT_UNKNOWN, 
		//D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);
}

CFlare::~CFlare()
{
	if (m_pFlare!=NULL)
	{
		delete m_pFlare;
		m_pFlare= NULL;
	}
}

void CFlare::Render(int iX, int iY, float fZ, long lTimer)
{
	int iOffset;

	float fXSize, fYSize;

	fXSize = m_pFlare->G

	if (iX>0 && iX<800 && iY>0 && iY<600)
	{

	iOffset=(int)(255*(fZ/50)/2);

	m_pFlare->SetTranslation((float)iX-iOffset, (float)iY-iOffset);
	m_pFlare->SetRotationCenter((255*(fZ/50)/2),(255*(fZ/50)/2));
	m_pFlare->SetScale(fZ/50,fZ/50);
//	m_pFlare->SetModulate(fZ,255,255,255);
	m_pFlare->SetRotation(lTimer * 0.1f);
//	m_pFlare->AddRotation(0.1f);
	m_pFlare->Render();
	}

}