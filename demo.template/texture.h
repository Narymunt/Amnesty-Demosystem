
#include <d3dx8.h>

enum TEXTUREERRORCODES
{
	TEXERR_NOTINITIALIZED,
	TEXERR_NOFILEPRESENT,
	TEXERR_FILEFORMATERR,
	TEXERR_UNABLETOLOAD
};

class CTextureException
{
public:

	DWORD			code;

public:

	CTextureException( DWORD _code ) : code( _code )
	{
	}
};

enum AUTOALPHASTATE
{
	AAS_ALPHALOADED,
	AAS_ALPHANOTLOADED
};

enum MIPMAPS
{
	MIP_NONE	= 1,
	MIP_ALL		= 0,
	MIP_ONE		= 2,
	MIP_TWO		= 3,
	MIP_TREE	= 4
};

void InitializeTextureSystem( PDIRECT3DDEVICE8 pdevice, DWORD format = D3DFMT_A8R8G8B8 );
void CloseTextureSystem();
int LoadTextureAutoAlpha( const char* filename, DWORD dwMip = MIP_NONE );
void LoadTexture( const char* filename, DWORD dwMip = MIP_NONE );
void LoadAlphaToTexture( const char* filename, const char* texturename );
void LoadTextureWithAlpha( const char* texfilename, const char* alpfilename, DWORD dwMip = MIP_NONE );
void LoadAlphaTexture( const char* filename );
void DeleteTexture( const char* name );
PDIRECT3DTEXTURE8 GetTexture( const char* name );
void SetTexturePath( const char* path );


