#include "material.h"
#include "keyframer.h"
#include "track.h"
#include "object.h"
#include "scene.h"

void G3D_CPointObject::UpdateFrame( G3D_CKeyframer* keyframer )
{
	G3D_CVector				vec;

	vPos = vec*keyframer->mtxFrame;
}

void G3D_CLight::Render()
{		
	if( pScene->dwFlags&FLAG_RENDERFLARE && pScene->pmatLight )
	{	
		pScene->pmatLight->ApplyMaterial();

		pScene->pd3dDevice->SetIndices( pScene->pibFlare, 0 );
		pScene->pd3dDevice->SetStreamSource( 0, pScene->pvbFlare, 3*sizeof(FLOAT) );
		pScene->pd3dDevice->SetVertexShader( D3DFVF_XYZ );

		pScene->pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		pScene->pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		pScene->pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		pScene->pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

		G3D_CMatrix		mtx = InverseCameraMtx(pScene->pcamCurrent->mtxCamera)*TranslationMtx( vPos );

		pScene->pd3dDevice->SetTransform( D3DTS_WORLD, mtx );
		
		pScene->pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 4, 0, 2 );		

		pScene->pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		pScene->pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

		pScene->pmatLight->RestoreMaterial();		
	}
}

void G3D_CLight::UpdateFrame( G3D_CKeyframer* keyframer )
{
	G3D_CPointObject::UpdateFrame( keyframer );

	D3DLIGHT8					d3dlight;

	ZeroMemory( &d3dlight, sizeof(D3DLIGHT8) );

	d3dlight.Attenuation0 = 1.0f;
	d3dlight.Attenuation1 = 0.001f;
	d3dlight.Type		  = D3DLIGHT_POINT;
	d3dlight.Diffuse.a	  = 0.0f;
	d3dlight.Diffuse.r	  = 0.75f;
	d3dlight.Diffuse.g	  = 0.75f;
	d3dlight.Diffuse.b	  = 0.75f;
	d3dlight.Position.x	  = vPos.x;
	d3dlight.Position.y	  = vPos.y;
	d3dlight.Position.z	  = vPos.z;
	d3dlight.Range		  = 500.0f;

	pScene->pd3dDevice->SetLight( dwLightID, &d3dlight );
	pScene->pd3dDevice->LightEnable( dwLightID, TRUE );
}

G3D_CCameraObject::~G3D_CCameraObject()
{
	if( pveFov )
		delete pveFov;
}

void G3D_CCameraObject::UpdateFrame( G3D_CKeyframer* keyframer )
{
	G3D_CPointObject::UpdateFrame( keyframer );

	if( pveFov )
		fFov = pveFov->EnvelopeValue( keyframer->frame );
}

G3D_CViewer::~G3D_CViewer()
{
	if( pveRoll )
		delete pveRoll;
}

void G3D_CViewer::UpdateFrame( G3D_CKeyframer* keyframer )
{
	G3D_CCameraObject::UpdateFrame( keyframer );

	if( pveRoll )
		fRoll = pveRoll->EnvelopeValue( keyframer->frame );
}

void G3D_CViewer::SetGeometry()
{
	G3D_CMatrix				mtxProjection;	

	mtxProjection = ProjectionMtx( fFov, pScene->fAspect, pScene->zMin, pScene->zMax );
	mtxCamera	  = CameraMtx( vPos, pTrg->vPos, fRoll );

	pScene->pd3dDevice->SetTransform( D3DTS_PROJECTION, mtxProjection );		
	pScene->pd3dDevice->SetTransform( D3DTS_VIEW, mtxCamera );
}

void G3D_CCamera::UpdateFrame( G3D_CKeyframer* keyframer )
{
	G3D_CCameraObject::UpdateFrame( keyframer );
	mtxCamera = InverseMtx( keyframer->mtxFrame );
}

void G3D_CCamera::SetGeometry()
{
	G3D_CMatrix				mtxProjection;	

	mtxProjection = ProjectionMtx( fFov, pScene->fAspect, pScene->zMin, pScene->zMax );

	pScene->pd3dDevice->SetTransform( D3DTS_PROJECTION, mtxProjection );
	pScene->pd3dDevice->SetTransform( D3DTS_VIEW, mtxCamera );
}

