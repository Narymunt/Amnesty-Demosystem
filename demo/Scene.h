// Scene.h: interface for the Scene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_)
#define AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3d8.h>
#include <d3dx8.h>
#include <d3d8caps.h>
#include "init.h"
#include "Matrix.h"
#include "timer.h"
#include "texture.h"
#include "3ds.h"
#include "SphereVN.h"
#include "SkyBox.h"
#include "Util.h"

class Scene  
{
public:
	void SaveScreen_BMP(IDirect3DDevice8 *pd);
	void ChangeCameraPosition();
	void InitDX(IDirect3DDevice8 *pd);
	void ReSizeDXScene(int width, int height, IDirect3DDevice8 *pd);
	void DrawDXScene(IDirect3DDevice8 *pd);
	int LoadVertexShader( char* filename, DWORD* pDeclaration, DWORD* phVertexShader, char* verprog=NULL, IDirect3DDevice8 *pd=NULL);
	int CreateVertexBufferForHelpRectange( float left, float top, float right, float bottom, float alpha,IDirect3DDevice8 *pd); 
	Scene(IDirect3DDevice8 *pd);
	virtual ~Scene();

	C3ds *teapot,*torus;
	SphereVN sphere;
	SkyBox skybox;
	DWORD hVertexShader;

	D3DLIGHT8 light0;
	Timer	timer0;
	texture sphere_map;

	struct camera
	{
		float angle_x,angle_y;
		float pos_x,pos_y,pos_z;
		POINT MouseBod;
	}c;

	LPDIRECT3DVERTEXBUFFER8 pvb_help_rectange;	// Buffers to hold vertices
	float	fps,timeframe;
	uFPS_counter fps_counter;
	char	text[100];					// text pre konverziu z float na text
	static int sirka,vyska;

	float	step;
};

#endif // !defined(AFX_SCENE_H__3C4142F5_12FF_11D8_8A6F_F03137CE7A04__INCLUDED_)
