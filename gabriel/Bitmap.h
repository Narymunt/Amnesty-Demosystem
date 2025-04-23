// Bitmap.h: interface for the CBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAP_H__04D6417F_81DA_4E1A_90EA_CA05C8102BDE__INCLUDED_)
#define AFX_BITMAP_H__04D6417F_81DA_4E1A_90EA_CA05C8102BDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBitmap  
{
public:
	
	unsigned char	*mBitmapDataR;	// dane bitmapy
	unsigned char	*mBitmapDataG;	// dane bitmapy
	unsigned char	*mBitmapDataB;	// dane bitmapy
	
	CBitmap();
	CBitmap(unsigned long size);	// rozmiar bitmapy
	CBitmap(unsigned long size, char filename[]);
	
	virtual ~CBitmap();


};

#endif // !defined(AFX_BITMAP_H__04D6417F_81DA_4E1A_90EA_CA05C8102BDE__INCLUDED_)
