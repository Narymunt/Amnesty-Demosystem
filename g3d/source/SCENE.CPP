#include "material.h"
#include "object.h"
#include "keyframer.h"
#include "track.h"
#include "scene.h"
#include "namemap.h"
#include "math3d.h"
#include "texture.h"
#include <stdio.h>

enum FILEIDS
{
	ID_MATERIALBLOCK		= 0x1010,
	ID_OBJECTBLOCK			= 0x2020,
	ID_KEYFRAMERBLOCK		= 0x3030,

	ID_CAMERA				= 0xff01,
	ID_TARGET				= 0xff02,
	ID_LIGHT				= 0xff03,
	ID_MESH					= 0xff04,
	ID_SKIN					= 0xff05,
	ID_VIEWER				= 0xff06,

	ID_BONE					= 0xee01,
	ID_MESHKEYFRAMER		= 0xee02,
	ID_LIGHTKEYFRAMER		= 0xee03,
	ID_VIEWERKEYFRAMER		= 0xee04,
	ID_CAMERAKEYFRAMER		= 0xee05,
	ID_TARGETKEYFRAMER		= 0xee06,
	ID_NULLKEYFRAMER		= 0xee07,
};

enum KEYFRAMERFLAGS
{
	KFF_SUB			= 1,
	KFF_NEXT		= 2,
	KFF_EULERROT	= 4,
	KFF_MORPH		= 8,
	KFF_HIDE		= 16,
	KFF_FOV			= 32,
	KFF_ROLL		= 64
};

G3D_CScene::G3D_CScene() : dwFlags(0), zMin(1.0f), zMax(1000.0f), fAspect(0.75f), 
							 pmatLight(NULL), 
							 pkfRoot(NULL), 
							 pcamCurrent(NULL), 
							 dwLightsCount(0), 
							 hVS(0), 
							 pvbFlare(NULL),
							 pibFlare(NULL), 
							 pd3dDevice(NULL)
{
	G3D_CMaterial*			pmatEmpty;

	nmMaterialsList		= new CNameMap<G3D_CMaterial>;
	nmObjectsList		= new CNameMap<G3D_CObject>;
	nmKeyframersList	= new CNameMap<G3D_CKeyframer>;
	
	pmatEmpty			= new G3D_CMaterial( this );
	pmatEmpty->dwFlags	= FLAG_NOTEXT;

	nmMaterialsList->Add( "$EMPTYMATERIAL", pmatEmpty );
}

G3D_CScene::~G3D_CScene()
{
	if( hVS )
		pd3dDevice->DeleteVertexShader( hVS );

	SAFE_RELEASE( pvbFlare );
	SAFE_RELEASE( pibFlare );

	SAFE_DELETE( nmMaterialsList );
	SAFE_DELETE( nmObjectsList );
	SAFE_DELETE( nmKeyframersList );	
}

