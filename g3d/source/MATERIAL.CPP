#include "material.h"
#include "texture.h"
#include "math3d.h"
#include "scene.h"

void G3D_CMaterial::ApplyMaterial()
{
	D3DMATERIAL8		d3dmat;	
	DWORD				dwWrap = 0;
	G3D_CMatrix		mtx;

	pScene->pd3dDevice->GetRenderState( D3DRS_CULLMODE, &dwPrevCull );
	
	ZeroMemory( &d3dmat, sizeof(D3DMATERIAL8) );

	d3dmat.Diffuse.a = a;
	d3dmat.Diffuse.r = 1.0f;
	d3dmat.Diffuse.g = 1.0f;
	d3dmat.Diffuse.b = 1.0f;

	d3dmat.Ambient.r = 1.0f;
	d3dmat.Ambient.g = 1.0f;
	d3dmat.Ambient.b = 1.0f;	

	if( dwFlags&FLAG_NOTEXT || dwFlags&FLAG_ENVONLY )
	{	
		d3dmat.Diffuse.r = r;
		d3dmat.Diffuse.g = g;
		d3dmat.Diffuse.b = b;				
	}	

	if( dwFlags&FLAG_NOTEXT ) pScene->pd3dDevice->SetMaterial( &d3dmat );		
	if( dwFlags&FLAG_WRAPU ) dwWrap |= D3DWRAP_U;
	if( dwFlags&FLAG_WRAPV ) dwWrap |= D3DWRAP_V; 

	//pScene->pd3dDevice->SetRenderState( D3DRS_WRAP0, dwWrap );
	pScene->pd3dDevice->SetRenderState( D3DRS_WRAP0, FALSE );
	pScene->pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pScene->pd3dDevice->SetMaterial( &d3dmat );
	
	pScene->pd3dDevice->SetTexture( 0, GetTexture( strTextureName ) );

	if( dwFlags&FLAG_ENVONLY )
	{	
		mtx = TranslationMtx( 1.0f, 1.0f, 1.0f )*ScaleMtx( 0.5f );

		pScene->pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL );
		pScene->pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
		pScene->pd3dDevice->SetTransform( D3DTS_TEXTURE0, mtx  );
	}

	if( dwFlags&FLAG_MULTITEXT )	
	{	
		mtx = TranslationMtx( 1.0f, 1.0f, 1.0f )*ScaleMtx( 0.5f );

		pScene->pd3dDevice->SetTexture( 1, GetTexture( strEnvTextName ) );		

		pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR );
		pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
		pScene->pd3dDevice->SetTransform( D3DTS_TEXTURE1, mtx  );		

		pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
		pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );		

		if( dwFlags&FLAG_TEXTUREMODULATE )		
			pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		else if( dwFlags&FLAG_TEXTUREBLEND )
			pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA );
		else if( dwFlags&FLAG_TEXTUREBLENDADD )
			pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR );
		else
			pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_ADD);

		if( dwFlags&FLAG_ENVALPHA )
			pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		else
			pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	}	

	if( dwFlags&FLAG_ALPHABLEND )
	{
		pScene->pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		pScene->pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		pScene->pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

		if( dwAlphaOp == ALPHAOP_ADD )
		{		
			pScene->pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			pScene->pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		}
		else if( dwAlphaOp == ALPHAOP_LINEAR )
		{
			pScene->pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			pScene->pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
		else
		{		
			pScene->pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
			pScene->pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		}		
	}

	if( dwFlags&FLAG_TEXTUREALPHA || dwFlags&FLAG_NOTEXT )
		pScene->pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	else
		pScene->pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
}

void G3D_CMaterial::RestoreMaterial()
{
	pScene->pd3dDevice->SetTexture( 0, NULL );

	if( dwFlags&FLAG_ENVONLY )
	{
		pScene->pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );
		pScene->pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );		
	}

	if( dwFlags&FLAG_MULTITEXT )
	{	
		pScene->pd3dDevice->SetTexture( 1, NULL );		
		pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );
		pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );		
		pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );		
		pScene->pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );		
	}

	pScene->pd3dDevice->SetRenderState( D3DRS_WRAP0, FALSE );
	pScene->pd3dDevice->SetRenderState( D3DRS_CULLMODE, dwPrevCull );
	pScene->pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	pScene->pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	pScene->pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
}

	
G3D_CRenderedMaterial::G3D_CRenderedMaterial( G3D_CScene* scene, DWORD width, DWORD height, PDIRECT3DSURFACE8 zbuffer ) : G3D_CMaterial( scene )
{
	scene->pd3dDevice->CreateTexture( width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &ptexText );

	dwOwnZBuffer = TRUE;

	if( zbuffer )						
	{
		D3DSURFACE_DESC		d3dsdesc;

		zbuffer->GetDesc( &d3dsdesc );

		if( d3dsdesc.Width == width && d3dsdesc.Height == height )
			dwOwnZBuffer = FALSE;			
	}

	if( dwOwnZBuffer )
		scene->pd3dDevice->CreateDepthStencilSurface( width, height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, &psZBuffer );
}

G3D_CRenderedMaterial::~G3D_CRenderedMaterial()
{			
	if( ptexText )
		ptexText->Release();
	
	if( dwOwnZBuffer && psZBuffer )
		psZBuffer->Release();
}

void G3D_CRenderedMaterial::ApplyMaterial()
{
	D3DMATERIAL8		d3dmat;
	ZeroMemory( &d3dmat, sizeof(D3DMATERIAL8) );
	
	d3dmat.Diffuse.r = 1.0f;
	d3dmat.Diffuse.g = 1.0f;
	d3dmat.Diffuse.b = 1.0f;
	
	d3dmat.Ambient.r = 1.0f;
	d3dmat.Ambient.g = 1.0f;
	d3dmat.Ambient.b = 1.0f;
	
	pScene->pd3dDevice->SetMaterial( &d3dmat );
	pScene->pd3dDevice->GetRenderState( D3DRS_AMBIENT, &dwPrevAmbient );
	pScene->pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffff );
	pScene->pd3dDevice->SetTexture( 0, ptexText );
}

void G3D_CRenderedMaterial::RestoreMaterial()
{
	pScene->pd3dDevice->SetRenderState( D3DRS_AMBIENT, dwPrevAmbient );
	pScene->pd3dDevice->SetTexture( 0, NULL );
}

void G3D_CRenderedMaterial::SetRenderTarget()
{
	PDIRECT3DSURFACE8		psTarget;
	
	pScene->pd3dDevice->GetRenderTarget( &psPrevTarget );
	psPrevTarget->Release();
	pScene->pd3dDevice->GetDepthStencilSurface( &psPrevZBuffer );
	psPrevZBuffer->Release();
	
	ptexText->GetSurfaceLevel( 0, &psTarget );
	psTarget->Release();
	
	pScene->pd3dDevice->SetRenderTarget( psTarget, psZBuffer );
}

void G3D_CRenderedMaterial::RestoreRenderTarget()
{
	pScene->pd3dDevice->SetRenderTarget( psPrevTarget, psPrevZBuffer );	
}