DWORD G3D_CBoundingBox::IsVisible( PDIRECT3DDEVICE8 pDevice )
{
	enum BBCLIP
	{
		CLIP_LEFT		= 0x01,
		CLIP_RIGHT		= 0x02,
		CLIP_TOP		= 0x04,
		CLIP_BOTTOM		= 0x08,
		CLIP_NEAR		= 0x10,
		CLIP_FAR		= 0x20
	};

	G3D_CVector	v[8];
	FLOAT			x[8], y[8], z[8], w[8];

	DWORD			dwClip;
	DWORD			dwVisible;
	DWORD			i;	

	G3D_CMatrix	mtx;
	G3D_CMatrix	mtxTransform = IdentMtx();

	pDevice->GetTransform( D3DTS_WORLD, mtx );
	mtxTransform *= mtx;
	pDevice->GetTransform( D3DTS_VIEW, mtx );
	mtxTransform *= mtx;
	pDevice->GetTransform( D3DTS_PROJECTION,  mtx );
	mtxTransform *= mtx;

	v[0] = G3D_CVector( bbmin.x, bbmin.y, bbmin.z );
	v[1] = G3D_CVector( bbmax.x, bbmin.y, bbmin.z );
	v[2] = G3D_CVector( bbmin.x, bbmax.y, bbmin.z );
	v[3] = G3D_CVector( bbmax.x, bbmax.y, bbmin.z );
	v[4] = G3D_CVector( bbmin.x, bbmin.y, bbmax.z );
	v[5] = G3D_CVector( bbmax.x, bbmin.y, bbmax.z );
	v[6] = G3D_CVector( bbmin.x, bbmax.y, bbmax.z );
	v[7] = G3D_CVector( bbmax.x, bbmax.y, bbmax.z );
	
	for( i = 0 ; i < 8 ; i++ )
	{
		x[i] = v[i].x*mtxTransform._11 + v[i].y*mtxTransform._21 + v[i].z*mtxTransform._31 + mtxTransform._41;
		y[i] = v[i].x*mtxTransform._12 + v[i].y*mtxTransform._22 + v[i].z*mtxTransform._32 + mtxTransform._42;
		z[i] = v[i].x*mtxTransform._13 + v[i].y*mtxTransform._23 + v[i].z*mtxTransform._33 + mtxTransform._43;
		w[i] = v[i].x*mtxTransform._14 + v[i].y*mtxTransform._24 + v[i].z*mtxTransform._34 + mtxTransform._44;
	}


	for( i = 0, dwVisible = 0xffffffff; i < 8 ; i++ )
	{
		dwClip = 0;

		if( x[i] < -w[i] )
			dwClip |= CLIP_LEFT;
		else if( x[i] > w[i] )
			dwClip |= CLIP_RIGHT;

		if( y[i] < -w[i] )
			dwClip |= CLIP_BOTTOM;
		else if( y[i] > w[i] )
			dwClip |= CLIP_TOP;

		if( z[i] < 0.0f )
			dwClip |= CLIP_NEAR;
		else if( z[i] > w[i] )
			dwClip |= CLIP_FAR;

		dwVisible &= dwClip;
	}	

	return !dwVisible;
}

G3D_CMesh::~G3D_CMesh()
{
	if( pvbVertices )
		pvbVertices->Release();

	if( pibIndices )
		pibIndices->Release();	

	if( pdeMorph )
		delete pdeMorph;

	if( pdeHide )
		delete pdeHide;
}

void G3D_CMesh::UpdateFrame( G3D_CKeyframer* keyframer )
{	
	mtxTransform	= keyframer->mtxFrame;

	if( pdeMorph )
	{	
		pmeshMorphSrc	= (G3D_CMesh*)pdeMorph->PrevData( keyframer->frame );
		pmeshMorphTrg	= (G3D_CMesh*)pdeMorph->NextData( keyframer->frame );
		fMorphFactor	= pdeMorph->LocalTime( keyframer->frame );
	}

	if( pdeHide )
		dwHide			= pdeHide->PrevData( keyframer->frame );
}

void G3D_CMesh::Render()
{
	if( dwHide )
		return;

	pScene->pd3dDevice->SetTransform( D3DTS_WORLD, mtxTransform );	
	
	if( !bbBoundingBox.IsVisible( pScene->pd3dDevice ) )
		return;

	if( pdeMorph && pmeshMorphSrc && pmeshMorphTrg && (pScene->dwFlags&FLAG_MORPH) )
	{
		if( pScene->dwFlags&FLAG_HW )
			pScene->pd3dDevice->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING, TRUE );

		pScene->pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_TWEENING );
		pScene->pd3dDevice->SetStreamSource( 0, pmeshMorphSrc->pvbVertices, sizeof(MESHVERTEX) );
		pScene->pd3dDevice->SetStreamSource( 1, pmeshMorphTrg->pvbVertices, sizeof(MESHVERTEX) );		
	}
	else
		pScene->pd3dDevice->SetStreamSource( 0, pvbVertices, sizeof(MESHVERTEX) );
	
	pScene->pd3dDevice->SetIndices( pibIndices, 0 );
	pScene->pd3dDevice->SetVertexShader( dwVS );	

	pMaterial->ApplyMaterial();
	pScene->pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, dwVerticesCount, 0, dwFacesCount );
	pMaterial->RestoreMaterial();

	pScene->pd3dDevice->SetStreamSource( 0, NULL, 0 );
	pScene->pd3dDevice->SetStreamSource( 1, NULL, 0 );
	pScene->pd3dDevice->SetIndices( NULL, 0 );	

	if( pScene->dwFlags&FLAG_HW )
		pScene->pd3dDevice->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING, FALSE );
}
	
