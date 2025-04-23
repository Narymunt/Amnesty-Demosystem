// 3dsScene.h: interface for the C3dsScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DSSCENE_H__CE8EC281_155A_11D5_AD0F_C04A57C10000__INCLUDED_)
#define AFX_3DSSCENE_H__CE8EC281_155A_11D5_AD0F_C04A57C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAIN_CHUNK		0x4D4D

#define VERSION_CHUNK  		0x0002

#define EDITOR_CHUNK  		0x3D3D

#define KEYFRAMER_CHUNK		0xB000

#define OBJECT_CHUNK    		0x4000
 #define TRIMESH_CHUNK      	0x4100
  #define VERTICES_CHUNK        	0x4110
  #define FACES_CHUNK        	0x4120
  #define FACES_MATERIAL_CHUNK      0x4130
  #define MAPPING_COORDS_CHUNK      0x4140
  #define LOCAL_COORDS_CHUNK        0x4160
 #define LIGHT_CHUNK      		0x4600
  #define SPOTLIGHT_CHUNK        	0x4610
 #define CAMERA_CHUNK      	0x4700
 #define MATERIAL_CHUNK 		0xAFFF
  #define MATERIAL_NAME_CHUNK      	0xA000
  #define MAP_FILENAME_CHUNK        0xA300
  #define TEXTURE_MAP1_CHUNK      	0xA200
  #define TEXTURE_MAP2_CHUNK      	0xA33A
  #define BUMP_MAP_CHUNK      	0xA230
  #define SPECULAR_MAP_CHUNK      	0xA204

#define AMBIENT_TRACK_CHUNK    	0xB001
#define MESH_TRACK_CHUNK    	0xB002
#define CAMERA_TRACK_CHUNK    	0xB003
#define CAMERA_TARGET_TRACK_CHUNK   0xB004
#define OMNILIGHT_TRACK_CHUNK    	0xB005
#define SPOTLIGHT_TRG_TRACK_CHUNK	0xB006
#define SPOTLIGHT_TRACK_CHUNK    	0xB007
#define FRAMES_CHUNK    		0xB008
#define OBJECT_INFO_CHUNK      	0xB010
#define DUMMYNAME_CHUNK			0xB011
#define OBJECT_PIVOT_CHUNK      	0xB013
#define MORF_ANGEL_TRACK_CHUNK      0xB015
#define POSITION_TRACK_CHUNK      	0xB020
#define ROTATION_TRACK_CHUNK      	0xB021
#define SCALE_TRACK_CHUNK      	0xB022
#define FOV_TRACK_CHUNK      	0xB023
#define ROLL_TRACK_CHUNK      	0xB024
#define COLOR_TRACK_CHUNK      	0xB025
#define MORPH_TRACK_CHUNK      	0xB026
#define HOTSPOT_TRACK_CHUNK      	0xB027
#define FALLOFF_TRACK_CHUNK      	0xB028
#define HIDE_TRACK_CHUNK      	0xB029
#define HIERARCHY_CHUNK      	0xB030


#define EPSILON			0.001
#define sqr(x)			((x)*(x))

#ifndef M_PI
 #define M_PI			3.1415
#endif

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
	ID_NULLKEYFRAMER		= 0xee07
};

enum KEYFRAMERFLAGS
{
	KFF_SUB			= 1,
	KFF_NEXT		= 2,
	KFF_EULERROT	= 4,
	KFF_MORPH		= 8,
	KFF_HIDE		= 16,
	KFF_FOV			= 32,
	KFF_ROLL		= 64,
};

enum errors
{
	error_ok=0,
	error_file_open,
	error_bad_file,
	error_invalid_track,
	error_invalid_frame,
	error_memory,
	error_invalid_hierarchy,
	error_invalid_object,
	error_invalid_scene,
	error_no_map
};

/*****************************************************************************
 struktury opisujace obiekty sceny
*****************************************************************************/

/*
 *  material pokrywajacy face'y
 */
typedef struct _MATERIAL
{
	char		*name;
	DWORD		type;

	char*		texture;		
	char*		envmap;

} MATERIAL;

/*
 *  wierzcholek
 */
typedef struct _VERTICE
{
	FLOAT		x, y, z;
	FLOAT		nx, ny,	nz;
		
	FLOAT		u, v;			//wspolrzedne zaczpienia tekstury
} VERTICE;


/* face - trojkaty z ktorych zbudowane sa obiekty */
typedef struct _FACE
{
	DWORD   	a, b, c;		//wierzcholki skladajace sie na face'a
	DWORD		group;
	MATERIAL*	mat;			//material face'a	
} FACE;


/*
 *  obiekt (triangular mesh) skadajacy sie z trojkatow
 */
typedef struct _TRIMESH
{
	DWORD		face, vertex; //liczba wierzcholkow i scian

	DWORD		morph;
	DWORD		hide;	

	DWORD		group;

	FLOAT		cx, cy, cz;

	FACE*		faces;       //tablica facow
	VERTICE*	vertices;    //tablica wiezrcholkow	

} TRIMESH;

/*
 *  kamera, czyli obiekt ktory opisuje pozycje obserwatora i kierunek
 *  patrzenia
 */
