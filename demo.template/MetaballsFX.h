// MetaballsFX.h: interface for the CMetaballsFX class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _METABALLSFX_H_
#define _METABALLSFX_H_

#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "Sprite.h"
#include "Metaballs.h"
#include "MarchingCubes.h"

class CMetaballsFX  
{
public:
	CMetaballsFX();
	virtual ~CMetaballsFX();

	virtual void SetActive(bool bState);
	virtual bool bGetActive(void);

	virtual void Initialize(IDirect3DDevice8 *pDevice,char cFilename[]);
	virtual void DeInitialize(void);

	virtual int	 DrawScene(IDirect3DDevice8 *pDevice, long lTimer);

private:

	bool			m_bActive;

	CSprite			*m_pBackground;

	CMetaballs		*m_pMetaballs;

	IDirect3DTexture8 *m_pTexture;
	
	IDirect3DVertexBuffer8 *m_pVertexBuffer;
	UINT                    m_nVertexSize;
	UINT                    m_nUsedVertices;
	UINT                    m_nMaxVertices;

	IDirect3DIndexBuffer8  *m_pIndexBuffer;
	UINT                    m_nIndexSize;
	UINT                    m_nUsedIndices;
	UINT                    m_nMaxIndices;

	long					lLastTimer; // ostatni czas rysowania klatki
	float					fDiff;	// roznica od ostatniego rysowania

	float					m_fModifier;	// troche losowoœci

};


#endif 
