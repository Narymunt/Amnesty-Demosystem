
#ifndef _FLARETRACK_
#define _FLARETRACK_

#include <windows.h>
#include <stdio.h>
#include <d3dx8.h>
#include "FlareFrame.h"
#include "Flare.h"

class CFlareTrack  
{
public:
	CFlareTrack(IDirect3DDevice8 *pDevice,FILE *pFile);
	virtual ~CFlareTrack();
	
	virtual void Render(unsigned long ulFrameNo);
	

protected: 

	unsigned long m_ulFramesCount;

	CFlareFrame	  **m_ppFlaresFrames;

	CFlare	*m_pFlare;

};

#endif 