#ifndef __SCENE_H_
#define __SCENE_H_

#include <d3d8.h>
#include "flaretrack.h"


enum G3D_SCENEFLAGS
{
	FLAG_RENDERFLARE		= 1,
	FLAG_LENSFALER			= 2,
	FLAG_MORPH				= 4,
	FLAG_SHADOW				= 8,
	FLAG_HW					= 16,
	FLAG_THROWTEXEXC		= 32,
	FLAG_SCENE_DWORD		= 0x7fffffff
};

enum G3D_ERRORCODES
{
	G3DERR_UNABLETOLOAD,
	G3DERR_UNABLETOINITIALIZE,
	G3DERR_NOCAMERA,
	G3DERR_TEXTUREEXCEPTION,
	G3DERR_NOTINITIALIZED,
	G3DERR_GENERIC
};

#define	MESHFVF	D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
#define SKINFVF	D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1

class G3D_CMaterial;
class G3D_CCameraObject;
class G3D_CKeyframer;
class G3D_CObject;

template<class T> class CNameMap;

#pragma pack(4)
typedef struct _tag_MESHVERTEX
{
	D3DVECTOR		p;
	D3DVECTOR		n;
	FLOAT			u, v;
} MESHVERTEX, *LPMESHVERTEX, *PMESHVERTEX;
#pragma pack()

#pragma pack(4)
typedef struct _tag_SKINVERTEX
{
	D3DVECTOR		p;
	FLOAT			w0;
	FLOAT			w1;
	FLOAT			w2;
	DWORD			index;
	D3DVECTOR		n;
	FLOAT			u, v;
} SKINVERTEX, *LPSKINVERTEX, *PSKINVERTEX;
#pragma pack()

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

class G3D_CException
{
public:

	DWORD		code;

public:

	G3D_CException( DWORD _code ) : code( _code )
	{
	}
};

class G3D_CScene
{
private:

	CNameMap<G3D_CKeyframer>*	nmKeyframersList;
	CNameMap<G3D_CObject>*		nmObjectsList;
	CNameMap<G3D_CMaterial>*	nmMaterialsList;	

public:

	DWORD						dwFlags;

	PDIRECT3DDEVICE8			pd3dDevice;

	FLOAT						zMin;
	FLOAT						zMax;
	FLOAT						fAspect;

	G3D_CCameraObject*			pcamCurrent;

	DWORD						dwFramesCount;

	DWORD						dwLightsCount;

	G3D_CMaterial*				pmatLight;	

	G3D_CKeyframer*			pkfRoot;

	PDIRECT3DVERTEXBUFFER8		pvbFlare;
	PDIRECT3DINDEXBUFFER8		pibFlare;

	DWORD						hVS;	

public:

	G3D_CScene();
	~G3D_CScene();

	void Initialize( PDIRECT3DDEVICE8 pDevice, DWORD dwFlags = 0 );

	G3D_CKeyframer* GetKeyframer( const char* name );
	G3D_CObject* GetObject( const char* name );
	G3D_CMaterial* GetMaterial( const char* name );
	G3D_CKeyframer* GetObjectKeyframer( const char* name );
	G3D_CKeyframer* GetObjectKeyframer( G3D_CObject* obj );

	void AddMaterial( const char* name, G3D_CMaterial* mat );
	void AddObject( const char* name, G3D_CObject* object );
	void AddKeyframer( const char* name, G3D_CKeyframer* keyframer );
	void ReplaceMaterial( const char* name, G3D_CMaterial* mat );

	void SetFlags( DWORD flags );
	void SetVieport( FLOAT zmin, FLOAT zmax, FLOAT aspect );
	void SetLightMaterial( G3D_CMaterial* mat );

	void RenderFrame( FLOAT frame );
	void Transform( FLOAT frame );
	void Render();
	
	void Load(IDirect3DDevice8 *pDevice, const char* filename );

	void CloseScene();

protected :

	CFlareTrack		*m_pFlareTrack;

};

#endif