
#include "math3d.h"
#include <d3dx8.h>

#pragma pack(4)
typedef struct _tag_TLVERTEX
{
	G3D_CVector	p;
	FLOAT			rhw;
	DWORD			color;
	FLOAT			u, v;
	
	_tag_TLVERTEX( const G3D_CVector& _p, FLOAT _rhw, DWORD _color, FLOAT _u, FLOAT _v );
	_tag_TLVERTEX( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _rhw, DWORD _color, FLOAT _u, FLOAT _v );
	_tag_TLVERTEX( const _tag_TLVERTEX& v );
	
} TLVERTEX, *PTLVERTEX;

#pragma pack(4)
typedef struct _tag_SIMPLEVERTEX
{
	G3D_CVector	p;
	FLOAT			rhw;
	DWORD			color;
	
	_tag_SIMPLEVERTEX( const G3D_CVector& _p, FLOAT _rhw, DWORD _color );
	_tag_SIMPLEVERTEX( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _rhw, DWORD _color );
	_tag_SIMPLEVERTEX( const _tag_SIMPLEVERTEX& v );
	
} SIMPLEVERTEX, *PSIMPLEVERTEX;

#pragma pack(4)
typedef struct _tag_VERTEX
{
	G3D_CVector	p;
	G3D_CVector	n;	
	FLOAT			u, v;
	
	_tag_VERTEX( const G3D_CVector& _p, const G3D_CVector& _n, FLOAT _u, FLOAT _v );
	_tag_VERTEX( const _tag_VERTEX& v );
	
} VERTEX, *PVERTEX;

#define TLVERTEXFVF		D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1
#define SIMPLEVERTEXFVF	D3DFVF_XYZRHW | D3DFVF_DIFFUSE
#define VERTEXFVF		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1

typedef struct _tag_IMAGESQUARE
{
	PDIRECT3DINDEXBUFFER8	pIB;
	PDIRECT3DVERTEXBUFFER8	pVB;
	
} IMAGESQUARE, *PIMAGESQUARE;

HRESULT CreateImageSquare( PDIRECT3DDEVICE8 pDevice, DWORD dwWidth, DWORD dwHeight, DWORD alpha, IMAGESQUARE& sq );
VOID DeleteImageSquare( IMAGESQUARE& sq );
HRESULT RenderImageSquare( PDIRECT3DDEVICE8 pDevice, IMAGESQUARE& sq );
VOID EnableStdAlpha( PDIRECT3DDEVICE8 pDevice );
VOID DisableStdAlpha( PDIRECT3DDEVICE8 pDevice );
VOID Flash( PDIRECT3DDEVICE8 pDevice, IMAGESQUARE& sq, FLOAT factor, DWORD color = 0xffffff );

/*class CSpriteSquare
{
public:
	
	PDIRECT3DINDEXBUFFER8	pIB;
	PDIRECT3DVERTEXBUFFER8	pVB;
	PDIRECT3DTEXTURE8		pText;

public:

	CSpriteSquare();
	~CSpriteSquare();

	Initialize();
	Render();
	SetTexture();
	Resize();
	ChangeColor();
};*/

/*
class CRenderTarget
{
public:

	PDIRECT3DTEXTURE8	pTarget;
	PDIRECT3DSURFACE8	pZBuffer;

	PDIRECT3DSURFACE8	pPrevTarget;
	PDIRECT3DSURFACE8	pPrevZBuffer;

public:

	CRenderTarget();
	~CRenderTarget();
	Initialize();
	SetTarget();
	RestoreTarget();
	operator PDIRECT3DTEXTURE8();
};

class CBlurer
{
public:

	PDIRECR3DTEXTURE8	
};
*/

