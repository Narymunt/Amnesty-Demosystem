// BassPlayer.h: interface for the CBassPlayer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BASSPLAYER_
#define _BASSPLAYER_

#include "Bass.h"

class CBassPlayer  
{

public:
	CBassPlayer(HWND hwnd);
	virtual ~CBassPlayer();

	bool	bIsSoundAvaible () const	
	{
			return m_bSoundAvaible;
	}

	bool	LoadMP3(char *pszFilename);
	bool	PlayMP3();
	void	StopMP3();

private:

	DWORD	m_dwStartTime;
	DWORD	m_dwCurTime;
	DWORD	m_dwPos;
	DWORD	m_dwLevel;

	BOOL	m_bMono;

	HSTREAM	m_hMP3Strem;

	bool	m_bSoundAvaible;

};

#endif 
