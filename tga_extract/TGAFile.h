// TGAFile.h: interface for the CTGAFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGAFILE_H__B4553226_E0BB_4407_9670_80F8E556B02C__INCLUDED_)
#define AFX_TGAFILE_H__B4553226_E0BB_4407_9670_80F8E556B02C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTGAFile  
{
public:
	CTGAFile(char cFilename[]);
	virtual ~CTGAFile();

	virtual void ExtractAlpha(void);

private:
	
	unsigned char	*m_pHeader;
	unsigned char	*m_pBuffer;

};

#endif // !defined(AFX_TGAFILE_H__B4553226_E0BB_4407_9670_80F8E556B02C__INCLUDED_)