void G3D_CScene::Initialize( PDIRECT3DDEVICE8 pDevice, DWORD flags )
{
	D3DVECTOR*			vec;
	LPWORD				ind;
	DWORD				vsdec[] = 
	{
		D3DVSD_STREAM( 0 ),
			D3DVSD_REG( D3DVSDE_POSITION,	D3DVSDT_FLOAT3 ),
			D3DVSD_REG( D3DVSDE_NORMAL,		D3DVSDT_FLOAT3 ),		
			D3DVSD_REG( D3DVSDE_TEXCOORD0,	D3DVSDT_FLOAT2 ),
			D3DVSD_STREAM( 1 ),		
			D3DVSD_REG( D3DVSDE_POSITION2,	D3DVSDT_FLOAT3 ),
			D3DVSD_REG( D3DVSDE_NORMAL2,	D3DVSDT_FLOAT3 ),		
			D3DVSD_END()
	};
	
	D3DDEVICE_CREATION_PARAMETERS	d3ddcp;
	
	pd3dDevice = pDevice;

	dwFlags = flags;
	
	pd3dDevice->GetCreationParameters( &d3ddcp );
	
	if( d3ddcp.BehaviorFlags&D3DCREATE_MIXED_VERTEXPROCESSING )
		dwFlags |= FLAG_HW;	
	
	if( pd3dDevice )
	{		
		if( FAILED( pd3dDevice->CreateVertexBuffer( 4*3*sizeof(FLOAT), D3DUSAGE_SOFTWAREPROCESSING, D3DFVF_XYZ, D3DPOOL_DEFAULT, &pvbFlare ) ) )
			return; //throw G3D_CException( G3DERR_UNABLETOINITIALIZE );
		
		if( FAILED( pd3dDevice->CreateIndexBuffer( 6*sizeof(WORD), D3DUSAGE_SOFTWAREPROCESSING, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pibFlare ) ) )
			return; //throw G3D_CException( G3DERR_UNABLETOINITIALIZE );
		
		if( FAILED( pvbFlare->Lock( 0, 0, (LPBYTE*)&vec, 0 ) ) )
			return; //throw G3D_CException( G3DERR_UNABLETOINITIALIZE );
		
		vec[0].x = -7.5f;
		vec[0].y = 7.5f;
		vec[0].z = 0.0f;
		
		vec[1].x = 7.5f;
		vec[1].y = 7.5f;
		vec[1].z = 0.0f;
		
		vec[2].x = 7.5f;
		vec[2].y = -7.5f;
		vec[2].z = 0.0f;
		
		vec[3].x = -7.5f;
		vec[3].y = -7.5f;
		vec[3].z = 0.0f;
		
		if( FAILED( pvbFlare->Unlock() ) )
			return; //throw G3D_CException( G3DERR_UNABLETOINITIALIZE );
		
		if( FAILED( pibFlare->Lock( 0, 0, (LPBYTE*)&ind, 0 ) ) )
			return; //throw G3D_CException( G3DERR_UNABLETOINITIALIZE );
		
		ind[0] = 0;
		ind[1] = 2;
		ind[2] = 1;
		
		ind[3] = 0;
		ind[4] = 3;
		ind[5] = 2;
		
		if( FAILED( pibFlare->Unlock() ) )
			return; //throw G3D_CException( G3DERR_UNABLETOINITIALIZE );
		
		if( FAILED( pd3dDevice->CreateVertexShader( vsdec, NULL, &hVS, D3DUSAGE_SOFTWAREPROCESSING ) ) )
			return; //throw G3D_CException( G3DERR_UNABLETOINITIALIZE );		
	}
}

G3D_CKeyframer* G3D_CScene::GetKeyframer( const char* name )
{
	return nmKeyframersList->Get( name );
}

G3D_CObject* G3D_CScene::GetObject( const char* name )
{
	return nmObjectsList->Get( name );
}

G3D_CMaterial* G3D_CScene::GetMaterial( const char* name )
{
	return nmMaterialsList->Get( name );
}

G3D_CKeyframer* G3D_CScene::GetObjectKeyframer( const char* name )
{
	G3D_CObject*			object = GetObject( name );

	return GetObjectKeyframer( object );
}

G3D_CKeyframer* G3D_CScene::GetObjectKeyframer( G3D_CObject* obj )
{
	G3D_CKeyframer*		keyframer;

	nmKeyframersList->SetToRoot();

	while( keyframer = nmKeyframersList->GetNext() )
	{
		if( keyframer->poKeyframerOwner == obj )
			return keyframer;
	}

	return NULL;
}

void G3D_CScene::AddMaterial( const char* name, G3D_CMaterial* mat )
{
	nmMaterialsList->Add( name, mat );
}

