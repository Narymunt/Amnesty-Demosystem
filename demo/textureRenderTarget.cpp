#include "textureRenderTarget.h"

textureRenderTarget::textureRenderTarget()
{
	pt = NULL;
	pRenderToSurface = NULL;
	pSurface = NULL;
}

textureRenderTarget::~textureRenderTarget()
{
	if(pRenderToSurface!=NULL)pRenderToSurface->Release();
	if(pSurface!=NULL)pSurface->Release();
	if(pt!=NULL)pt->Release();
}

int textureRenderTarget::Init(HWND hWnd,IDirect3DDevice8 *pd,int size_x, int size_y, int zbuf, int stencil)
{
	int ok=0;

	if( pd->CreateTexture( size_x, size_y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R8G8B8, D3DPOOL_DEFAULT, &pt )
		== D3D_OK )ok=1;
	if(!ok)
	{
		if( pd->CreateTexture( size_x, size_y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pt )
			== D3D_OK )ok=1;
	}
	
	if(!ok)
	{

		pt = NULL; 
		MessageBox( hWnd, "Error: Couldn't creat texture with using as RENDER TARGET", "ERROR", MB_OK );
		return 0;
	}

    D3DSURFACE_DESC desc;
    pt->GetSurfaceLevel( 0, &pSurface );
    pSurface->GetDesc( &desc );

	D3DFORMAT DepthStencilFormat;
	
	if(stencil)
		DepthStencilFormat = D3DFMT_D24S8;
	else
	{
		switch(zbuf)
		{
		case  0: DepthStencilFormat = D3DFMT_UNKNOWN; break;
		case 16: DepthStencilFormat = D3DFMT_D16; break;
		case 24: DepthStencilFormat = D3DFMT_D24X8; break;
		case 32: DepthStencilFormat = D3DFMT_D32; break;
		default: DepthStencilFormat = D3DFMT_D16;
		}
	}

	if (FAILED(D3DXCreateRenderToSurface( pd, desc.Width, desc.Height, desc.Format, 
											zbuf!=0, DepthStencilFormat, &pRenderToSurface )))
	{
		MessageBox( hWnd, "Error: Couldn't creat Render To Surface", "ERROR", MB_OK );
		return 0;
	}

	return 1;
}

void textureRenderTarget::Begin(IDirect3DDevice8 *pd)
{
	pRenderToSurface->BeginScene( pSurface, NULL );

	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
}

void textureRenderTarget::End()
{
	pRenderToSurface->EndScene();
}

void textureRenderTarget::SetTexture(int Stage,IDirect3DDevice8 *pd)
{
	Stage=0;
	pd->SetTexture( Stage, pt );
}
