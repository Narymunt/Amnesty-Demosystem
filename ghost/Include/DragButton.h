// DragButton.h: interface for the CDragButton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GHOSTDragButton_
#define _GHOSTDragButton_

#include <windows.h>
#include <d3d8.h>
#include "FileSystem.h"
#include "Sprite.h"

// roznego rodzaju przyciski
// TODO:
//		- sprawdzane wspolrzedne powinny byc dynamiczne

// przycisk ktory mozna przeciagac po ekranie


class __declspec(dllexport) CDragButton  
{
public:
	
	// tworzy na podstawie systemu pliku
	
	CDragButton(char cFileSystem[], char clicked_name[], 
		char onDragButton_name[], char pimage_name[], char onmove_name[],
		IDirect3DDevice8	*pDevice);
	
	CDragButton(char cFileSystem[], char clicked_name[], 
		char onDragButton_name[], char pimage_name[], char onmove_name[],
		char dropped_name[],IDirect3DDevice8 *pDevice);

	virtual ~CDragButton();

	virtual int Draw(float mousex, float mousey, bool left, bool center, bool right); // rysuj przycisk

	virtual void SetPosition(long lX, long lY);

	// hot coords
	
	virtual void SetHotCoords(long lXStart, long lYStart, long lXEnd, long lYEnd);

	virtual void SetDropArea(long x1, long y1, long x2, long y2);	// gdzie mozna upuszczac
	virtual bool bIsDrop(long x, long y);

	virtual void SetReturnArea(long x1, long y1, long x2, long y2);	// gdzie mozna "zwrocic"
	virtual bool bIsReturn(long x, long y);	// true - w pozycji zwrotu, false - poza nia

	// mouse state 

	virtual void SetState(char cState);	// ustaw stan myszy
	virtual char cGetState(void); // pobierz stan myszy

	// czy obiekt wraca do pola macierzystego

	virtual void SetMovable(bool bMove);
	virtual bool bGetMovable(void);

	// ustawianie wspolrzednych

	virtual void SetDefaultPosition(long lX, long lY);
	virtual void SetCurrentPosition(long lX, long lY);

	// pobieranie wspolrzednych

	virtual long lGetDefaultPositionX(void);
	virtual long lGetDefaultPositionY(void);

	virtual long lGetCurrentPositionX(void);
	virtual long lGetCurrentPositionY(void);

	// wartosci centrujace

	virtual void SetAddPosition(long x, long y);

	virtual long lGetAddXPosition(void);
	virtual long lGetAddYPosition(void);

	// wartosci jednostkowe

	virtual void SetFE(bool bState);
	virtual bool bGetFE(void);

	virtual void SetPress(bool bState);
	virtual bool bGetPress(void);

	// obracanie

	virtual void		SetRotation(float newRotation);	// ustaw rotacje
	virtual void		SetRotationCenter(float x, float y); // ustaw punkt rotacji
	virtual float		fGetRotation(void);				// pobierz rotacje
	virtual void		AddRotation(float addValue);	// dodaj lub odejmij 

	//-=- skalowanie -=-

	virtual void		SetScale(float fValueX, float fValueY);	// ustaw skalê obiektu
	
	virtual void		SetScaleX(float fValueX);	// ustaw skalê
	virtual void		SetScaleY(float fValueY);

	virtual float		fGetScaleX(void);	// pobierz wspó³rzêdne
	virtual float		fGetScaleY(void);		

	virtual void		AddScale(float fValueX, float fValueY);	// dodaj skalê

	virtual void		AddScaleX(float fValueX);
	virtual void		AddScaleY(float fValueY);

	virtual int			iGetClickedSizeX(void);
	virtual int			iGetClickedSizeY(void);
	
	virtual int			iGetOnDragButtonSizeX(void);
	virtual int			iGetOnDragButtonSizeY(void);
	
	virtual int			iGetImageSizeX(void);
	virtual int			iGetImageSizeY(void);
	
	virtual int			iGetOnMoveSizeX(void);
	virtual int			iGetOnMoveSizeY(void);

	virtual int			iGetDroppedSizeX(void);
	virtual int			iGetDroppedSizeY(void);


private:
	
	IDirect3DDevice8	*m_pDevice;
	
	char		m_cFilesystem[256];	// do zapamietania

	char		m_cState;	// 0 lezy na ekranie, mysz poza nim, aktywny
							// 1 lezy na ekranie, mysz nad nim
							// 2 lezy na ekranie, mysz kliknieta na nim
							// 3 podaza za myszka
							// 4 przeniesiony, na nowej pozycji						

	bool		m_bMovable;	// true - przycisk po upuszczeniu ustawiony w dozwolonym obszarze
							// false - przycisk po upuszczeniu wraca do pozycji macierzystych

	// wartosci korygujace centrum obrazka wzgledem kursora myszy, czyli dla 512x512 bedzie to -256,-256 itd.

	long		m_lXAdd;
	long		m_lYAdd;

	// pozycje startowe na ekranie, matczyne

	long		m_lXDefault;
	long		m_lYDefault;

	// pozycje aktualne

	long		m_lXCurrent;
	long		m_lYCurrent;
	
	// gdzie mozna kliknac zeby przycisk oddac 

	long		m_lXReturnStart;
	long		m_lYReturnStart;
	long		m_lXReturnEnd;
	long		m_lYReturnEnd;

	// obszar upuszczania, tutaj przycisk moze byc postawiony

	long		m_lXDropStart;
	long		m_lYDropStart;
	long		m_lXDropEnd;
	long		m_lYDropEnd;

	// gor¹ce wspó³rzêdne, tutaj przycisk reaguje na ruch myszy

	long		m_lXStart;
	long		m_lYStart;
	long		m_lXEnd;
	long		m_lYEnd;

	CSprite		*m_pClicked;		// nacisniety

	int			m_iClickedSizeX;
	int			m_iClickedSizeY;

	CSprite		*m_pOnDragButton;		// mysz nad nim
	
	int			m_iOnDragButtonSizeX;
	int			m_iOnDragButtonSizeY;
	
	CSprite		*m_pImage;		// zwyczajny
	
	int			m_iImageSizeX;
	int			m_iImageSizeY;

	CSprite		*m_pOnMove;		// przesuwany myszka
	
	int			m_iOnMoveSizeX;
	int			m_iOnMoveSizeY;

	char		m_cOnMoveFilename[256];

	CSprite		*m_pDropped;	// upuszczony przycisk moze np. miec wiekszy rozmiar

	int			m_iDroppedSizeX;
	int			m_iDroppedSizeY;

	char		m_cDroppedFilename[256];	// doczytujemy w trakcie dzialania


	bool		m_bFE;			// first enter
	bool		m_bPressed;		// czy nacisniety ?


};

#endif // !defined(AFX_DragButton_H__5E00FEAF_DE67_4436_9881_6636BBF0EC75__INCLUDED_)
