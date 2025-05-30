// Scene.cpp: implementation of the Scene class.
//
//////////////////////////////////////////////////////////////////////
#include <d3d8.h>
#include <d3dx8.h>
#include "Scene.h"

// Create the shader declaration.

extern int error;
extern int screen_x;
extern int screen_y;
extern bool fullscreen;
extern int software_vertexprocessing;

DWORD dwDecl[] =
{
    D3DVSD_STREAM(0),
    D3DVSD_REG(0, D3DVSDT_FLOAT3),      // the position data
    D3DVSD_REG(1, D3DVSDT_FLOAT3),		// normal
    D3DVSD_END()
};

int Scene::sirka,Scene::vyska;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scene::Scene(IDirect3DDevice8 *pd)
{
	pvb_help_rectange = NULL;
	hVertexShader = NULL;
	teapot = NULL;
	torus = NULL;
	c.angle_x=0;
	c.angle_y=0;
	c.pos_x=0;
	c.pos_y=0;
	c.pos_z=5.f;
	c.MouseBod.x = screen_x/2;
	c.MouseBod.y = screen_y/2;
	SetCursorPos(screen_x/2,screen_y/2);

	fps = 200;							// v pripade ze nieco zavisy od fps
	timeframe = 1.f/fps;

	InitDX(pd);
	ShowCursor(0);
	if(!CreateVertexBufferForHelpRectange(80,screen_y_font-45,screen_x_font-80,300,0.4f,pd)){	error = 1; return;}

	sphere_map.load("data/spheremap.JPG");

	if(!sphere.Init(100,200,1.5f)){	error = 1;	return;}
	if(!skybox.Init(5.f)){	error = 1;	return;}
	if(!skybox.LoadTextureSkyBox("data/","jpg")){	error = 1;	return;}


	torus = new C3ds("data/torus.3ds");		if(error)return;
	teapot = new C3ds("data/teapot.3ds");	if(error)return;
	teapot->Rescale( 0.7f);
	if(!torus->CreateDXObjects(pd)){	error = 1;	return;}
	if(!teapot->CreateDXObjects(pd)){	error = 1;	return;}
	if(!LoadVertexShader( "data/vertex_shader.txt", dwDecl, &hVertexShader)){	error = 1;	return;}
}

Scene::~Scene()
{
//	if(hVertexShader!=NULL) pd->DeleteVertexShader( hVertexShader);
	if(teapot!=NULL) delete teapot;
	if(torus!=NULL) delete torus;
	if(pvb_help_rectange!=NULL) pvb_help_rectange->Release();
	ShowCursor(1);
}

void Scene::DrawDXScene(IDirect3DDevice8 *pd)
{
	mat4 m;
    if( NULL == pd )return;

    // Clear the backbuffer to a blue color
	pd->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

    // Begin the scene
    pd->BeginScene();

	ChangeCameraPosition();
	m.Scale(1,1,-1);
	m.Translate( -c.pos_x, -c.pos_y, -c.pos_z);
	m.RotX( c.angle_x);
	m.RotY( c.angle_y);
	pd->SetTransform( D3DTS_WORLD, (D3DMATRIX*)m.m );
	
	pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	static int use_vertex_shader = 1;
	static int model=3;



	if(use_vertex_shader)
	{
		vec4 campos;		// camera position
		mat4 proj,mod;

		pd->GetTransform( D3DTS_PROJECTION, (D3DMATRIX*) proj.m );
		pd->GetTransform( D3DTS_WORLD, (D3DMATRIX*) mod.m );

		campos = (!mod)*vec(0,0,0);		// bod (0,0,0) v suradniciach camery = campos v suradniciach objektu

		m = proj*mod;
		m = ~m;

		vec4 c5(0.5f,-0.5f,0.5f,0.5f);
		vec4 c7(-2,-2,-2,-2);
		vec4 c8(0,0,1,0);
		pd->SetVertexShaderConstant(  0, m.m, 4 );		// c0-c3 contains composite transform matrix
		pd->SetVertexShaderConstant(  5, c5.v, 1 );		// c5   0.5,-0.5,0.5,0.5
		pd->SetVertexShaderConstant(  6, campos.v, 1 );	// c6   pozicia kamery v suradniciach objektu
		pd->SetVertexShaderConstant(  7, c7.v, 1 );		// c7   -2,-2,-2,-2
		pd->SetVertexShaderConstant(  8, c8.v, 1 );		// c8   0,0,1,0

		sphere_map.SetTexture(0);
		pd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

		switch(model)
		{
		case 1: sphere.Render(hVertexShader); break;
		case 2: torus->Render(2,0,hVertexShader); break;
		default: teapot->Render(2,0,hVertexShader);
		}
	}
	else
	{
		// My must change texture matrix. The matrix has to scale and translate 
		// the texture coordinates to get from camera space (-1, +1) to texture space (0,1).

		m.Identity();
		m.m11 = 0.5f;
		m.m22 = -0.5f;
		m.m14 = 0.5f;
		m.m24 = -0.5f;
		pd->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX*)m.m );
		pd->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

		sphere_map.SetTexture(0);
		pd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
		// zapneme ganerovanie texturovych koordinacii
		pd->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL | 1 );
		
		switch(model)
		{
		case 1: sphere.Render(); break;
		case 2: torus->Render(2,0); break;
		default: teapot->Render(2,0);
		}
		// vypneme ganerovanie texturovych koordinacii
		pd->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
		pd->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	}
	pd->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	skybox.Render();

	// vypis FPS
	timeframe = timer0.GetTime();
	fps = 1.0f / timeframe;
	timer0.Reset();
	fps_counter.GetAvgFps(timeframe,0.2f);
	gcvt( (double)fps_counter.avg_fps, 4, text);
		
	// End the scene
	pd->EndScene();

	// Present the backbuffer contents to the display
	pd->Present( NULL, NULL, NULL, NULL );
}

