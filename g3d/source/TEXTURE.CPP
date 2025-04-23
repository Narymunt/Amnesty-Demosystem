#include "texture.h"
#include <stdio.h>
#include "ijl.h"
#include "namemap.h"

class RAWIMAGE
{
public:

	LPDWORD			pPixels;
	DWORD			width;
	DWORD			height;

public:

	RAWIMAGE() : pPixels(NULL)
	{		
	}

	~RAWIMAGE()
	{
		if( pPixels )
			delete pPixels;
	}	
};

enum TGAIMAGETYPE
{
	TGA_COLLORMAPPED = 1,
	TGA_FULLCOLOR,
	TGA_GRAYSCALE,
	TGA_COMPRESSEDCOLORMAPPED = 9,
	TGA_COMPRESSEDFULLCOLOR,
	TGA_COMPRESSEDGRAYSCALE
};


#pragma pack(1)
typedef struct _tag_TGAHEADER
{
    BYTE	IDLength;		/* length of Identifier String */
    BYTE	ColorMapType;	/* 0 = no map */
    BYTE	ImgType;		/* image type (see below for values) */
    WORD    ColorMapIndex;	/* index of first color map entry */
    WORD	ColorMapLenght;	/* number of entries in color map */
    BYTE	ColorSize;		/* size of color map entry (15,16,24,32) */
    WORD	xorg;			/* x origin of image */
    WORD	yorg;			/* y origin of image */
    WORD    width;			/* width of image */
    WORD	height;			/* height of image */
    BYTE	bpp;			/* pixel size (8,16,24,32) */
	BYTE	attribs;    
} TGAHEADER;
#pragma pack()

#pragma pack(1)						
typedef struct _tag_BMPHEADER
{

	WORD			ID;			/* identyfikator BM */
	DWORD			filesize;	/* rozmiar pliku */
	DWORD			reserved;
	DWORD			pixeloffset;/* poczatek danych bitmapy */

	DWORD			bmisize;	/* rozmiar infa musi byc 40 */
	DWORD			width;		/* szerokosc */
	DWORD			height;		/* wysokosc */
	WORD			planes;		/* liczba plaszczyzn bitowych */
	WORD			bpp;		/* liczba bitow na pixel */
	DWORD			compression;/* typ kompresji */
	DWORD			cmpsize;	/* rozmiar skompresowanego pliku */
	DWORD			xscale;		/* pixle na metr, nieuzywane */
	DWORD			yscale;		/* jak wyzej */
	DWORD			colors;		/* uzyte kolory, nieuzywane */
	DWORD			impcolors;	/* wazne kolor, nieuzywane */

} BMPHEADER;
#pragma pack()

#define BMP_ID			0x4D42

