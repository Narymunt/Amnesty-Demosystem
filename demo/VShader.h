
#ifndef _VSSHADER_
#define _VSSHADER_

#include <windows.h>
#include <d3d8.h>
#include "Sprite.h"

class CVShader
{

public:
	CVShader();
	virtual ~CVShader();

	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice, char cBackgroundFilename[], char cSceneFilename[],
		char cTextureFilename[], char cEnvironmentFilename[], char cShaderFilename[]);

	virtual void DeInitialize(void);

	virtual int	 DrawScene(IDirect3DDevice8 *pDevice, long lTimerStart, long lTimer);

	void SetStartPosition(float fX, float fY, float fZ); // startowe parametry dla obiektu
	void SetModificators(float fX, float fY, float fZ);	// o jakie zmienne co klatke maj¹ siê zmieniac wspolrzedne i katy

private:

	bool						m_bActive;

	CSprite						*m_pBackground;
	CSprite						*m_pRamka;
	
	double						dDa, dDb;
	double						fScale;

	float						m_fStartXPosition;
	float						m_fStartYPosition;
	float						m_fStartZPosition;

	float						m_fModifyX;	// modyfikacje wspolrzednych co klatke
	float						m_fModifyY;
	float						m_fModifyZ;

};

#endif 
