// Mouse.h: interface for the CMouse class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GHOSTMOUSE_
#define _GHOSTMOUSE_

#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "FileSystem.h"
#include "Sprite.h"

//=== klasa obslugujaca myszke

class  __declspec(dllexport) CMouse  
{

public:
	CMouse();
	virtual ~CMouse();

	// renderuje na ekranie

	virtual void Render(void);		// tylko zwykly wskaznik, bez animacji

	// tu renderujemy kursor specjalny

	virtual void Render(int iNumer);	// numer kursora

	// zarzadzanie wygladem kursorow 

	virtual void Initialize(IDirect3DDevice8 *pDevice,char cDataFile[]);
	virtual void DeInitialize(void);

	// dodawanie wlasnych kursorow

	virtual void AddCustomPoint(IDirect3DDevice8 *pDevice, int iIndex, char cName[], char cDataFile[]);

	// ustaw stan przyciskow

	virtual void SetLeftButtonState(bool bState);
	virtual void SetRightButtonState(bool bState);
	virtual void SetCenterButtonState(bool bState);

	virtual bool GetLeftButtonState(void);
	virtual bool GetRightButtonState(void);
	virtual bool GetCenterButtonState(void);

	virtual void SetMouseX(float fPosition);	// ustawia wspolrzedne
	virtual void SetMouseY(float fPosition);	// przekazuje je z petli przechwytywania
												// komunikatow

	virtual float GetMouseX(void);	// zwraca wspolrzedne
	virtual float GetMouseY(void);

	virtual void SetLastMouseX(float fPosition);	// ustaw ostatnie wspolrzedne
	virtual void SetLastMouseY(float fPosition);	// tjw

	virtual float GetLastMouseX(void);		// pobierz ostatnie wspolrzedne
	virtual float GetLastMouseY(void);		// tjw

	virtual void SetAlfaSpot(bool bState);	// czy alfa czy hotspot
	virtual bool GetAlfaSpot(void);

	virtual void SetDeltaX(float fValue);	// ustaw delte
	virtual void SetDeltaY(float fValue);

	virtual float GetDeltaX(void);		// odczytaj delte
	virtual float GetDeltaY(void);		// tjw

	virtual void SetState(char cState);
	virtual char GetState(void);


private:


	bool	m_bLeftButton;		// lewy przycisk myszy
	bool	m_bRightButton;		// prawy przycisk myszy
	bool	m_bCenterButton;	// srodkowy przycisk myszy

	float	m_fDeltaX;			// m_fMouseX-m_fLastMouseX
	float   m_fDeltaY;			// m_fMouseY-m_fLastMouseY


	float	m_fLastMouseX;		// ostatnie wspolrzedne X
	float	m_fLastMouseY;		// ostatnie wspolrzedne Y
	
	float	m_fMouseX;			// wspolrzedne x
	float	m_fMouseY;			// wspolrzedne y

	bool	m_bAlfaSpot;		// 0 - hot point 
								// 1 - jezeli alfa jest na obszarze to zwraca zaliczenie

	char	m_cState;			// 0 - normalny kursor
								// 1 - onEnter
								// 2 - onClick


	char	m_cHotX;		// goracy przycisk
	char	m_cHotY;		// poczatek


	unsigned short	m_usHotSizeX;	// jak duzy jest goracy punkt
	unsigned short  m_usHotSizeY;	// tak samo

	CFileSystem	*m_pMouseFile;	// z tad wczytujemy wyglad myszki

	CSprite		*m_pNormalPoint;		// zwyczajny wskaznik
	CSprite		*m_pOnObjectPoint;	// znajduje sie nad obiektem
	CSprite		*m_pOnClickPoint;		// nacisniecie przycisku

	CSprite		*m_pCustomPoint[16];	// tutaj dodajemy nasze wlasne kursory

};

#endif 
