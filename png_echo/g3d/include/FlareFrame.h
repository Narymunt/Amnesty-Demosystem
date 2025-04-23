// FlareFrame.h: interface for the CFlareFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FLAREFRAME_
#define _FLAREFRAME_

#include <windows.h>
#include <stdio.h>
#include "flare.h"

// reprezentuje jedna klatke, na ktorej jest kilka flar

class CFlareFrame  
{
public:
	CFlareFrame(unsigned long ulFlaresCount);	// ile flarek w tej klatce
	virtual ~CFlareFrame();

	virtual void AddFlare(int iXFlare, int iYFlare, float fZFlare, unsigned long ulIDFlare, unsigned long ulIndex);
	
	virtual unsigned long ulGetFlaresCount(void);

	virtual int iGetFlareX(unsigned long ulIndex);
	virtual int iGetFlareY(unsigned long ulIndex);
	
	virtual float fGetFlareZ(unsigned long ulIndex);
	
	virtual unsigned long ulGetFlareID(unsigned long ulIndex);
	
protected:

	unsigned long m_ulFlaresCount;

	int		*m_piXFlare;		// [klatka][id_flary]
	int		*m_piYFlare;
	
	float	*m_pfZFlare;

	unsigned long *m_pulIDFlare;


};

#endif 