G3D_CSegmentedMesh::~G3D_CSegmentedMesh()
{	
	if( ptabSegments )
		delete[] ptabSegments;	
}


void G3D_CSegmentedMesh::Render()
{
	if( dwHide || (!dwSegmentsCount) )
		return;

	if( dwSegmentsCount == 1 )
	{	
		G3D_CMesh::Render();		
	}
	else
	{	
		pScene->pd3dDevice->SetTransform( D3DTS_WORLD, mtxTransform );	
		
		if( !bbBoundingBox.IsVisible( pScene->pd3dDevice ) )
			return;

		if( pdeMorph && pmeshMorphSrc && pmeshMorphTrg && (pScene->dwFlags&FLAG_MORPH) )
		{
			if( pScene->dwFlags&FLAG_HW )
				pScene->pd3dDevice->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING, TRUE );

			pScene->pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_TWEENING );
			pScene->pd3dDevice->SetStreamSource( 0, pmeshMorphSrc->pvbVertices, sizeof(MESHVERTEX) );
			pScene->pd3dDevice->SetStreamSource( 1, pmeshMorphTrg->pvbVertices, sizeof(MESHVERTEX) );
		}
		else
			pScene->pd3dDevice->SetStreamSource( 0, pvbVertices, sizeof(MESHVERTEX) );
		
		pScene->pd3dDevice->SetIndices( pibIndices, 0 );
		pScene->pd3dDevice->SetVertexShader( dwVS );		

		for( DWORD i = 0 ; i < dwSegmentsCount ; i++ )
		{					
			ptabSegments[i].pMaterial->ApplyMaterial();			
			pScene->pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, dwVerticesCount, ptabSegments[i].dwStartIndex, ptabSegments[i].dwFacesCount );	
			ptabSegments[i].pMaterial->RestoreMaterial();
		}

		pScene->pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );
		pScene->pd3dDevice->SetStreamSource( 0, NULL, 0 );
		pScene->pd3dDevice->SetStreamSource( 1, NULL, 0 );
		pScene->pd3dDevice->SetIndices( NULL, 0 );		

		if( pScene->dwFlags&FLAG_HW )
			pScene->pd3dDevice->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING, FALSE );
	}
}

G3D_CSkin::~G3D_CSkin()
{
	if( ptabBones )
		delete[] ptabBones;	
}


void G3D_CSkin::Render()
{
	G3D_CMatrix			mtx;
	DWORD					i;
	HRESULT					hr;	

	if( dwHide || (!dwBonesCount) )
		return;			

	if( pScene->dwFlags&FLAG_HW )
		pScene->pd3dDevice->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING, TRUE );

	pScene->pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );
	hr = pScene->pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS );
	pScene->pd3dDevice->SetIndices( pibIndices, 0 );
	pScene->pd3dDevice->SetVertexShader( dwVS );
	pScene->pd3dDevice->SetStreamSource( 0, pvbVertices, sizeof(SKINVERTEX) );

	pScene->pd3dDevice->SetTransform( D3DTS_WORLD, mtxTransform ); 

	for( i = 0 ; i < dwBonesCount ; i++ )
	{					
		mtx = ptabBones[i].mtxRest*ptabBones[i].pBoneKeyframer->mtxFrame;		
		pScene->pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i+1 ), mtx ); 
	}

	for( i = 0 ; i < dwSegmentsCount ; i++ )
	{					
		ptabSegments[i].pMaterial->ApplyMaterial();		
		pScene->pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, dwVerticesCount, ptabSegments[i].dwStartIndex, ptabSegments[i].dwFacesCount );
		ptabSegments[i].pMaterial->RestoreMaterial();
	}

	pScene->pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
	pScene->pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );
	pScene->pd3dDevice->SetStreamSource( 0, NULL, 0 );
	pScene->pd3dDevice->SetIndices( NULL, 0 );	

	if( pScene->dwFlags&FLAG_HW )
		pScene->pd3dDevice->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING, FALSE );
}
