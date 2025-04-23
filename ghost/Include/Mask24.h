// Mask24.h: interface for the CMask24 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GHOSTMASK24_
#define _GHOSTMASK24_

#include <windows.h>
#include "YTable.h"

class __declspec(dllexport) CMask24  
{
public:
	CMask24(long lXSize, long lYSize);
	virtual ~CMask24();

private:

	unsigned char *m_pBuffer;		// pomnozyc x3 
	
	CYTable		  *m_pYTable;

};

#endif 
