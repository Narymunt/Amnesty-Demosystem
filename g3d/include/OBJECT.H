
//	CObject
//	 |
//	 +---------------+
//	 |				 |
//	CMesh			CPointObject
//	 |				 |
//	 +				 +---------------+---------------+
//	 |				 |				 |				 |
//  CSkin			CCameraObject	CLight			CTarget
//					 |
//					 +---------------+
//					 |				 |
//					CViewer         CCamera

#ifndef	__OBJECT_H_
#define __OBJECT_H_

#include "math3d.h"

enum G3D_OBJECTTYPES
{
	G3DOT_SIMPLEMESH,
	G3DOT_MESH,
	G3DOT_SKIN,
	G3DOT_CAMERA,
	G3DOT_VIEWER,
	G3DOT_LIGHT,
	G3DOT_TARGET,
	G3DOT_BONE,
	G3DOT_FORCEDWORD = 0x7fffffff
};

class G3D_CScene;
class G3D_CValueEnvelope;
class G3D_CDataEnvelope;
class G3D_CMaterial;
class G3D_CKeyframer;

//	name:	CObject
//	desc:	base class for object hierarchy 

class G3D_CObject
{
public:
	
	G3D_CScene*			pScene;								// scena do ktorej nalezy obiekt

public:

	G3D_CObject( G3D_CScene* scene ) : pScene( scene )
	{
	}

	virtual ~G3D_CObject()
	{
	}

	virtual DWORD Type() = 0;
	virtual void Render() = 0;	
	virtual void UpdateFrame( G3D_CKeyframer* keyframer ) = 0;
};


class G3D_CPointObject	: public G3D_CObject								
{
public:

	G3D_CVector			vPos;

public:

	G3D_CPointObject( G3D_CScene* scene ) : G3D_CObject( scene )
	{
	}	

	virtual DWORD Type() = 0;

	virtual void Render()
	{
	}

	virtual void UpdateFrame( G3D_CKeyframer* keyframer );	
};

class G3D_CLight : public G3D_CPointObject
{
public:

	DWORD					dwLightID;
	G3D_CVector			vColor;

public:

	G3D_CLight( G3D_CScene* scene ) : G3D_CPointObject( scene )
	{
	}	

	virtual DWORD Type()
	{
		return G3DOT_LIGHT;
	}

	virtual void Render();
	virtual void UpdateFrame( G3D_CKeyframer* keyframer );
};

class G3D_CTarget : public G3D_CPointObject
{
public:

	G3D_CTarget( G3D_CScene* scene ) : G3D_CPointObject( scene )
	{
	}	

	virtual DWORD Type()
	{
		return G3DOT_TARGET;
	}	
};

class G3D_CCameraObject : public G3D_CPointObject
{
public:

	G3D_CValueEnvelope*	pveFov;
	FLOAT					fFov;

	G3D_CMatrix			mtxCamera;

public:

	G3D_CCameraObject( G3D_CScene* scene ) : G3D_CPointObject( scene ), fFov( 0.33f*H_PI ), pveFov( NULL )
	{		
	}

	virtual ~G3D_CCameraObject();

	virtual void UpdateFrame( G3D_CKeyframer* keyframer );
	virtual void SetGeometry() = 0;	
};

class G3D_CViewer : public G3D_CCameraObject
{
public:

	G3D_CValueEnvelope*	pveRoll;
	FLOAT					fRoll;

	G3D_CTarget*			pTrg;

public:

	G3D_CViewer( G3D_CScene* scene ) : G3D_CCameraObject( scene ), fRoll( 0.0f ), pveRoll( NULL )
	{
	}

	virtual ~G3D_CViewer();

	virtual DWORD Type()
	{
		return G3DOT_VIEWER;
	}

	virtual void UpdateFrame( G3D_CKeyframer* keyframer );
	virtual void SetGeometry();	
};

class G3D_CCamera : public G3D_CCameraObject
{
public:

	G3D_CCamera( G3D_CScene* scene ) : G3D_CCameraObject( scene )
	{
	}

	virtual DWORD Type()
	{
		return G3DOT_VIEWER;
	}

	virtual void UpdateFrame( G3D_CKeyframer* keyframer );
	virtual void SetGeometry();
};

class G3D_CBoundingBox
{
public:

	G3D_CVector			bbmin;
	G3D_CVector			bbmax;

public:

	DWORD IsVisible( PDIRECT3DDEVICE8 pDevice );
};

class G3D_CMesh : public G3D_CObject
{
public:	

	G3D_CBoundingBox		bbBoundingBox;

	G3D_CDataEnvelope*		pdeMorph;
	G3D_CDataEnvelope*		pdeHide;

	G3D_CMesh*				pmeshMorphSrc;
	G3D_CMesh*				pmeshMorphTrg;
	FLOAT					fMorphFactor;
	
	DWORD					dwHide;

	G3D_CMatrix			mtxTransform;

	PDIRECT3DVERTEXBUFFER8	pvbVertices;
	PDIRECT3DINDEXBUFFER8	pibIndices;

	G3D_CVector			vCenter;

	G3D_CMaterial*			pMaterial;

	union
	{	
		DWORD				dwFVF;
		DWORD				dwVS;
	};

	DWORD					dwFlags;	
	DWORD					dwVerticesCount;
	DWORD					dwFacesCount;
	DWORD					dwIndicesCount;

public:

	G3D_CMesh( G3D_CScene* scene ) : G3D_CObject( scene ), dwHide( FALSE ), pdeMorph( NULL ), pdeHide( NULL )
	{
	}

	virtual ~G3D_CMesh();

	virtual DWORD Type()
	{
		return G3DOT_SIMPLEMESH;
	}
	
	virtual void Render();	
	virtual void UpdateFrame( G3D_CKeyframer* keyframer );
};


class G3D_CSegmentedMesh : public G3D_CMesh
{
public:

	struct G3D_CSegment
	{	
		DWORD				dwStartIndex;
		DWORD				dwFacesCount;

		G3D_CMaterial*		pMaterial;
	};

	DWORD					dwSegmentsCount;
	G3D_CSegment*			ptabSegments;

public:

	G3D_CSegmentedMesh( G3D_CScene* scene ) : G3D_CMesh( scene )
	{
	}

	virtual ~G3D_CSegmentedMesh();

	virtual DWORD Type()
	{
		return G3DOT_MESH;
	}
	
	virtual void Render();	
};

class G3D_CSkin : public G3D_CSegmentedMesh
{
public:

	class G3D_CBone
	{
	public:
		
		G3D_CKeyframer*	pBoneKeyframer;
		G3D_CMatrix		mtxRest;
	};

	DWORD					dwBonesCount;
	G3D_CBone*				ptabBones;

public:

	G3D_CSkin( G3D_CScene* scene ) : G3D_CSegmentedMesh( scene ), dwBonesCount(0)
	{
	}

	virtual ~G3D_CSkin();

	virtual DWORD Type()
	{
		return G3DOT_SKIN;
	}
	
	virtual void Render();	
};

#endif