void G3D_CScene::ReplaceMaterial( const char* name, G3D_CMaterial* mat )
{
	G3D_CMaterial*			oldmat = nmMaterialsList->Get( name );
	G3D_CSegmentedMesh*	mesh;
	G3D_CObject*			obj;

	if( !oldmat )
		return;

	nmObjectsList->SetToRoot();

	while( obj = nmObjectsList->GetNext() )
	{
		if( obj->Type() == G3DOT_MESH || obj->Type() == G3DOT_SKIN )
		{
			mesh = (G3D_CSegmentedMesh*)obj;

			for( DWORD i = 0 ; i < mesh->dwSegmentsCount ; i++ )
			{
				if( mesh->ptabSegments[i].pMaterial == oldmat )				
					mesh->ptabSegments[i].pMaterial = mat;				
			}
		}
	}

	nmMaterialsList->Remove( name );
	nmMaterialsList->Add( name, mat );
}

void G3D_CScene::AddObject( const char* name, G3D_CObject* object )
{
	nmObjectsList->Add( name, object );
}

void G3D_CScene::AddKeyframer( const char* name, G3D_CKeyframer* keyframer )
{
	nmKeyframersList->Add( name, keyframer );
}

void G3D_CScene::SetFlags( DWORD flags )
{
	dwFlags = flags;
}

void G3D_CScene::SetVieport( FLOAT zmin, FLOAT zmax, FLOAT aspect )
{
	zMin	= zmin;
	zMax	= zMax;
	fAspect = aspect;
}

void G3D_CScene::SetLightMaterial( G3D_CMaterial* mat )
{
	pmatLight = mat;
}

void G3D_CScene::RenderFrame( FLOAT frame, long lTimer )
{	
	Transform( frame );
	Render();
	m_pFlareTrack->Render((unsigned long)frame,lTimer);
}

void G3D_CScene::Transform( FLOAT frame )
{
	G3D_CMtxStack			stack;
	G3D_CMatrix			mtx = IdentMtx();

	stack.Push( mtx );

	pkfRoot->Transform( frame, stack );

	pcamCurrent->SetGeometry();
}

void G3D_CScene::Render()
{
	if( !pd3dDevice ) return;

	G3D_CObject*			object;	
	
	nmObjectsList->SetToRoot();

	while( object = nmObjectsList->GetNext() )
		object->Render();	

}
	
void G3D_CScene::CloseScene()
{
	if( !pd3dDevice ) return;

	for( DWORD i = 0; i < dwLightsCount ; i++ )
		pd3dDevice->LightEnable( i, FALSE );
}

FLOAT ReadFloat( FILE* in )
{
	FLOAT					f;

	if( feof( in ) ) return NULL;

	fread( &f, sizeof(FLOAT), 1, in );

	return f;
}

WORD ReadWord( FILE* in )
{
	WORD					w;

	if( feof( in ) ) return NULL;

	fread( &w, sizeof(WORD), 1, in );

	return w;
}

DWORD ReadDword( FILE* in )
{
	DWORD					dw;

	if( feof( in ) ) return NULL;

	fread( &dw, sizeof(DWORD), 1, in );

	return dw;
}

char* ReadString( FILE* in )
{	
	char*					out;
	static char				buffer[255];
	DWORD					i = 0;

	do 
	{
		if( feof( in ) ) return NULL;

		fread( &(buffer[i]), sizeof(char), 1, in );				

	} while( buffer[i++] != 0 );

	out = new char[strlen(buffer)+1];
	strcpy( out, buffer );

	return out;
}

G3D_CVector ReadVector( FILE* in )
{
	G3D_CVector			vec;

	vec.x = ReadFloat( in );
	vec.y = ReadFloat( in );
	vec.z = ReadFloat( in );

	return vec;
}

