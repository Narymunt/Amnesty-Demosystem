// Ytable.h: interface for the CYtable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YTABLE_H__D05B6EB9_318A_4567_B8A5_2C0CC9013579__INCLUDED_)
#define AFX_YTABLE_H__D05B6EB9_318A_4567_B8A5_2C0CC9013579__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYtable  
{

						// do przeliczania wartosci
public:

	unsigned long	*mLine;

	CYtable();
	CYtable(unsigned long size, unsigned long resolution);	// size * resolution

	virtual ~CYtable();

};

#endif // !defined(AFX_YTABLE_H__D05B6EB9_318A_4567_B8A5_2C0CC9013579__INCLUDED_)
