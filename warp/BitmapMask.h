// BitmapMask.h: interface for the CBitmapMask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPMASK_H__8B804C6F_78E3_4931_BA35_A18F02A5F470__INCLUDED_)
#define AFX_BITMAPMASK_H__8B804C6F_78E3_4931_BA35_A18F02A5F470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBitmapMask  
{
public:

	unsigned char	*mBitmapMaskData;

	CBitmapMask();
	CBitmapMask(unsigned long size);	// od razu alokuj pamiec
	CBitmapMask(unsigned long size, char filename[]);	// wczytaj z pliku


	virtual ~CBitmapMask();

};

#endif // !defined(AFX_BITMAPMASK_H__8B804C6F_78E3_4931_BA35_A18F02A5F470__INCLUDED_)