void ReadMaterialBlock( G3D_CScene* scene, FILE* in )
{
	DWORD					count = ReadDword( in );
	char*					name;
	G3D_CMaterial*			mat;

	for( DWORD i = 0 ; i < count ; i++ )
	{
		name	= ReadString( in );
		mat		= new G3D_CMaterial( scene );

		scene->AddMaterial( name, mat );

		delete name;

		mat->dwFlags		= ReadDword( in );

		mat->a				= ReadFloat( in );
		mat->r				= ReadFloat( in );
		mat->g				= ReadFloat( in );
		mat->b				= ReadFloat( in );

		if( !(mat->dwFlags&FLAG_NOTEXT ) )
		{		

			try
			{
				mat->strTextureName = ReadString( in );

				if( _stricmp( mat->strTextureName, "" ) )
					if( LoadTextureAutoAlpha( mat->strTextureName ) == AAS_ALPHALOADED )
					{					
						mat->dwFlags	|= FLAG_TEXTUREALPHA | FLAG_ALPHABLEND;
						mat->dwAlphaOp	= ALPHAOP_ADD;
					}
			}
			catch( CTextureException ex )
			{
				if( ex.code == TEXERR_UNABLETOLOAD )
					DeleteTexture( mat->strTextureName );
				else
					throw G3D_CException( G3DERR_TEXTUREEXCEPTION );
			}

			try
			{		
				if( mat->dwFlags & FLAG_MULTITEXT )
				{			
					mat->strEnvTextName = ReadString( in );

					if( _stricmp( mat->strEnvTextName, "" ) )
						if( LoadTextureAutoAlpha( mat->strEnvTextName ) == AAS_ALPHALOADED )
							mat->dwFlags &= FLAG_ENVALPHA;
				}
			}

			catch( CTextureException ex )
			{
				if( ex.code == TEXERR_UNABLETOLOAD )
					DeleteTexture( mat->strTextureName );
				else
					throw G3D_CException( G3DERR_TEXTUREEXCEPTION );
			}
		}
	}
}

void ReadMeshHeader( G3D_CSegmentedMesh* mesh, FILE* in )
{
	mesh->dwVerticesCount	= ReadDword( in );
	mesh->dwFacesCount		= ReadDword( in );
	mesh->dwIndicesCount	= ReadDword( in );	
	mesh->dwHide			= ReadDword( in );
	mesh->vCenter			= ReadVector( in );
	mesh->dwSegmentsCount	= ReadDword( in );				
}

