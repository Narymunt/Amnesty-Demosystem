#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <ctype.h>

#include "init.h"
#include "Image.h"

class texture  
{
public:
	int load( char* filename, int repeat=0, int filter=3, int compression=0,IDirect3DDevice8 *pd=NULL );
	int load( Image &im, int repeat=0, int filter=3, int compression=0,IDirect3DDevice8 *pd=NULL );
	void SetTexture(int Stage=0, IDirect3DDevice8 *pd=NULL){pd->SetTexture( Stage, pt );}

	LPDIRECT3DTEXTURE8 pt;

	texture();
	virtual ~texture();
};

#endif // _TEXTURE_H