RAWIMAGE* LoadBMP( const char* name )
{

	FILE*				inFile;
	BMPHEADER			header;
	DWORD				linelength;
	LPBYTE				line;

	DWORD				palette[256];
	DWORD				colornbr;
	LONG				i, j, k;
	LPDWORD				buffer;
	BYTE				tmp, r, g, b;
	DWORD				pos;

	RAWIMAGE*			image = NULL;

	// file open
	inFile = fopen( name, "rb" );
	if( inFile == NULL )
		return NULL;

	// read header
	fread( &header, sizeof(header), 1, inFile );

	// id check
	if( header.ID != BMP_ID )
	{
		fclose( inFile );
		return NULL;
	}

	// skip compressed bitmaps
	if( header.compression || header.planes>1 )
	{
		fclose( inFile );
		return NULL;
	}

	image = new RAWIMAGE;
	
	image->width	= header.width;
	image->height	= header.height;

	linelength = ( (header.width*header.bpp >> 3) + 3 ) & 0xFFFFFFFC;

	image->pPixels = new DWORD[header.width*header.height];
		
	// tmp line holder
	line = new BYTE[linelength];

	// palete read
	if( header.bpp != 24 )
	{
		// calculate palette entry number
		colornbr = (1LU<<header.bpp);
		// palete read
		for( i = 0 ; i < (LONG)colornbr ; i++ )
		{
			fread( &palette[i], sizeof(DWORD), 1, inFile );
		}
	}

	fseek( inFile, header.pixeloffset, SEEK_SET );

	
	for( i = header.height-1 ; i>=0 ; i-- )		
		if( fread( line, 1, linelength, inFile ) != linelength )
		{	
			fclose( inFile );
			return NULL;
		}
		else
		{			
			buffer = image->pPixels + header.width*i;
			for( j = 0, pos = 0 ; j < (LONG)header.width ; )
			{
				switch( header.bpp )
				{
					case 1:
					{						
						tmp = line[pos++];						
						for( k = 7 ; k>=0 ; k--, j++)
						{
							r = (tmp>>k)&0x01;								
							buffer[j] = palette[r];
						}
					}
					break;
					
					case 4:
					{						
						tmp = line[pos++];						
						buffer[j++] = palette[tmp>>4];						
						buffer[j++] = palette[tmp&0xF];
					}
					break;

					case 8:
					{						
						tmp = line[pos++];
						buffer[j++] = palette[tmp];
					}
					break;

					case 24:
					{							
						b = line[pos++];
						g = line[pos++];
						r = line[pos++];						
						buffer[j++] = D3DCOLOR_ARGB( 0, r, g, b );
					}
					break;
					
				}
			}
		}

	delete line;
	fclose( inFile );

	return image;
}

RAWIMAGE* Load8bitBMPFile( const char* name )
{
	FILE*				inFile;
	BMPHEADER			header;
	DWORD				linelength;
	LPBYTE				line;
	
	LONG				i;
	LPBYTE				buffer, current;	

	RAWIMAGE*			image = NULL;

	// file open
	inFile = fopen( name, "rb" );

	if( inFile == NULL )
		return NULL;

	// read header
	fread( &header, sizeof(header), 1, inFile );

	// id check
	if( header.ID != BMP_ID )
	{
		fclose( inFile );
		return NULL;
	}

	// skip compressed bitmaps
	if( header.compression || header.planes>1 )
	{
		fclose( inFile );
		return NULL;
	}

	if( header.bpp != 8 )
	{
		fclose( inFile );
		return NULL;
	}	


	image = new RAWIMAGE;

	image->width	= header.width;
	image->height	= header.height;

	linelength = ( (header.width*header.bpp >> 3) + 3 ) & 0xFFFFFFFC;

	buffer = new BYTE[header.width*header.height];
	image->pPixels = (LPDWORD)buffer;
		
	// tmp line holder
	line = new BYTE[linelength];
	
	fseek( inFile, header.pixeloffset, SEEK_SET );
	
	for( i = header.height-1 ; i>=0 ; i-- )		
		if( fread( line, 1, linelength, inFile ) != linelength )
		{	
			fclose( inFile );
			delete image;
			return NULL;
		}
		else
		{			
			current = buffer + i*image->width;
			for( DWORD j = 0 ; j < image->width ; j++ )
				current[j] = line[j];
		}

	fclose( inFile );			
	delete line;	

	return image;
}