void ReadObjectBlock( G3D_CScene* scene, FILE* in )
{
	DWORD					count = ReadDword( in );
	DWORD					objectid;
	char*					name;
	G3D_CObject*			object;
	

	for( DWORD i = 0 ; i < count ; i++ )
	{
		name	= ReadString( in );
		objectid= ReadDword( in );

		switch( objectid )
		{
			case ID_CAMERA:
			{
				G3D_CCamera*	cam = new G3D_CCamera( scene );

				object		= cam;
				cam->vPos	= ReadVector( in );
				cam->fFov	= ReadFloat( in );
			}
			break;

			case ID_VIEWER:
			{
				G3D_CViewer*	cam = new G3D_CViewer( scene );

				object		= cam;
				cam->vPos	= ReadVector( in );
				cam->fFov	= ReadFloat( in );
				cam->fRoll	= ReadFloat( in );
			}
			break;

			case ID_LIGHT:
			{
				G3D_CLight*		light = new G3D_CLight( scene );

				object		= light;
				light->vPos	= ReadVector( in );
				light->dwLightID = scene->dwLightsCount++;
			}
			break;

			case ID_TARGET:
			{
				G3D_CTarget*	trg = new G3D_CTarget( scene );
				char*			camname = ReadString( in );
				G3D_CViewer*	cam = (G3D_CViewer*)scene->GetObject( camname );

				cam->pTrg	= trg;
				object		= trg;
				trg->vPos	= ReadVector( in );

				delete camname;
			}
			break;

			case ID_MESH:
			{
				G3D_CSegmentedMesh*	mesh = new G3D_CSegmentedMesh( scene );
				DWORD					morph;
				LPWORD					indices;
				PMESHVERTEX				vertices;
				char*					matname;
				DWORD					k;

				FLOAT					xmin, xmax;
				FLOAT					ymin, ymax;
				FLOAT					zmin, zmax;

				object		= mesh;				

				ReadMeshHeader( mesh, in );

				morph		= ReadDword( in );				

				if( !morph )
				{				
					mesh->dwFVF	= MESHFVF;
										
					scene->pd3dDevice->CreateVertexBuffer( mesh->dwVerticesCount*sizeof(MESHVERTEX), D3DUSAGE_WRITEONLY, 
														   MESHFVF, D3DPOOL_DEFAULT, &mesh->pvbVertices );
					scene->pd3dDevice->CreateIndexBuffer( mesh->dwIndicesCount*sizeof(WORD), D3DUSAGE_WRITEONLY, 
														  D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mesh->pibIndices );					
				}
				else
				{
					mesh->dwVS	= scene->hVS;
					scene->pd3dDevice->CreateVertexBuffer( mesh->dwVerticesCount*sizeof(MESHVERTEX), D3DUSAGE_SOFTWAREPROCESSING|D3DUSAGE_WRITEONLY, 
														   0, D3DPOOL_DEFAULT, &mesh->pvbVertices );
					scene->pd3dDevice->CreateIndexBuffer( mesh->dwIndicesCount*sizeof(WORD), D3DUSAGE_SOFTWAREPROCESSING|D3DUSAGE_WRITEONLY, 
														  D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mesh->pibIndices );

				}				

				mesh->pvbVertices->Lock( 0, 0, (LPBYTE*)&vertices, 0 );

				vertices[0].p	= ReadVector( in );
				vertices[0].n	= ReadVector( in );
				vertices[0].u	= ReadFloat( in );
				vertices[0].v	= ReadFloat( in );

				xmin = vertices[0].p.x;
				xmax = vertices[0].p.x;

				ymin = vertices[0].p.y;
				ymax = vertices[0].p.y;

				zmin = vertices[0].p.z;
				zmax = vertices[0].p.z;

				for( k = 1 ; k < mesh->dwVerticesCount ; k++ )
				{
					vertices[k].p	= ReadVector( in );
					vertices[k].n	= ReadVector( in );
					vertices[k].u	= ReadFloat( in );
					vertices[k].v	= ReadFloat( in );

					if( vertices[k].p.x < xmin )
						xmin = vertices[k].p.x;
					else if( vertices[k].p.x > xmax )
						xmax = vertices[k].p.x;

					if( vertices[k].p.y < ymin )
						ymin = vertices[k].p.y;
					else if( vertices[k].p.y > ymax )
						ymax = vertices[k].p.y;

					if( vertices[k].p.z < zmin )
						zmin = vertices[k].p.z;
					else if( vertices[k].p.z > zmax )
						zmax = vertices[k].p.z;
				}

				mesh->pvbVertices->Unlock();

				mesh->bbBoundingBox.bbmin = G3D_CVector( xmin, ymin, zmin );
				mesh->bbBoundingBox.bbmax = G3D_CVector( xmax, ymax, zmax );

				mesh->pibIndices->Lock( 0, 0, (LPBYTE*)&indices, 0 );

				for( k = 0 ; k < mesh->dwIndicesCount ; k++ )				
					indices[k] = ReadWord( in );				

				mesh->pibIndices->Unlock();				

				mesh->ptabSegments		= new G3D_CSegmentedMesh::G3D_CSegment[mesh->dwSegmentsCount];

				for( k = 0 ; k < mesh->dwSegmentsCount ; k++ )
				{
					matname = ReadString( in );

					mesh->ptabSegments[k].dwStartIndex = ReadDword( in );
					mesh->ptabSegments[k].dwFacesCount = ReadDword( in );					

					mesh->ptabSegments[k].pMaterial		= scene->GetMaterial( matname );

					delete matname;
				}
				
				mesh->pMaterial = mesh->ptabSegments[0].pMaterial;								
			}
			break;

			case ID_SKIN:
			{
				G3D_CSkin*				skin = new G3D_CSkin( scene );
				LPWORD					indices;
				PSKINVERTEX				vertices;
				char*					matname;
				DWORD					k;
				
				object		= skin;				
				
				ReadMeshHeader( skin, in );				
				skin->dwBonesCount = ReadDword( in );				

				skin->ptabBones = new G3D_CSkin::G3D_CBone[skin->dwBonesCount];
													
				skin->dwFVF	= SKINFVF;
				scene->pd3dDevice->CreateVertexBuffer( skin->dwVerticesCount*sizeof(SKINVERTEX), D3DUSAGE_SOFTWAREPROCESSING|D3DUSAGE_WRITEONLY, 
													   SKINFVF, D3DPOOL_DEFAULT, &skin->pvbVertices );				
				scene->pd3dDevice->CreateIndexBuffer( skin->dwIndicesCount*sizeof(WORD), D3DUSAGE_SOFTWAREPROCESSING|D3DUSAGE_WRITEONLY, 
													  D3DFMT_INDEX16, D3DPOOL_DEFAULT, &skin->pibIndices );
				
				skin->pvbVertices->Lock( 0, 0, (LPBYTE*)&vertices, 0 );
				
				for( k = 0 ; k < skin->dwVerticesCount ; k++ )
				{
					vertices[k].p		= ReadVector( in );
					vertices[k].n		= ReadVector( in );
					vertices[k].w0		= ReadFloat( in );
					vertices[k].w1		= ReadFloat( in );
					vertices[k].w2		= ReadFloat( in );
					vertices[k].index	= ReadDword( in );
					vertices[k].u		= ReadFloat( in );
					vertices[k].v		= ReadFloat( in );
				}
				
				skin->pvbVertices->Unlock();
				
				skin->pibIndices->Lock( 0, 0, (LPBYTE*)&indices, 0 );
				
				for( k = 0 ; k < skin->dwIndicesCount ; k++ )				
					indices[k] = ReadWord( in );				
				
				skin->pibIndices->Unlock();				
				
				skin->ptabSegments		= new G3D_CSegmentedMesh::G3D_CSegment[skin->dwSegmentsCount];
				
				for( k = 0 ; k < skin->dwSegmentsCount ; k++ )
				{
					matname = ReadString( in );
					
					skin->ptabSegments[k].dwStartIndex = ReadDword( in );
					skin->ptabSegments[k].dwFacesCount = ReadDword( in );					
					
					skin->ptabSegments[k].pMaterial		= scene->GetMaterial( matname );
					
					delete matname;
				}
								
				skin->pMaterial = skin->ptabSegments[0].pMaterial;				
				
			}
			break;
		}
		
		scene->AddObject( name, object );
		delete name;
	}
}

