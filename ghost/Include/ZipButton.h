// ZipButton.h: interface for the CZipButton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GHOSTZipButton_
#define _GHOSTZipButton_

#include <windows.h>
#include <d3d8.h>
#include "zip.h"
#include "unzip.h"
#include "FileSystem.h"
#include "ZipSprite.h"

// roznego rodzaju przyciski
// TODO:
//		- sprawdzane wspolrzedne powinny byc dynamiczne


class __declspec(dllexport) CZipButton  
{
public:
	
	// tworzy na podstawie systemu pliku
	
	CZipButton(char cFileSystem[], char cPassword[],
		char clicked_name[], 
		char onZipButton_name[], char pimage_name[],
		IDirect3DDevice8	*pDevice);
	
	virtual ~CZipButton();

	virtual int Draw(float mousex, float mousey, bool left, bool center, bool right); // rysuj przycisk

	virtual void SetPosition(long lX, long lY);

	// hot coords
	
	virtual void SetHotCoords(long lXStart, long lYStart, long lXEnd, long lYEnd);

private:

	// gor¹ce wspó³rzêdne ;) 

	long		m_lXStart;
	long		m_lYStart;
	long		m_lXEnd;
	long		m_lYEnd;

	CZipSprite		*m_pClicked;		// nacisniety
	CZipSprite		*m_pOnButton;		// mysz nad nim
	CZipSprite		*m_pImage;		// zwyczajny

	bool		m_bFE;			// first enter
	bool		m_bPressed;		// czy nacisniety ?

};

#endif // !defined(AFX_ZipButton_H__5E00FEAF_DE67_4436_9881_6636BBF0EC75__INCLUDED_)