//----------------------------------------------------------------------------
//name: LoadJPG()
//desc: loades a .jpg file, decompresion rutine comes from IJP (IndependedJPEG
//		Group)
//----------------------------------------------------------------------------
RAWIMAGE* LoadJPG( const char* name )
{			
	JPEG_CORE_PROPERTIES		jcp;
	DWORD						color;
	LPBYTE						buffer;

	RAWIMAGE*					image = NULL;

	ZeroMemory( &jcp, sizeof(JPEG_CORE_PROPERTIES) );

	if( ijlInit( &jcp ) != IJL_OK )
		return NULL;

	jcp.JPGFile = name;
	
	if( ijlRead( &jcp, IJL_JFILE_READPARAMS ) != IJL_OK )
		return NULL;

	if( jcp.JPGChannels != 3 )
		return NULL;


	image			= new RAWIMAGE;
	image->width	= jcp.JPGWidth;
	image->height	= jcp.JPGHeight;
	image->pPixels	= new DWORD[image->width*image->height];

	buffer = new BYTE[image->width*image->height*3];

	jcp.DIBWidth	= image->width;
	jcp.DIBHeight	= image->height;
	jcp.DIBChannels	= 3;
	jcp.DIBColor	= IJL_RGB;
	jcp.DIBPadBytes	= 0;
	jcp.DIBBytes	= buffer;

	jcp.JPGColor	= IJL_YCBCR;

	if( ijlRead( &jcp, IJL_JFILE_READWHOLEIMAGE ) != IJL_OK )
	{		
		delete image;
		delete buffer;
		return NULL;
	}

	ijlFree( &jcp );

	for( DWORD i = 0 ; i < image->width*image->height ; i++ )
	{	
		color = ( buffer[i*3] << 16 ) | ( buffer[i*3+1] << 8 ) | buffer[i*3+2];
		image->pPixels[i] = color;
	}

	delete buffer;

	return image;
}

//----------------------------------------------------------------------------
//name: LoadTGAfile()
//desc: only fullcolor file are supported
//----------------------------------------------------------------------------
RAWIMAGE* LoadTGA( const char* name )
{
	TGAHEADER			header;	
	FILE*				imgFile = fopen( name, "rb" );
	LONG				i, j;			
	WORD				p1;
	DWORD				p2;	

	RAWIMAGE*			image = NULL;

	if(	!imgFile )		
		return NULL;

	fread( &header, sizeof(TGAHEADER), 1, imgFile );
	
	if( header.ImgType!=TGA_FULLCOLOR )
	{
		fclose( imgFile );
		return NULL;
	}

	image = new RAWIMAGE;

	image->width	= header.width;
	image->height	= header.height;
	image->pPixels	= new DWORD[header.width*header.height];
	
	switch( header.bpp )
	{
		case 15:
		case 16:							
			for( i = (LONG)header.height-1 ; i>=0 ; i--)
				for( j = 0 ; j<(LONG)header.width ; j++ )
				{
					fread( &p1, sizeof(WORD), 1, imgFile );										
					image->pPixels[i*header.width+j] = D3DCOLOR_ARGB( 0, ( ( p1>>10 )&0x1F )<<3, ( ( p1>>5 )&0x1F )<<3, ( p1&0x1F )<<3 );
				}			
		break;

		case 32:
			for( i = (LONG)header.height-1 ; i>=0 ; i-- )
				for( j = 0 ; j<(LONG)header.width ; j++ )
				{
					fread( &p2, sizeof(DWORD), 1, imgFile );					
					image->pPixels[i*header.width+j] = p2;
				}			
		break;

		default:
			fclose( imgFile );
			return NULL;
		break;
	}

	fclose( imgFile );

	return image;
}

void MergeAlphaAndColor( RAWIMAGE* riBitmap, RAWIMAGE* riAlpha )
{
	if( riBitmap->width != riAlpha->width || riBitmap->height != riAlpha->height )
	{
		LPBYTE			buffer	= new BYTE[riBitmap->width*riBitmap->height];
		LPBYTE			alpha	= (LPBYTE)riAlpha->pPixels;
		FLOAT			dx		= (FLOAT)riAlpha->width/(FLOAT)riBitmap->width;
		FLOAT			dy		= (FLOAT)riAlpha->height/(FLOAT)riBitmap->height;

		FLOAT			x, y;
		DWORD			i, j;

		for( i = 0, y = 0 ; i < riBitmap->height ; i++, y += dy )
			for( j = 0, x = 0 ; j < riBitmap->width ; j++, x+= dx )			
				buffer[i*riBitmap->width+j] = alpha[(LONG)(y*riAlpha->width+x)];

		delete riAlpha->pPixels;

		riAlpha->pPixels	= (LPDWORD)buffer;
		riAlpha->width		= riBitmap->width;
		riAlpha->height		= riBitmap->height;
	}

	LPBYTE			alpha	= (LPBYTE)riAlpha->pPixels;
	LPDWORD			dest	= riBitmap->pPixels;

	for( DWORD i = 0 ; i < riBitmap->width*riBitmap->height ; i++ )
		dest[i] = ( dest[i]&0xffffff ) | ( alpha[i]<<24 );
}