void ReadVectorTrack( G3D_CVectorTrack* track, FILE* in )
{
	track->dwKeysCount	= ReadDword( in );
	track->pkKeys		= new G3D_CVectorTrack::CKey[track->dwKeysCount];

	for( DWORD i = 0 ; i < track->dwKeysCount ; i++ )
	{
		track->pkKeys[i].key		= ReadVector( in );
		track->pkKeys[i].frame		= ReadFloat( in );
		track->pkKeys[i].T			= ReadFloat( in );
		track->pkKeys[i].B			= ReadFloat( in );
		track->pkKeys[i].C			= ReadFloat( in );
		track->pkKeys[i].easeto		= ReadFloat( in );
		track->pkKeys[i].easefrom	= ReadFloat( in );
	}

	track->InitTrack();
}

void ReadQuaternionTrack( G3D_CQuaternionTrack* track, FILE* in )
{
	track->dwKeysCount	= ReadDword( in );
	track->pkKeys		= new G3D_CQuaternionTrack::CKey[track->dwKeysCount];

	for( DWORD i = 0 ; i < track->dwKeysCount ; i++ )
	{
		track->pkKeys[i].axis		= ReadVector( in );
		track->pkKeys[i].angle		= ReadFloat( in );
		track->pkKeys[i].frame		= ReadFloat( in );
		track->pkKeys[i].T			= ReadFloat( in );
		track->pkKeys[i].B			= ReadFloat( in );
		track->pkKeys[i].C			= ReadFloat( in );
		track->pkKeys[i].easeto		= ReadFloat( in );
		track->pkKeys[i].easefrom	= ReadFloat( in );
	}

	track->InitTrack();
}

