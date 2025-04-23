#ifndef _TEXTURERENDERTARGET_H
#define _TEXTURERENDERTARGET_H

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>

class textureRenderTarget  
{
public:


	void End();
	void Begin(IDirect3DDevice8 *pd);
	int Init(HWND hWnd,IDirect3DDevice8 *pd,int size_x, int size_y, int zbuf=0, int stencil=0);
	void SetTexture(int Stage,IDirect3DDevice8 *pd);

	LPDIRECT3DTEXTURE8 pt;
	LPD3DXRENDERTOSURFACE   pRenderToSurface;
	LPDIRECT3DSURFACE8      pSurface;

	textureRenderTarget();
	virtual ~textureRenderTarget();
};

#endif // _TEXTURERENDERTARGET_H
