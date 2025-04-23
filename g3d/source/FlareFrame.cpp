// FlareFrame.cpp: implementation of the CFlareFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "FlareFrame.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlareFrame::CFlareFrame(unsigned long ulFlaresCount)
{
	m_piXFlare = new int[1+ulFlaresCount];//(int*) malloc(ulFlaresCount*sizeof(int));
	m_piYFlare = new int[1+ulFlaresCount];//(int*) malloc(ulFlaresCount*sizeof(int));
	m_pfZFlare = new float[1+ulFlaresCount];//(float*) malloc(ulFlaresCount*sizeof(float));
	m_pulIDFlare = new unsigned long[1+ulFlaresCount];//(unsigned long*) malloc(ulFlaresCount*sizeof(unsigned long));

	m_ulFlaresCount=ulFlaresCount;
}

CFlareFrame::~CFlareFrame()
{
	delete[] m_piXFlare; //free (m_piXFlare);
	delete[] m_piYFlare; //free (m_piYFlare);
	delete[] m_pfZFlare; //free (m_pfZFlare);
	delete[] m_pulIDFlare; //free (m_pulIDFlare);
}


void CFlareFrame::AddFlare(int iXFlare, int iYFlare, float fZFlare, unsigned long ulIDFlare, unsigned long ulIndex)
{
	
	assert( ulIndex < m_ulFlaresCount );
	m_piXFlare[ulIndex] = iXFlare;
	m_piYFlare[ulIndex] = iYFlare;
	m_pfZFlare[ulIndex] = fZFlare;
	m_pulIDFlare[ulIndex] = ulIDFlare;
}

unsigned long CFlareFrame::ulGetFlaresCount(void)
{
	return m_ulFlaresCount;
}

int CFlareFrame::iGetFlareX(unsigned long ulIndex)
{
	return m_piXFlare[ulIndex];
}

int CFlareFrame::iGetFlareY(unsigned long ulIndex)
{
	return m_piYFlare[ulIndex];
}

float CFlareFrame::fGetFlareZ(unsigned long ulIndex)
{
	return m_pfZFlare[ulIndex];
}

unsigned long CFlareFrame::ulGetFlareID(unsigned long ulIndex)
{
	return m_pulIDFlare[ulIndex];
}
	