void ReadMorphEnvelope( G3D_CScene* scene, G3D_CDataEnvelope* envelope, FILE* in )
{
	envelope->dwKeysCount	= ReadDword( in );
	envelope->pkKeys		= new G3D_CDataEnvelope::CKey[envelope->dwKeysCount];

	for( DWORD i = 0 ; i < envelope->dwKeysCount ; i++ )
	{
		char*				name = ReadString( in );

		envelope->pkKeys[i].key		= (DWORD)scene->GetObject( name );
		envelope->pkKeys[i].frame	= ReadFloat( in );
		envelope->pkKeys[i].easeto	= ReadFloat( in );
		envelope->pkKeys[i].easefrom= ReadFloat( in );

		delete name;
	}

	envelope->InitEnvelope();
}

void ReadHideEnvelope( G3D_CDataEnvelope* envelope, FILE* in )
{
	envelope->dwKeysCount	= ReadDword( in );
	envelope->pkKeys		= new G3D_CDataEnvelope::CKey[envelope->dwKeysCount];

	for( DWORD i = 0 ; i < envelope->dwKeysCount ; i++ )
	{		
		envelope->pkKeys[i].key		= ReadDword( in );
		envelope->pkKeys[i].frame	= ReadFloat( in );
		envelope->pkKeys[i].easeto	= ReadFloat( in );
		envelope->pkKeys[i].easefrom= ReadFloat( in );
	}

	envelope->InitEnvelope();
}

void ReadValueEnvelope( G3D_CValueEnvelope* envelope, FILE* in )
{
	envelope->dwKeysCount	= ReadDword( in );
	envelope->pkKeys		= new G3D_CValueEnvelope::CKey[envelope->dwKeysCount];

	for( DWORD i = 0 ; i < envelope->dwKeysCount ; i++ )
	{		
		envelope->pkKeys[i].key		= ReadFloat( in );
		envelope->pkKeys[i].frame	= ReadFloat( in );
		envelope->pkKeys[i].T		= ReadFloat( in );
		envelope->pkKeys[i].B		= ReadFloat( in );
		envelope->pkKeys[i].C		= ReadFloat( in );
		envelope->pkKeys[i].easeto	= ReadFloat( in );
		envelope->pkKeys[i].easefrom= ReadFloat( in );
	}

	envelope->InitEnvelope();
}

