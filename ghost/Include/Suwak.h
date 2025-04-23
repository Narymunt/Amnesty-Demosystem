// Suwak.h: interface for the CSuwak class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GHOSTSuwak_
#define _GHOSTSuwak_

#include <windows.h>
#include <d3d8.h>
#include "FileSystem.h"
#include "Sprite.h"

// suwak, tlo musi byc juz narysowane

class __declspec(dllexport) CSuwak  
{
public:
	
	// tworzy na podstawie systemu pliku
	
	CSuwak(char cFileSystem[], char cImage[], int iX, int iStartY, int iEndY, IDirect3DDevice8	*pDevice);	// przesuwamy w pionie
	
	virtual ~CSuwak();

	virtual int Draw(float mousex, float mousey, bool left, bool center, bool right); // rysuj przycisk

	virtual int iGetValue(void);	// zwraca aktualna pozycje suwaka pomiedzy wspolrzednymi Y
	
	virtual void SetValue(int iValue);	// ustawienie wspolrzednej Y

private:

	// gor¹ce wspó³rzêdne ;) 

	CSprite		*m_pImage;		// nacisniety

	int m_iXPosition;
	int m_iYPosition;	// aktualna wspolrzedna suwaka

	int m_iStartY;
	int m_iEndY;

};

#endif // !defined(AFX_Suwak_H__5E00FEAF_DE67_4436_9881_6636BBF0EC75__INCLUDED_)
