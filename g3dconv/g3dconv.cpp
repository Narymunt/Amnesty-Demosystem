//=========================================================================================
// Converter utility from .3ds format to native helium3d format .scene
//=========================================================================================

#include "stdafx.h"
#include "resource.h"
#include "3dsScene.h"

#pragma pack(4)
typedef struct _SMFHEADER
{
	DWORD			dwID;
	DWORD			dwType;
	DWORD			dwVertexCount;
	DWORD			dwFacesCount;	
} SMFHEADER;
#pragma pack()

#define	SMF_ID		0x31464d53

int main(int argc, char* argv[])
{	

	SCENE			scene;
	int				err;
	OBJECT*			mesh;

	InitScene( &scene );	

	printf( "\n....loading %s file\n", argv[1] );
	if( ( err = Read3DS( argv[1], &scene ) ) != error_ok )
	{
		printf( "....error: 3ds read error #%d\n", err );
		return 2;
	}		

	if( !strnicmp( argv[2], "-extract", strlen( argv[2] ) ) )
	{
		mesh = FindObject( &scene, argv[3] );

		if( mesh == NULL || mesh->type != ID_MESH )
		{		
			printf( "....error: %s is not present in scene or is not a mesh\n", argv[3] );
			return 4;
		}

		TRIMESH*	outmesh = (TRIMESH*)mesh->obj;

		if( outmesh->group != 1 )
			printf( "....warning: %s multisegment mesh\n" );
		
		SMFHEADER	header;

		FILE*		outfile;
		CHAR		filename[MAX_PATH];

		if( argc >= 5 )
			strcpy( filename, argv[4] );
		else
			sprintf( filename, "%s.smf", argv[3] );
			
		outfile = fopen( filename, "wb" );

		if( outfile == NULL )
		{
			printf( "....error: unable to open dest file\n" );
			return 5;
		}

		header.dwID = SMF_ID;
		header.dwType = 0x0;
		header.dwVertexCount = outmesh->vertex;
		header.dwFacesCount = outmesh->face;

		fwrite( &header, sizeof(SMFHEADER), 1, outfile );
		fwrite( outmesh->vertices, sizeof(VERTICE), outmesh->vertex, outfile );
		
		for( DWORD i = 0 ; i < outmesh->face ; i++ )
		{		
			WORD			dwIndice;

			dwIndice = (WORD)outmesh->faces[i].a;
			fwrite( &dwIndice, sizeof(WORD), 1, outfile );
			dwIndice = (WORD)outmesh->faces[i].b;
			fwrite( &dwIndice, sizeof(WORD), 1, outfile );
			dwIndice = (WORD)outmesh->faces[i].c;
			fwrite( &dwIndice, sizeof(WORD), 1, outfile );			
		}

		fclose( outfile );

		return 0;
	}

	FLOAT scale = 1.0f;

	if( argc >= 4 )
		scale = (FLOAT)atof( argv[3] );
		

	printf( "\n....saving %s file\n", argv[2] );	
	if( ( err = WriteSCENE( argv[2], &scene, scale ) ) != error_ok )
	{
		printf( "....error: scene write error #%d\n", err );
		return 3;
	}	

	freeScene( &scene );

	return 0;
}