void ReadKeyframer( G3D_CScene* scene, G3D_CKeyframer* keyframer, FILE* in )
{	
	DWORD					keyframerid;
	char*					name;
	char*					objectname;
	G3D_CObject*			object;
	DWORD					flags;

	keyframerid	= ReadDword( in );
	flags		= ReadDword( in );
	name		= ReadString( in );
	objectname	= ReadString( in );	

	scene->AddKeyframer( name, keyframer );

	if( keyframerid != ID_NULLKEYFRAMER )
		object = scene->GetObject( objectname );

	if( keyframerid != ID_BONE && keyframerid != ID_NULLKEYFRAMER )
		keyframer->poKeyframerOwner = object;	

	keyframer->ptPosition = new G3D_CPositionTrack;
	ReadVectorTrack( (G3D_CVectorTrack*)keyframer->ptPosition, in );

	if( flags&KFF_EULERROT )
	{	
		keyframer->ptRotation = new G3D_CEulerRotTrack;
		ReadVectorTrack( (G3D_CVectorTrack*)keyframer->ptRotation, in );
	}
	else
	{
		keyframer->ptRotation = new G3D_CQuaternionTrack;
		ReadQuaternionTrack( (G3D_CQuaternionTrack*)keyframer->ptRotation, in );
	}

	keyframer->ptScale	  = new G3D_CScaleTrack;
	ReadVectorTrack( (G3D_CVectorTrack*)keyframer->ptScale, in );

	switch( keyframerid )
	{
		case ID_MESHKEYFRAMER:
		{
			G3D_CMesh*			mesh = (G3D_CMesh*)object;

			if( flags&KFF_MORPH )
			{
				mesh->pdeMorph	= new G3D_CDataEnvelope;
				ReadMorphEnvelope( scene, mesh->pdeMorph, in );
			}

			if( flags&KFF_HIDE )
			{
				mesh->pdeHide	= new G3D_CDataEnvelope;
				ReadHideEnvelope( mesh->pdeHide, in );
			}
		}
		break;

		case ID_VIEWERKEYFRAMER:
		{			
			G3D_CViewer*		viewer = (G3D_CViewer*)object;

			if( flags&KFF_ROLL )
			{				
				viewer->pveRoll = new G3D_CValueEnvelope;
				ReadValueEnvelope( viewer->pveRoll, in );
			}

			if( flags&KFF_FOV )
			{				
				viewer->pveFov = new G3D_CValueEnvelope;
				ReadValueEnvelope( viewer->pveFov, in );				
			}
		}
		break;
			
		case ID_CAMERAKEYFRAMER:
		{	
			G3D_CCameraObject*	cam = (G3D_CCamera*)object;

			if( flags&KFF_FOV )
			{				
				cam->pveFov = new G3D_CValueEnvelope;
				ReadValueEnvelope( cam->pveFov, in );				
			}
		}
		break;

		case ID_BONE:
		{
			G3D_CSkin*				skin = (G3D_CSkin*)object;			
			DWORD					boneIndex;
			G3D_CMatrix			mtx;

			boneIndex = ReadDword( in );

			for( DWORD i = 1 ; i <=4 ; i++ )
				for( DWORD j = 1 ; j <= 4 ; j++ )
					mtx( i, j ) = ReadFloat( in );
			
			skin->ptabBones[boneIndex-1].mtxRest		= mtx;
			skin->ptabBones[boneIndex-1].pBoneKeyframer = keyframer;
			keyframer->poKeyframerOwner					= NULL;			
		}
		break;
	}

	delete name;
	delete objectname;
		
	if( flags&KFF_SUB )
	{
		keyframer->pkfSub = new G3D_CKeyframer( scene );
		ReadKeyframer( scene, keyframer->pkfSub, in );
	}

	if( flags&KFF_NEXT )
	{
		keyframer->pkfNext = new G3D_CKeyframer( scene );
		ReadKeyframer( scene, keyframer->pkfNext, in );
	}
}


void G3D_CScene::Load(IDirect3DDevice8 *pDevice, const char* filename, char cFlareFilename[] )
{
	if( !pd3dDevice )
		throw G3D_CException( G3DERR_NOTINITIALIZED );

	FILE*					in = fopen( filename, "rb" );
	char*					fileid = ReadString( in );
	G3D_CObject*			obj;

	if( strcmp( fileid, "FOX3D" ) ) return;

	delete fileid;

	// wczytaj flarki

	m_pFlareTrack = new CFlareTrack(pDevice, in, cFlareFilename);

	// koniec czytania flarek

	dwFramesCount = ReadDword( in );

	if( ReadDword( in ) != ID_MATERIALBLOCK ) return;

	ReadMaterialBlock( this, in );

	if( ReadDword( in ) != ID_OBJECTBLOCK ) return;

	ReadObjectBlock( this, in );

	if( ReadDword( in ) != ID_KEYFRAMERBLOCK ) return;

	pkfRoot = new G3D_CKeyframer( this );
	ReadKeyframer( this, pkfRoot, in );

	fclose( in );

	nmObjectsList->SetToRoot();

	while( obj = nmObjectsList->GetNext() )
		if( obj->Type() == G3DOT_CAMERA || obj->Type() == G3DOT_VIEWER )
		{
			pcamCurrent = (G3D_CCameraObject*)obj;
			break;
		}

	if( !pcamCurrent ) return;
}