void Scene::ReSizeDXScene(int width, int height,IDirect3DDevice8 *pd)
{
	sirka = width;
	vyska = height;

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, FIELD_OF_VIEW*PI180, (float)sirka/(float)vyska, Z_NEAR_VIEW_PLANE, Z_FAR_VIEW_PLANE );
//	D3DXMatrixOrthoOffCenterLH( &matProj, 0, 800, 0, 600, -1, 1 );
	pd->SetTransform( D3DTS_PROJECTION, &matProj );
}

void Scene::InitDX(IDirect3DDevice8 *pd)
{
	if(!fullscreen)
	{
		RECT		WindowRect;					// miesto pre velkost okna
		WindowRect.left=(long)0;
		WindowRect.top=(long)0;
		GetClientRect(hWnd,&WindowRect);				//zistenie rozmerov uzivatelskej casti okna
		ReSizeDXScene(WindowRect.right,WindowRect.bottom, pd);	//nastavenie perspektivy
	}
	else ReSizeDXScene(screen_x, screen_y, pd);	//definovana v tomto subore

	pd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );
	
	// Enable z-buffering.
	pd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pd->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	mat4 m;	
	pd->SetTransform( D3DTS_VIEW, (struct _D3DMATRIX *)(void*)&m );
	pd->SetTransform( D3DTS_WORLD, (struct _D3DMATRIX *)(void*)&m );

	D3DMATERIAL8 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	pd->SetMaterial( &mtrl );

	ZeroMemory( &light0, sizeof(D3DLIGHT8) );
	light0.Type = D3DLIGHT_DIRECTIONAL;
//	light0.Type = D3DLIGHT_POINT;
//	light0.Diffuse.r = 1.0f;	light0.Diffuse.g = 1.0f;	light0.Diffuse.b = 1.0f;
	light0.Diffuse.r = 0.7f;	light0.Diffuse.g = 0.7f;	light0.Diffuse.b = 0.7f;

	light0.Diffuse.a = 1.0f;
	light0.Ambient.r = 0.3f;
	light0.Ambient.g = 0.3f;
	light0.Ambient.b = 0.3f;
	light0.Ambient.a = 1.0f;

	light0.Direction.x = 0;	light0.Direction.y = 0;	light0.Direction.z = 1;
	
	light0.Position.x = 0;
	light0.Position.y = 0;
	light0.Position.z = -10;
	light0.Attenuation0 = 1.f;

	light0.Range = 1000;

	pd->SetLight( 0, &light0 );
