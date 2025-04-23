// YTable.h: interface for the CYTable class.
//
// przeliczenia offsetow dla masek i sprites
//////////////////////////////////////////////////////////////////////

#ifndef _GHOSTYTABLE_
#define _GHOSTYTABLE_

//#include <windows.h>

// xsize * y

class CYTable  
{
public:
	
	CYTable();		// ten nie robi nic 
	virtual ~CYTable();

	CYTable(unsigned long ulSize, unsigned long ulResolution);	// ten tworzy tablice

	long	lGetValue(unsigned long ulIndex);

private:

	unsigned long	*p_ulLine;		// tablica z offsetami


};

#endif // !defined(AFX_YTABLE_H__148BB383_BB62_4653_A18E_19AA5F81A7A0__INCLUDED_)
