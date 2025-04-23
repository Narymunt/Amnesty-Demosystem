// BassPlayer.cpp: implementation of the CBassPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "BassPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBassPlayer::CBassPlayer(HWND hwnd)
{
        m_dwStartTime = timeGetTime();
        m_dwCurTime = 0;
        m_hMP3Strem = 0;
        m_dwPos = 0;
        m_dwLevel = 0;
        m_bMono = false;

    if ( BASS_GetVersion() != MAKELONG(1,2) )
    {
         m_bSoundAvaible = false;
		 return;
	}


//	if ( !BASS_Init(-1, 44100, 0, hwnd) )
  	if ( !BASS_Init(-1, 44100, BASS_DEVICE_NOSYNC, hwnd) )
    {
          m_bSoundAvaible = false;
		  return;
    }

    m_bSoundAvaible = true;

}

CBassPlayer::~CBassPlayer()
{
    m_hMP3Strem = 0;
	BASS_Free();
}

//=== z tesli

void CALLBACK LoopSync( HSYNC handle, DWORD channel, DWORD data ) 
{

        BASS_Free();
//        PostMessage( g_pMainFrame->GetWindowHandle(), WM_CLOSE, 0, 0 );
}

//=== z tesli

void CALLBACK Sync( HSYNC handle, DWORD channel, DWORD data ) 
{

}

bool CBassPlayer::LoadMP3( char* pszFileName ) {

    if ( !m_bSoundAvaible ) return false;

	if ( m_hMP3Strem = BASS_StreamCreateFile(FALSE, pszFileName, 0, 0, 0) )
	{
	  m_bMono = BASS_ChannelGetFlags( m_hMP3Strem )&BASS_SAMPLE_MONO;
	  BASS_ChannelSetSync( m_hMP3Strem, BASS_SYNC_END, 0, 0,0 );
      BASS_Start();
  	  m_dwStartTime = timeGetTime();
          return true;
	}
        else        
          return false;
}

bool CBassPlayer::PlayMP3() {
      
          if ( !m_hMP3Strem ) return false;

          BASS_StreamPlay( m_hMP3Strem, FALSE, BASS_SAMPLE_LOOP );
          m_dwStartTime = timeGetTime();
          return true;
}

void CBassPlayer::StopMP3() {

        m_hMP3Strem = 0;
	BASS_Free();
}