//	pd->LightEnable( 0, TRUE);
//	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.3f,0.3f,0.3f,1) );
	pd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1) );

	pd->SetTextureStageState( 0, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState( 0, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState( 0, D3DTSS_MIPFILTER,D3DTEXF_LINEAR);

// caps

	pd->SetTextureStageState( 1, D3DTSS_MINFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState( 1, D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	pd->SetTextureStageState( 1, D3DTSS_MIPFILTER,D3DTEXF_LINEAR);

//	pd->SetTextureStageState(0, D3DTSS_MAXANISOTROPY,16);
//	pd->SetTextureStageState(1, D3DTSS_MAXANISOTROPY,16);
//	pd->SetTextureStageState(2, D3DTSS_MAXANISOTROPY,16);
	pd->SetRenderState( D3DRS_LIGHTING, 0 );
}

void Scene::ChangeCameraPosition()
{
	
	step *= 0.2f;

		c.pos_x += step * (float)sin(c.angle_y*PI180) * (float)cos(c.angle_x*PI180);
		c.pos_y -= step * (float)sin(c.angle_x*PI180);
		c.pos_z -= step * (float)cos(c.angle_y*PI180) * (float)cos(c.angle_x*PI180);

		c.pos_x -= step * (float)sin(c.angle_y*PI180) * (float)cos(c.angle_x*PI180);
		c.pos_y += step * (float)sin(c.angle_x*PI180);
		c.pos_z += step * (float)cos(c.angle_y*PI180) * (float)cos(c.angle_x*PI180);

		if(c.pos_z<0.f)c.pos_z=0.f;

	if(c.angle_x>90.f) c.angle_x = 90.f;
	if(c.angle_x<-90.f) c.angle_x = -90.f;
}

void Scene::SaveScreen_BMP(IDirect3DDevice8 *pd)
{
	int scr_size_x; int scr_size_y;
	IDirect3DSurface8 *sur;
	D3DDISPLAYMODE Mode;
	
	pd->GetDisplayMode( &Mode);
	Mode.Format = D3DFMT_A8R8G8B8;
	if(pd->CreateImageSurface(Mode.Width, Mode.Height, Mode.Format, &sur)!=D3D_OK )return;
	
	D3DSURFACE_DESC surd;
	if(pd->GetFrontBuffer( sur)!=D3D_OK){sur->Release(); return;}
	sur->GetDesc( &surd);

	D3DLOCKED_RECT LockedRect;
	if(sur->LockRect( &LockedRect, NULL, D3DLOCK_READONLY)!=D3D_OK)
	{
		sur->UnlockRect();
		sur->Release();
		return;
	}

	scr_size_x = surd.Width;
	scr_size_y = surd.Height;

	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	FILE *sub;
	int line_size = 3*scr_size_x;
	if(line_size%4)line_size += 4 - line_size%4;

	unsigned char* buf = new unsigned char[3*line_size*scr_size_y];
	char file[30];

	if(buf==NULL )
	{
		sur->UnlockRect();
		sur->Release();
		return;
	}

	int Pitch = LockedRect.Pitch;
	unsigned char* p = (unsigned char*) LockedRect.pBits;
	for(unsigned int y=0; y<surd.Height; y++)
	{
		for(unsigned int x=0; x<surd.Width; x++)
		{
			buf[line_size*y+3*x+0] = p[Pitch*(surd.Height-y-1)+4*x+0];
			buf[line_size*y+3*x+1] = p[Pitch*(surd.Height-y-1)+4*x+1];
			buf[line_size*y+3*x+2] = p[Pitch*(surd.Height-y-1)+4*x+2];
		}
	}
	sur->UnlockRect();
	sur->Release();

	for(int c=0;c<999;c++)
	{
		char pom[20];
		strcpy(file,"screen");
		if(c<100)strcat(file,"0");
		if(c<10)strcat(file,"0");
		strcat(file,itoa(c,pom,10));
		strcat(file,".bmp");
		sub = fopen(file,"r");
		if(sub==NULL)break;
		if(sub!=NULL)fclose(sub);
	}
	
	fileheader.bfType = 0x4D42; // Magic identifier   - "BM"	| identifikacia BMP suboru musi byt "BM"
	fileheader.bfSize = 3*scr_size_x*scr_size_y+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	// File size in bytes			| velkos suboru v byte
	fileheader.bfReserved1 = 0;
	fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	// Offset to image data, bytes	| posun na zaciatok dat
	
	infoheader.biSize = sizeof(BITMAPINFOHEADER);	// Header size in bytes			| velkost hlavicky BITMAPINFOHEADER
	infoheader.biWidth = scr_size_x;	// Width of image			| sirka obrazka - sizeX
	infoheader.biHeight = scr_size_y;	// Height of image			| vyska obrazka - sizeY
	infoheader.biPlanes = 1;		// Number of colour planes	| pocet farebnych rovin musi bit 1
	infoheader.biBitCount = 24;		// Bits per pixel			| bitov na pixel moze bit 1,4,8,24
	infoheader.biCompression = 0;	// Compression type			| typ compresie , 0 - bez kompresie
	infoheader.biSizeImage = line_size*infoheader.biHeight ;	// Image size in bytes		| velkost obrazka v byte
	infoheader.biXPelsPerMeter = 0;	// Pixels per meter X		| pixelov na meter v smere x
	infoheader.biYPelsPerMeter = 0;	// Pixels per meter Y		| pixelov na meter v smere y
	infoheader.biClrUsed = 0;		// Number of colours		| pocet  farieb v palete, ak 0 vsetky su pouzivane
	infoheader.biClrImportant = 0;	// Important colours		| dolezite farby v palete, ak 0 vsetky su dolezite
	
	sub = fopen(file,"wb");
	fwrite( &fileheader, sizeof(BITMAPFILEHEADER), 1, sub);
	fwrite( &infoheader, sizeof(BITMAPINFOHEADER), 1, sub);
	fwrite( buf, line_size*scr_size_y, 1, sub);

	delete[] buf;
	fclose(sub);
}

int Scene::LoadVertexShader(char *filename, DWORD *pDeclaration, DWORD* phVertexShader, char* verprog, IDirect3DDevice8 *pd)
{
	// Create the vertex shader.
	LPD3DXBUFFER pCode;                             // assembled shader code
	LPD3DXBUFFER pError;
	if(filename==NULL)
		D3DXAssembleShader( verprog, strlen(verprog), 0, NULL, &pCode, &pError );  // assemble shader code
	else
	{
		if( D3DXAssembleShaderFromFile( filename, 0, NULL, &pCode, &pError )  // assemble shader code
			!= D3D_OK)
			MessageBox(hWnd, filename, "Nenajdeny subor pre vertex program", MB_OK);
	}

	if(pError!=NULL)
	{
		char* error_text = new char[pError->GetBufferSize()+1];
		memcpy(error_text,pError->GetBufferPointer(),pError->GetBufferSize());
		error_text[pError->GetBufferSize()]=NULL;

		pError->Release();
		MessageBox(hWnd, error_text, "Chyba pri nacitavani vertex programu", MB_OK);
		delete [] error_text;
		phVertexShader = NULL;
		return 0;
	}

	if(	pd->CreateVertexShader( pDeclaration, (DWORD*)pCode->GetBufferPointer(), phVertexShader, software_vertexprocessing )
		== D3D_OK )
	{
		pCode->Release();
		return 1;
	}
	pCode->Release();
	MessageBox(hWnd, filename, "Chyba pri nacitavani vertex programu", MB_OK);
	return 0;
}

int Scene::CreateVertexBufferForHelpRectange(float left, float top, float right, float bottom, float alpha, IDirect3DDevice8 *pd)
{
	int size_of_custom_vertex = 3*sizeof(float)+sizeof(DWORD);
	if( FAILED( pd->CreateVertexBuffer( 4*size_of_custom_vertex,0, 
								D3DFVF_XYZ|D3DFVF_DIFFUSE,D3DPOOL_DEFAULT, &pvb_help_rectange ) ) )return 0;
	void* p1;
	if( FAILED( pvb_help_rectange->Lock( 0, 4*size_of_custom_vertex, (BYTE**)&p1, 0 ) ) )return 0;
	((float*)p1)[0*4+0] = left;
	((float*)p1)[0*4+1] = bottom;
	((float*)p1)[0*4+2] = 0;
	((DWORD*)p1)[0*4+3] = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,alpha);
	((float*)p1)[1*4+0] = right;
	((float*)p1)[1*4+1] = bottom;
	((float*)p1)[1*4+2] = 0;
	((DWORD*)p1)[1*4+3] = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,alpha);
	((float*)p1)[2*4+0] = left;
	((float*)p1)[2*4+1] = top;
	((float*)p1)[2*4+2] = 0;
	((DWORD*)p1)[2*4+3] = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,alpha);
	((float*)p1)[3*4+0] = right;
	((float*)p1)[3*4+1] = top;
	((float*)p1)[3*4+2] = 0;
	((DWORD*)p1)[3*4+3] = D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,alpha);
	pvb_help_rectange->Unlock();
	return 1;
} 
