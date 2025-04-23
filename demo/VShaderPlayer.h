//========================================================================================================================
// dla obiektow z vertex shaderem
//========================================================================================================================
// <V_SHADER>
// data/background.jpg
// data/obiekt.x
// data/textura.tga
// data/env.jpg
// data/shader.vsh
// [czas_start] [czas_end] [czas_load]
// startX startY startZ
// modifyX modifyY modifyZ
// domyslnie maksymalna liczba obiektow na raz to 128

#if !defined VSHADER_PLAYER
#define VSHADER_PLAYER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <stdio.h>
#include "VShader.h"

class CVShaderPlayer
{
public:

	// inicjalizacja obiektu

	CVShaderPlayer();	// ile elementow bedzie wczytywanych 
	virtual ~CVShaderPlayer();

	virtual void Parse(char cFilename[]);	// wczytuje wszystkie potrzebne dane na podstawie skryptu
	virtual void Play(long lTimer,IDirect3DDevice8 *pDevice);			// odtwarzaj obiekt wg timera

private:

	CVShader			*m_pVScene[128];	// alokujemy potrzebne wskazniki

	int					m_iStartTime[128];	// kiedy zaczynamy odtwarzanie
	int					m_iEndTime[128];		// kiedy konczymy odtwarzanie
	int					m_iLoadTime[128];		// kiedy dane maja byc wczytane

	char				m_cBackgroundFilename[128][128];	// nazwa pliku z tlem
	char				m_cSceneFilename[128][128];	// tutaj nazwy plikow ze scenkami
	char				m_cTextureFilename[128][128];	// nazwa pliku textury .tga lub .png
	char				m_cEnvironmentFilename[128][128];	// nazwa pliku environment mapping .jpg lub .png
	char				m_cShaderFilename[128][128];	// nazwa pliku z procedura shadera

	int					m_iSceneCount;		// ile scenek jest juz wczytanych

	long				m_lStartX;	// startowe wspolrzedne dla obiektu
	long				m_lStartY;
	long				m_lStartZ;	

	long				m_lModifyX;	// modyfikatory dla obiektu
	long				m_lModifyY;
	long				m_lModifyZ;

};

#endif // !defined(AFX_EVENT_H__5058FACD_CA6F_42E0_99E6_FD5BBDAEF7C9__INCLUDED_)