typedef struct _CAMERA
{
	FLOAT		x, y, z;     //wektor polozenia
	
	FLOAT		FOV;	     //FOV - field of view - rozpietosc ostroslupa widoku
	FLOAT		roll;	     //kat "przechylu"

} CAMERA;


/*
 *  swiato, obiekt reprezentujacy zrodo oswietlenia
 */
typedef struct _LIGHT
{
	FLOAT		x, y, z;

} LIGHT;


typedef struct _TARGET
{
	FLOAT		x, y, z;
	char*		name;

} TARGET;

/*
 *  ogolna struktura reprezentujaca dowolny obiekt sceny
 */
typedef struct _OBJECT
{
	DWORD		type;	     //typ obiektu
	char* 		name; 	     //nazwa

	void*		obj;	     //wskaznik na wlasciwy obiekt
	_OBJECT*	next;	     //wskaznik na nastepny (do twozenia listy)

} OBJECT;

/*
 *  pomocnicza unia wykozystywana przez struktory sciezek
 */
typedef struct _INFO
{
	FLOAT		x, y, z;
	FLOAT		val;
	char*		name;

} INFO;

/*
 *  struktura, ktora reprezentuje key'a czyli pojedynczy element sciezki
 *  po ktorej porusza sie obiekt
 */
typedef struct _KEY
{
	FLOAT 		frame; 	     //nr klatki w ktorej lezy key

	INFO		data;	     //odpowienia wspolzedna zapisana w keyu
							  //moze to byc wektor, kwaternion lub liczba
	FLOAT		easefrom;     //wspolczynniki
	FLOAT		easeto;

	FLOAT		T, B, C;

	_KEY*		next;				//wskaznik na nastepny key

} KEY;


/*
 *  sciezka obiektu (triangular mesh)
 */
typedef struct _MESHTRACK
{
	//	word	loop;
	DWORD		dwRotCount;
	DWORD		dwPosCount;
	DWORD		dwSclCount;

	DWORD		dwHideCount;
	DWORD		dwMorphCount;


	KEY*		rotation;    //obrot
	KEY*		position;    //pozycja
	KEY*		scale;       //skala

	KEY*		morph;	     //morfing
	KEY*		hide;	     //ukrywanie     

	FLOAT		px, py, pz;

} MESHTRACK;

/*
 *  sciezka swiatla
 */
typedef struct _LIGHTTRACK
{
	DWORD		dwPosCount;
	KEY*		position;		//w tej wersji biblioteki zaimplementowany
								//jest tylko track pozycji dla swiatla
} LIGHTTRACK;                   //mozliwe sa jescze sciezki koloru

/*
 *  sciezka celu kamery
 */
typedef struct _TARGETTRACK
{
	DWORD		dwPosCount;	

	KEY*		position;   //sciezka pozycja

} TARGETTRACK;


/*
 *  track kamery
 */
typedef struct _CAMERATRACK
{
	DWORD		dwPosCount;
	DWORD		dwRollCount;
	DWORD		dwFovCount;

	KEY*		position;  //pozycja i roll, nie jest zimplementowana
	KEY*		roll;		//sciezka ostoslupa widzenia (FOV)
	KEY*		fov;		//sciezka ostoslupa widzenia (FOV)

} CAMERATRACK;


/*
 *  ogolna struktura reprezentujaca pojedynczya sciezke dowolnego typu obiektu
 */
typedef struct _KEYFRAMER
{
	DWORD			type;	  //typ sciezki
	DWORD			flags;

	_KEYFRAMER*		parent; //wskaznik na rodzica, brata i potomka
	_KEYFRAMER*		next;   //twoza hierarchie sceny

	_KEYFRAMER*		child;
	_KEYFRAMER*		brother;

	void*			track;  //wskaznik na wlasciwa sciezke

	char*			objectname; //nazwa obiektu dla ktorego jest sciezka
	void*			object;     //i wskaznik na niego	

	WORD			hierarchy;   //identyfikator wskazujacy na pozycja w hierarchi
	WORD			link;        //identyfikator rodzica

} KEYFRAMER;

/*
 *  struktura reprezentujaca scene
 */
typedef struct _SCENE
{
	DWORD			objCount;
	DWORD			matCount;
	DWORD			keyCount;

	OBJECT*			objects;		//lista obiektow
	OBJECT*			materials;
	KEYFRAMER*		tracks;			//lista sciezek
	DWORD 			start, end;		//klatka poczatkowa i koncowa

	KEYFRAMER*		root;

	DWORD			version;		//wersja pliku 3ds z ktorego zostala odczytana scena
} SCENE;

int  Read3DS( char name[], SCENE *scene );
int  WriteSCENE( char name[], SCENE *scene, FLOAT scale = 1.0f );
void InitScene( SCENE *s );
void freeScene( SCENE *s );

OBJECT* FindFirst( SCENE* s, DWORD type );
OBJECT* FindNext();
OBJECT *FindObject(SCENE *s, char *name, DWORD type );
OBJECT *FindObject(SCENE *s, char *name );

#endif // !defined(AFX_3DSSCENE_H__CE8EC281_155A_11D5_AD0F_C04A57C10000__INCLUDED_)