void ExtendAlphaTo32Bpp( RAWIMAGE* riAlpha )
{
	LPDWORD				buffer	= new DWORD[riAlpha->width*riAlpha->height];
	LPBYTE				alpha	= (LPBYTE)riAlpha->pPixels;

	for( DWORD i = 0 ; i < riAlpha->width*riAlpha->height ; i++ )
		buffer[i] = ( ( alpha[i]&0xff ) <<24 ) | 0xffffff;

	delete riAlpha->pPixels;

	riAlpha->pPixels = buffer;
}

char* GetExtension( const char* str )
{	
	DWORD				length = strlen( str );

	for( int i =  length ; i >= 0 ; i-- )
	{
		if( str[i] == '.' )
			return (char*)( &(str[i+1]) );

		if( str[i] == '//' )
			return (char*)( &(str[length]) );
	}

	return (char*)( &(str[length]) );
}

RAWIMAGE* LoadImage( const char* name )
{
	char*			ext = GetExtension( name );

	if( !_stricmp( ext, "bmp" ) )	
		return LoadBMP( name );
	
	if( !_stricmp( ext, "jpg" ) )
		return LoadJPG( name );

	if( !_stricmp( ext, "tga" ) )
		return LoadTGA( name );

	return NULL;
}

class CTexture
{
public:
	
	PDIRECT3DTEXTURE8		text;	

	CTexture() : text( NULL )
	{
	}

	~CTexture()
	{
		if( text )
			text->Release();		
	}
};

typedef	CNameMap<CTexture>	CTextureList;

static	DWORD				dwFormat	= D3DFMT_A8R8G8B8;
static	DWORD				dwInit		= 0;

static	PDIRECT3DDEVICE8	pDev		= NULL;

static	CTextureList*		pTextList	= NULL;

static	char				strPath[MAX_PATH] = "";

void InitializeTextureSystem( PDIRECT3DDEVICE8 pdevice, DWORD format )
{
	dwInit		= 1;

	dwFormat	= format;
	pDev		= pdevice;

	pTextList	= new CTextureList;
}

void CloseTextureSystem()
{
	dwInit		= 0;
	delete pTextList;	
};

int LoadTextureAutoAlpha( const char* filename, DWORD dwMip )
{
	if( !filename )
		throw CTextureException( TEXERR_UNABLETOLOAD );

	char*				ext		= GetExtension( filename );
	char				alpha[MAX_PATH];
	char				path[MAX_PATH];

	strnset( alpha, 0, strlen( filename ) + strlen( "_alpha" ) + 1 );
	strncpy( alpha, filename, strlen( filename ) - strlen( ext ) - 1);
	strcat( alpha, "_alpha.bmp" );	

	FILE*				tmpFile = fopen( alpha, "rb" );

	if( tmpFile )
	{
		fclose( tmpFile );
		LoadTextureWithAlpha( filename, alpha, dwMip );		

		return AAS_ALPHALOADED;
	}
	else
	{	
		strcpy( path, strPath );
		strcat( path, alpha );

		tmpFile = fopen( path, "rb" );

		if( tmpFile )
		{
			fclose( tmpFile );
			LoadTextureWithAlpha( filename, alpha, dwMip );		

			return AAS_ALPHALOADED;
		}

		LoadTexture( filename, dwMip );		

		return AAS_ALPHANOTLOADED;
	}	
}

