
#include "SceneX.h"

LPD3DXMESH					g_pMesh				= NULL; // Our mesh object in sysmem
LPD3DXMESH					g_pMeshClone		= NULL; // Our mesh object in sysmem
D3DMATERIAL8*				g_pMeshMaterials	= NULL; // Materials for our mesh
LPDIRECT3DTEXTURE8*			g_pMeshTextures		= NULL; // Textures for our mesh
LPDIRECT3DTEXTURE8			g_pTexture			= NULL;
LPDIRECT3DTEXTURE8			g_pTexture1			= NULL;

DWORD                   g_dwNumMaterials	= 0L;   // Number of mesh materials
DWORD					VertexShader;		// vertex shader
FLOAT					fAlfa = 0.0f;

D3DMATERIAL8			g_Material;
D3DLIGHT8				g_Light;
D3DBLEND				srcBlend = D3DBLEND_ZERO;
D3DBLEND				dstBlend = D3DBLEND_ZERO;


D3DXMATRIX		matWorldX;
D3DXMATRIX		matWorldY;
D3DXMATRIX		matWorld;
D3DXMATRIX		matView;
D3DXMATRIX		matInvView;
D3DXMATRIX		matProj;
D3DXMATRIX		matViewProj;

// vertex shader for sphere mapping
	
DWORD dwDecl[] =
{
	D3DVSD_STREAM(0),
	D3DVSD_REG( D3DVSDE_POSITION, D3DVSDT_FLOAT3 ),
	D3DVSD_REG( D3DVSDE_NORMAL, D3DVSDT_FLOAT3 ),
	D3DVSD_REG( D3DVSDE_DIFFUSE, D3DVSDT_D3DCOLOR ),
	D3DVSD_REG( D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2 ),
	D3DVSD_REG( D3DVSDE_TEXCOORD1, D3DVSDT_FLOAT2 ),
	D3DVSD_END()
};

struct CUSTOMVERTEX
{
    FLOAT	x, y, z;      // The untransformed, 3D position for the vertex
	FLOAT	nx, ny, nz;
    DWORD	color;        // The vertex color
	FLOAT	tx;
	FLOAT	ty;
	FLOAT	tx1;
	FLOAT	ty1;
};

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneX::CSceneX()
{
	m_bActive = false;

	m_pBackground = NULL;
	m_pRamka = NULL;

}

CSceneX::~CSceneX()
{

	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground = NULL;
	}


	if (m_pRamka!=NULL)
	{
		delete m_pRamka;
		m_pRamka=NULL;
	}

	m_bActive = false;
}

//=== ustawianie aktywnosci - czy dane sa w pamieci

void CSceneX::SetActive(bool bState)
{
	m_bActive = bState;
}

//=== zwraca informacje czy dane sa wczytane

bool CSceneX::bGetActive(void)
{
	return m_bActive;
}

//=== aktualizuje i wczytuje dane

void CSceneX::Initialize(IDirect3DDevice8 *pDevice)
{

	m_pBackground = new CSprite(255,255,255,255);	// nowy sprite

//	m_pBackground->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
//		m_pFile->Search("back002.jpg"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
//		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pBackground->InitializeEx("data/back002.jpg",pDevice,
				0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);


	
	m_pRamka = new CSprite(255,255,255,255);	// nowy sprite

//	m_pRamka->InitializeTGAinMemoryEx((unsigned int*)m_pFile->pDataBuffer,
//		m_pFile->Search("ramka1.tga"), pDevice, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
//		D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_pRamka->InitializeEx("data/ramka1.tga",pDevice,
				0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, NULL, NULL);

	m_bActive = true;	// dane wczytane
	
	// ustawienia dla karty

    // Turn on the zbuffer

    pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    // Turn on ambient lighting 

//		pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	 //g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );

	// inicjalizacja geometri

    LPD3DXBUFFER pD3DXMtrlBuffer;

    // Load the mesh from the specified file
    if( FAILED( D3DXLoadMeshFromX( "torus.x", D3DXMESH_SYSTEMMEM, 
                                   pDevice, NULL, 
                                   &pD3DXMtrlBuffer, &g_dwNumMaterials, 
                                   &g_pMesh ) ) )
    {
//        return E_FAIL;
    }

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterials = new D3DMATERIAL8[g_dwNumMaterials];
    g_pMeshTextures  = new LPDIRECT3DTEXTURE8[g_dwNumMaterials];

    for( DWORD i=0; i<g_dwNumMaterials; i++ )
    {
        // Copy the material
        g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;
     
        // Create the texture
        if( FAILED( D3DXCreateTextureFromFile( pDevice, 
                                               d3dxMaterials[i].pTextureFilename, 
                                               &g_pMeshTextures[i] ) ) )
        {
            g_pMeshTextures[i] = NULL;
        }
    }

	if( FAILED( D3DXCreateTextureFromFile( pDevice, "earthenvmap.bmp", &g_pTexture ) ) )
	{
//		return E_FAIL;
	}
	if( FAILED( D3DXCreateTextureFromFile( pDevice, "nvlogo.tga", &g_pTexture1 ) ) )
	{
//		return E_FAIL;
	}
	
	g_pMesh->CloneMeshFVF( 0L, D3DFVF_CUSTOMVERTEX, pDevice, &g_pMeshClone );
	g_pMesh = g_pMeshClone;
