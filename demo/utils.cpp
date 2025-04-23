// utils.cpp: implementation of the utils class.
//
//////////////////////////////////////////////////////////////////////


#include "utils.h"
	
_tag_TLVERTEX::_tag_TLVERTEX( const G3D_CVector& _p, FLOAT _rhw, DWORD _color, FLOAT _u, FLOAT _v ) : p(_p), rhw(_rhw), color(_color), u(_u), v(_v)
{
}

_tag_TLVERTEX::_tag_TLVERTEX( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _rhw, DWORD _color, FLOAT _u, FLOAT _v ) : rhw(_rhw), color(_color), u(_u), v(_v)
{
	p.x = _x;
	p.y = _y;
	p.z = _z;
}

_tag_TLVERTEX::_tag_TLVERTEX( const _tag_TLVERTEX& v  ) : p(v.p), rhw(v.rhw), color(v.color), u(v.u), v(v.v)
{
}

_tag_SIMPLEVERTEX::_tag_SIMPLEVERTEX( const G3D_CVector& _p, FLOAT _rhw, DWORD _color ) : p(_p), rhw(_rhw), color(_color)
{
}

_tag_SIMPLEVERTEX::_tag_SIMPLEVERTEX( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _rhw, DWORD _color ) : rhw(_rhw), color(_color)
{
	p.x = _x;
	p.y = _y;
	p.z = _z;		
}

_tag_SIMPLEVERTEX::_tag_SIMPLEVERTEX( const _tag_SIMPLEVERTEX& v ) : p(v.p), rhw(v.rhw), color(v.color)
{
}	
	
_tag_VERTEX::_tag_VERTEX( const G3D_CVector& _p, const G3D_CVector& _n, FLOAT _u, FLOAT _v ) : p(_p), n(_n), u(_u), v(_v)
{
}

_tag_VERTEX::_tag_VERTEX( const _tag_VERTEX& v ) : p(v.p), n(v.n), u(v.u), v(v.v)
{
}

HRESULT CreateImageSquare( PDIRECT3DDEVICE8 pDevice, DWORD dwWidth, DWORD dwHeight, DWORD alpha, IMAGESQUARE& sq )
{
	LPWORD				ind;
	PTLVERTEX			verts;

	pDevice->CreateIndexBuffer( 6*sizeof(WORD), D3DUSAGE_SOFTWAREPROCESSING, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &sq.pIB );
	sq.pIB->Lock( 0, NULL, (LPBYTE*)&ind, 0 );
	ind[0] = 0;
	ind[1] = 2;
	ind[2] = 1;
	ind[3] = 0;
	ind[4] = 3;
	ind[5] = 2;
	sq.pIB->Unlock();
	
	pDevice->CreateVertexBuffer( 4*sizeof(TLVERTEX), D3DUSAGE_SOFTWAREPROCESSING, TLVERTEXFVF, D3DPOOL_DEFAULT, &sq.pVB );
	sq.pVB->Lock( 0, NULL, (LPBYTE*)&verts, 0 );	
	verts[0] = TLVERTEX( -1.0f, -1.0f, 0.0f, 0.5f, 0xffffff | (alpha<<24), 0.0f, 0.0f );
	verts[1] = TLVERTEX( -1.0f, (FLOAT)dwHeight, 0.0f, 0.5f, 0xffffff | (alpha<<24), 0.0f, 1.0f );
	verts[2] = TLVERTEX( (FLOAT)dwWidth, (FLOAT)dwHeight, 0.0f, 0.5f, 0xffffff | (alpha<<24), 1.0f, 1.0f );
	verts[3] = TLVERTEX( (FLOAT)dwWidth, -1.0f, 0.0f, 0.5f, 0xffffff | (alpha<<24), 1.0f, 0.0f );
	sq.pVB->Unlock();	

	return S_OK;
}

VOID DeleteImageSquare( IMAGESQUARE& sq )
{
	if( sq.pVB )
		sq.pVB->Release();

	if( sq.pIB )
		sq.pIB->Release();
}

HRESULT RenderImageSquare( PDIRECT3DDEVICE8 pDevice, IMAGESQUARE& sq )
{
	pDevice->SetIndices( sq.pIB, 0 );
	pDevice->SetStreamSource( 0, sq.pVB, sizeof(TLVERTEX) );
	pDevice->SetVertexShader( TLVERTEXFVF );
	pDevice->BeginScene();
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 4, 0, 2 );
	pDevice->EndScene();
	pDevice->SetStreamSource( 0, NULL, 0 );
	pDevice->SetIndices( NULL, 0 );

	return S_OK;
}

VOID EnableStdAlpha( PDIRECT3DDEVICE8 pDevice )
{
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );		
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
}

VOID DisableStdAlpha( PDIRECT3DDEVICE8 pDevice )
{
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
}

VOID Flash( PDIRECT3DDEVICE8 pDevice, IMAGESQUARE& sq, FLOAT factor, DWORD color )
{
	PTLVERTEX				verts;
	DWORD					alpha;
	
	alpha = (DWORD)( ( sinf( factor*H_PI ) + 1.0f )*128.0f );

	sq.pVB->Lock( 0, NULL, (LPBYTE*)&verts, 0 );

	for( DWORD i = 0 ; i < 4 ; i++ )
		verts[i].color = color | (alpha<<24);

	sq.pVB->Unlock();

	pDevice->SetTexture( 0, NULL );
	EnableStdAlpha( pDevice );
	RenderImageSquare( pDevice, sq );
	DisableStdAlpha( pDevice );
}