void LoadTexture( const char* filename, DWORD dwMip )
{
	if( !filename )
		throw CTextureException( TEXERR_UNABLETOLOAD );

	if( GetTexture( filename ) != NULL )
		return;

	DWORD					width;
	DWORD					height;
	PDIRECT3DSURFACE8		surface;
	RECT					rect;
	char					path[MAX_PATH];

	if( !dwInit )
		throw CTextureException( TEXERR_NOTINITIALIZED );

	RAWIMAGE*			image = LoadImage( filename );

	if( !image )
	{	
		strcpy( path, strPath );
		strcat( path, filename );
		image = LoadImage( path );
	}

	if( !image )	
		throw CTextureException( TEXERR_UNABLETOLOAD );


	CTexture*			texture = new CTexture;	

	pTextList->Add( filename, texture );

	for( width = 1 ; width < image->width ; width <<= 1 );
	for( height = 1 ; height < image->height ; height <<= 1 );

	if( FAILED( pDev->CreateTexture( width, height, dwMip, 0, (D3DFORMAT)dwFormat, D3DPOOL_MANAGED, &texture->text ) ) )
	{
		delete image;
		
		throw CTextureException( TEXERR_UNABLETOLOAD );
	}

	rect.left	= 0;
	rect.top	= 0;
	rect.right	= image->width;
	rect.bottom	= image->height;
	
	for( DWORD i = 0 ; i < texture->text->GetLevelCount() ; i++ )
	{
		if( FAILED( texture->text->GetSurfaceLevel( i, &surface ) ) )
		{
			texture->text->Release();
			texture->text = NULL;
			
			delete image;

			throw CTextureException( TEXERR_UNABLETOLOAD );
		}

		if( FAILED( D3DXLoadSurfaceFromMemory( surface, NULL, NULL, image->pPixels, D3DFMT_A8R8G8B8, image->width*sizeof(DWORD), NULL, &rect, D3DX_FILTER_LINEAR|D3DX_FILTER_DITHER, 0 ) ) )
		{
			surface->Release();

			texture->text->Release();
			texture->text = NULL;

			delete image;

			throw CTextureException( TEXERR_UNABLETOLOAD );
		}

		surface->Release();
	}

	delete image;
}

void LoadAlphaToTexture( const char* filename, const char* texturename )
{
	throw CTextureException( TEXERR_UNABLETOLOAD );
}

void LoadTextureWithAlpha( const char* texfilename, const char* alpfilename, DWORD dwMip )
{
	if( !texfilename || !alpfilename )
		throw CTextureException( TEXERR_UNABLETOLOAD );

	if( GetTexture( texfilename ) != NULL )
		return;

	DWORD					width;
	DWORD					height;
	PDIRECT3DSURFACE8		surface;
	RECT					rect;
	char					path[MAX_PATH];

	if( dwFormat != D3DFMT_A8R8G8B8 && dwFormat != D3DFMT_A4R4G4B4 && dwFormat != D3DFMT_A8R3G3B2 )
		throw CTextureException( TEXERR_UNABLETOLOAD );

	RAWIMAGE*			image	= LoadImage( texfilename );
	RAWIMAGE*			alpha	= Load8bitBMPFile( alpfilename );

	if( !image )
	{
		strcpy( path, strPath );
		strcat( path, texfilename );
		image = LoadImage( path );
	}

	if( !alpha )
	{
		strcpy( path, strPath );
		strcat( path, alpfilename );
		alpha	= Load8bitBMPFile( path );
	}

	if( !image || !alpha )	
		throw CTextureException( TEXERR_UNABLETOLOAD );

	MergeAlphaAndColor( image, alpha );	
	
	CTexture*			texture = new CTexture;

	pTextList->Add( texfilename, texture );

	for( width = 1 ; width < image->width ; width <<= 1 );
	for( height = 1 ; height < image->height ; height <<= 1 );

	if( FAILED( pDev->CreateTexture( width, height, dwMip, 0, (D3DFORMAT)dwFormat, D3DPOOL_MANAGED, &texture->text ) ) )
	{
		delete image;
		delete alpha;

		throw CTextureException( TEXERR_UNABLETOLOAD );
	}
	
	rect.left	= 0;
	rect.top	= 0;
	rect.right	= image->width;
	rect.bottom	= image->height;
	
	for( DWORD i = 0 ; i < texture->text->GetLevelCount() ; i++ )
	{
		if( FAILED( texture->text->GetSurfaceLevel( i, &surface ) ) )
		{
			texture->text->Release();
			texture->text = NULL;

			delete image;
			delete alpha;
			
			throw CTextureException( TEXERR_UNABLETOLOAD );
		}
		
		if( FAILED( D3DXLoadSurfaceFromMemory( surface, NULL, NULL, image->pPixels, D3DFMT_A8R8G8B8, image->width*sizeof(DWORD), NULL, &rect, D3DX_FILTER_LINEAR|D3DX_FILTER_DITHER, 0 ) ) )
		{
			surface->Release();
			
			texture->text->Release();
			texture->text = NULL;

			delete image;
			delete alpha;
			
			throw CTextureException( TEXERR_UNABLETOLOAD );
		}
		
		surface->Release();
	}
	
	delete alpha;
	delete image;
}