//	D3DXComputeNormals( g_pMesh, NULL );

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

	g_Material.Ambient.a = 0xf0;
	g_Material.Ambient.r = 0x0f;
	g_Material.Ambient.g = 0xf0;
	g_Material.Ambient.b = 0x0f;
	g_Material.Diffuse.a = 0xf0;
	g_Material.Diffuse.r = 0x0f;
	g_Material.Diffuse.g = 0xf0;
	g_Material.Diffuse.b = 0x0f;

	//D3DXMatrixTranslation( &matView, 0.0f, 10.0f, 300.0f );


	// tworzymy vertex shader

	LPD3DXBUFFER	pCode;
	LPD3DXBUFFER	pError;
	HRESULT			hError;

	hError = D3DXAssembleShaderFromFile( "sphere.vsh", 0, NULL, &pCode, &pError );
	// plik << (char*)( pError->GetBufferPointer() ) <<endl;
	pDevice->CreateVertexShader( dwDecl, (DWORD*)pCode->GetBufferPointer(), &VertexShader, 0 );
	pCode->Release();



	// koniec
	

}

//=== deaktualizuje, ale trzyma znaczniki (zwalnia tylko textury)

void CSceneX::DeInitialize(void)
{

	// zwolnij tlo jezeli uzywane
	
	if (m_pBackground!=NULL)
	{
		delete m_pBackground;
		m_pBackground=NULL;
	}

	// obiekt

    if( g_pMeshMaterials != NULL ) 
        delete[] g_pMeshMaterials;

    if( g_pMeshTextures )
    {
        for( DWORD i = 0; i < g_dwNumMaterials; i++ )
        {
            if( g_pMeshTextures[i] )
                g_pMeshTextures[i]->Release();
        }
        delete[] g_pMeshTextures;
    }
    if( g_pMesh != NULL )
        g_pMesh->Release();

	// ramka

	if (m_pRamka!=NULL)
	{
		delete m_pRamka;
		m_pRamka=NULL;
	}

}

//=== rysuje pojedyncza klatke

int CSceneX::DrawScene(IDirect3DDevice8 *pDevice, long lTimer)
{
	

    // Clear the backbuffer and the zbuffer
    pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
                         D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	m_pBackground->Render();	// rysuj tlo

	// teraz nasz obiekt

	fAlfa += 0.1f;
	if( fAlfa >= 360.0f )
		fAlfa = 0.0f;

	// world transform
	D3DXMatrixRotationY( &matWorldX, timeGetTime()/1500.0f );
	D3DXMatrixRotationZ( &matWorldY, timeGetTime()/1500.0f );
	
	// view transform
    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 70.0f,700.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );

	// projection transform
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 700.0f );

	// calculations for sphere mapping
	D3DXMatrixMultiply( &matWorld, &matWorldX, &matWorldY );
	D3DXMatrixMultiply( &matViewProj, &matView, &matProj );
	D3DXMatrixInverse( &matInvView, NULL, &matView );
	D3DXVECTOR4		vectPos( matInvView._41, matInvView._42, matInvView._43, 1.0 );
	D3DXVECTOR4		vect( 0.0f, 0.5f, 1.0f, 2.0 );

	pDevice->SetVertexShaderConstant( 0, D3DXMatrixTranspose( &matWorld, &matWorld ), 4 );
	pDevice->SetVertexShaderConstant( 4, D3DXMatrixTranspose( &matViewProj, &matViewProj ), 4 );
	pDevice->SetVertexShaderConstant( 8, &vectPos, 1 );

	pDevice->SetVertexShaderConstant( 9, &vect, 1 );

	// teraz renderowanie

	// Meshes are divided into subsets, one for each material. Render them in a loop
    for( DWORD i=0; i < g_dwNumMaterials; i++ )
    {
		pDevice->SetMaterial( &g_Material );
		pDevice->SetTexture( 0, g_pTexture );
		pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		pDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
		pDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

		pDevice->SetTexture( 1, g_pTexture1 );
		pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA );
		pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		pDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
		pDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

		pDevice->SetVertexShader( VertexShader );

		LPDIRECT3DVERTEXBUFFER8 pVB;
		LPDIRECT3DINDEXBUFFER8 pIB;
		
		g_pMesh->GetVertexBuffer( &pVB );
		g_pMesh->GetIndexBuffer( &pIB );
		pDevice->SetStreamSource( 0, pVB, sizeof( CUSTOMVERTEX ) );
		pDevice->SetIndices( pIB, 0 );
		pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, g_pMesh->GetNumVertices(),
                    0, g_pMesh->GetNumFaces() );

		g_pMesh->DrawSubset( i );
    }

	m_pRamka->Render();

	return 0;

}
//end