void LoadAlphaTexture( const char* filename )
{
	if( !filename )
		throw CTextureException( TEXERR_UNABLETOLOAD );

	if( GetTexture( filename ) != NULL )
		return;

	DWORD					width;
	DWORD					height;
	PDIRECT3DSURFACE8		surface;
	RECT					rect;
	char					path[MAX_PATH];

	RAWIMAGE*				image = Load8bitBMPFile( filename );

	if( !image )
	{	
		strcpy( path, strPath );
		strcat( path, filename );
		image = Load8bitBMPFile( path );
	}

	if( !image )		
		throw CTextureException( TEXERR_UNABLETOLOAD );

	ExtendAlphaTo32Bpp( image );

	CTexture*			texture = new CTexture;

	pTextList->Add( filename, texture );

	for( width = 1 ; width < image->width ; width <<= 1 );
	for( height = 1 ; height < image->height ; height <<= 1 );
	
	if( FAILED( pDev->CreateTexture( width, height, 1, 0, (D3DFORMAT)dwFormat, D3DPOOL_MANAGED, &texture->text ) ) )
	{	
		delete image;

		throw CTextureException( TEXERR_UNABLETOLOAD );	
	}

	rect.left	= 0;
	rect.top	= 0;
	rect.right	= image->width;
	rect.bottom	= image->height;

	for( DWORD i = 0 ; i < texture->text->GetLevelCount() ; i++ )
	{
		if( FAILED( texture->text->GetSurfaceLevel( i, &surface ) ) )
		{
			texture->text->Release();
			texture->text = NULL;

			delete image;

			throw CTextureException( TEXERR_UNABLETOLOAD );
		}
		
		if( FAILED( D3DXLoadSurfaceFromMemory( surface, NULL, NULL, image->pPixels, D3DFMT_A8R8G8B8, image->width*sizeof(DWORD), NULL, &rect, D3DX_FILTER_LINEAR|D3DX_FILTER_DITHER, 0 ) ) )
		{
			surface->Release();

			texture->text->Release();
			texture->text = NULL;

			delete image;

			throw CTextureException( TEXERR_UNABLETOLOAD );
		}

		surface->Release();
	}		

	delete image;
}

void DeleteTexture( const char* name )
{	
	pTextList->Remove( name );
}

PDIRECT3DTEXTURE8 GetTexture( const char* name )
{	
	CTexture*			text;

	if( !name )
		return NULL;

	 text = pTextList->Get( name );	

	if( text )
		return text->text;

	return NULL;
}

void SetTexturePath( const char* path )
{
	strcpy( strPath, path );

	if( strPath[strlen(strPath)-1] != '\\' )
		strcat( strPath, "\\" );
}

