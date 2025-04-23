#include "stdafx.h"
#include "3dsScene.h"

// ech nie ma to jak uzywanie starych zrodel nigdy nic nie wiadomo 
// no ale nic :)

#define MATERIALTYPE		18
#define MULTITEXT			1
#define ENVONLY				32

/*
 *  teksty bledow
 */

static OBJECT*				current = NULL;
static FLOAT				__scale = 1.0f;

char *errormsg[10]=
{
	"none\n",
	"unable to open file\n",
	"invalid file\n",
	"invalid track or NULL pointer\n",
	"frame below zero\n",
	"insufficient memory\n",
	"invlid hierarchy entry\n",
	"invalid object or NULL pointer\n",
	"invalid scene or NULL pointer\n",
	"no map present or invalid\n"
};

/****************************************************************************
		     SCENE CONSTRUCTION FUNCTION
****************************************************************************/

/*
 *  przeksztalca soczewka (zamiast FOV w 3ds przechowywana jest inforamcja o
 *  ogniskowej soczewki kamery) na rozpietosc otroslupa widzenia (FOV)
 */
float LensToFOV(float lens)
{

	int  		i;
	float 		fov;

  struct {
    float lens, fov;
  } lens_table[] = {
    {15.0f,  115.0f}, {20.0f, 94.28571f}, {24.0f, 84.0f}, {28.0f,  76.36364f},
    {35.0f,  63.0f},  {50.0f, 46.0f},     {85.0f, 28.0f}, {135.0f, 18.0f},
    {200.0f, 12.0f}
  };

  for (i = 0; i < 9; i++)
   if (lens == lens_table[i].lens)
    fov = lens_table[i].fov;
   else
    fov = 15.0f / lens * 160.0f;

  return fov;
}

/*
 *  dodaje obiekt object do sceny s
 */
void AddObject(OBJECT *object, SCENE *s)
{
	OBJECT*			tmp=s->objects;

	object->next=NULL;

	if (!tmp)
		s->objects=object;
	else
	{
	   for(;tmp->next;tmp=tmp->next);

		tmp->next=object;
	}
}

void AddMaterial( MATERIAL *mat, SCENE *s)
{
	OBJECT*			tmp = s->materials;
	OBJECT*			obj = (OBJECT*)malloc( sizeof(OBJECT) );

	obj->next = NULL;
	obj->name = NULL;
	obj->obj  = mat;
	
	if( !tmp )
		s->materials = obj;
	else
	{
		for( ; tmp->next ; tmp=tmp->next );

		tmp->next = obj;
	}
}


/*
 *  dodaje key'a do listy
 */
void AddKey(KEY *key, KEY *&head)
{
	KEY*		tmp=head;

	key->next=NULL;

	if (!tmp)
		head=key;
	else
	{
		for(;tmp->next;tmp=tmp->next);

		tmp->next=key;
	}
}


/*
 *  dodaje sciezke track do sceny s
 */
void AddKeyframer(KEYFRAMER *track, SCENE *s)
{
	KEYFRAMER*		tmp=s->tracks;

	track->next=NULL;

	if (!tmp)
		s->tracks=track;
	else
	{
		for(;tmp->next;tmp=tmp->next);

		tmp->next=track;
	}
}

OBJECT* FindFirst( SCENE* s, DWORD type )
{
	OBJECT*			tmp=s->objects;

	while (tmp)
	{
		if( tmp->type == type )
			break;

		tmp=tmp->next;
	}	

	current = tmp;

	return tmp;
}

OBJECT* FindNext()
{
	if( !current )
		return NULL;

	OBJECT*		tmp = current->next;

	while( tmp )
	{
		if( tmp->type == current->type )
			break;
	}

	current = tmp;

	return tmp;
}

/*
 *  w scenie s znajduje obiekt o nazwie name i typie type lub pierwsze wystapienie
 *  obiektu o podanym typie jezeli name="" jezeli obiekt nie zostal znaleziony
 *  zwraca null
 */
OBJECT *FindObject(SCENE *s, char *name, DWORD type)
{
	OBJECT*			tmp=s->objects;

	while (tmp)
	{
		if ((!stricmp(name,tmp->name)&&(tmp->type&type))||(!strlen(name)&&(tmp->type&type)))
			return tmp;

		tmp=tmp->next;
	}

	return NULL;
}

OBJECT *FindObject(SCENE *s, char *name )
{
	OBJECT*			tmp=s->objects;

	while (tmp)
	{
		if ( !stricmp(name,tmp->name) )
			return tmp;

		tmp=tmp->next;
	}

	return NULL;
}

MATERIAL *FindMaterial(SCENE *s, char *name )
{
	OBJECT*			tmp=s->materials;
	MATERIAL*		mat;

	while( tmp )
	{
		mat = (MATERIAL*)tmp->obj;

		if( !strcmp(name, mat->name) )
			return mat;

		tmp=tmp->next;
	}

	return NULL;
}


/*
 *  inicjuje scene s ustawiajac odpowiednie zmienne
 */
void InitScene(SCENE *s)
{
	s->objects=NULL;
	s->tracks=NULL;
	s->materials = NULL;
	s->start=0;
	s->end=0;
}

/*
 *  odpowiednio modyfikuje pozycje obiektow uwzgledniajac pivot point
 */
void PrepareKeyframer(SCENE *s)
{
	KEYFRAMER*		tmp=s->tracks;
	OBJECT*			obj;
	TRIMESH*		mesh;
	MESHTRACK*		mtrack;
	CAMERA*			camera;
	CAMERATRACK*	ctrack;	
	KEY*			key;
	DWORD			i;

	while (tmp)
	{		
		switch( tmp->type )
		{		
			case ID_MESHKEYFRAMER:		

				obj=FindObject(s,tmp->objectname);
				tmp->object=obj->obj;
				mesh=(TRIMESH *)obj->obj;
				mtrack=(MESHTRACK *)tmp->track;
				for ( i = 0 ; i<mesh->vertex ; i++ )
				{
					mesh->vertices[i].x -= mtrack->px;
					mesh->vertices[i].y -= mtrack->py;
					mesh->vertices[i].z -= mtrack->pz;
				}

				mesh->morph = 0;

				if( mtrack->morph )
				{				
					tmp->flags	|= KFF_MORPH;
					mesh->morph = 1;
				}

				if( mtrack->hide )
				{
					if( mtrack->dwHideCount == 1 )
					{					
						mesh->hide = (DWORD)mtrack->hide->data.val;
						free( mtrack->hide );
						mtrack->hide = NULL;
					}
					else
						tmp->flags |= KFF_MORPH;
				}
			break;

			case ID_VIEWERKEYFRAMER:

				obj=FindObject(s,tmp->objectname);
				tmp->object=obj->obj;

				camera = (CAMERA*)obj->obj;
				ctrack = (CAMERATRACK*)tmp->track;

				for( key = ctrack->fov ; key ; key = key->next )
					key->data.val = LensToFOV( key->data.val );

				if( ctrack->fov )
				{				
					if( ctrack->dwFovCount == 1 )
					{					
						camera->FOV = ctrack->fov->data.val;
						free( ctrack->fov );
						ctrack->fov = NULL;
					}
					else
						tmp->flags |= KFF_FOV;
				}

				if( ctrack->roll )
				{				
					if( ctrack->dwRollCount == 1 )
					{					
						camera->roll = ctrack->roll->data.val;
						free( ctrack->roll );
						ctrack->roll = NULL;
					}
					else
						tmp->flags |= KFF_ROLL;
				}
			break;
		}

		tmp=tmp->next;
	}
}



/*
 *  znajduje rodzica sciezki track w scenie s, zwraca null jezeli nie istnieje
 *  wykozystuje identyfikatory hierarchi
 */
KEYFRAMER *FindParent(SCENE *s, KEYFRAMER *track)
{
	KEYFRAMER	*tmp=s->tracks;

 while (tmp)
  {

   if (tmp->hierarchy==track->link)
    {
     tmp->child=track;
     return tmp;
    }

   tmp=tmp->next;
  }

 return NULL;
}

void CreateSubNextLink( SCENE* s, KEYFRAMER* first )
{
	KEYFRAMER*			tmp = s->tracks;	
	KEYFRAMER*			leaf;	

	while( tmp )
	{
		if( tmp != first && tmp->parent == first->parent )
		{
			for( leaf = first ; leaf->brother ; leaf = leaf->brother );

			leaf->brother	=  tmp;
			leaf->flags		|= KFF_NEXT;
		}

		tmp = tmp->next;
	}

	for( tmp = first ; tmp ; tmp = tmp->brother )
	{
		for( leaf = s->tracks ; leaf && leaf->parent != tmp ; leaf = leaf->next );

		if( leaf )
		{
			tmp->flags |= KFF_SUB;
			tmp->child = leaf;
			CreateSubNextLink( s, leaf );
		}		
	}
}

/*
 *  wiaze hierarchie sciezek w scenie s
 */
void LinkHierarchy(SCENE *s)
{
	KEYFRAMER	*tmp=s->tracks;

	while (tmp)
	{

		if (tmp->link==0xFFFF)
			tmp->parent=NULL;
		else
			tmp->parent=FindParent(s,tmp);		

		tmp=tmp->next;
	}

	tmp = s->tracks;

	while( tmp )
	{
		if( !tmp->parent )
		{
			s->root = tmp;
			break;
		}
	}			

	CreateSubNextLink( s, s->root );
}


/*
 *  oblicza normalne dla wszystkich wieszcholkow obiektu mesh
 */
void CalculateNormals(TRIMESH *mesh)
{
	FLOAT		ax, ay, az;
	FLOAT		bx, by, bz;
	long		count;
	FLOAT		nx, ny, nz;
	FLOAT		length;
	FLOAT		cx = 0, cy = 0, cz = 0;

	for( DWORD i = 0 ; i < mesh->vertex ; i++ )
    {
		count	=	0;

		nx		=	0.0f;
		ny		=	0.0f;
		nz		=	0.0f;

		for ( DWORD j=0 ; j < mesh->face ; j++ )
			if (mesh->faces[j].a==i || mesh->faces[j].b==i || mesh->faces[j].c==i)
			{
				ax = mesh->vertices[mesh->faces[j].b].x - mesh->vertices[mesh->faces[j].a].x;
				ay = mesh->vertices[mesh->faces[j].b].y - mesh->vertices[mesh->faces[j].a].y;
				az = mesh->vertices[mesh->faces[j].b].z - mesh->vertices[mesh->faces[j].a].z;

				bx = mesh->vertices[mesh->faces[j].c].x - mesh->vertices[mesh->faces[j].a].x;
				by = mesh->vertices[mesh->faces[j].c].y - mesh->vertices[mesh->faces[j].a].y;
				bz = mesh->vertices[mesh->faces[j].c].z - mesh->vertices[mesh->faces[j].a].z;

				nx += ay*bz - az*by;
				ny += az*bx - ax*bz;
				nz += ax*by - ay*bx;

				count++;
			}

     
		length = nx*nx + ny*ny + nz*nz;

		if( length <= 0.001 )
		{		
			nx = 0.0f; ny = 0.0f; nz = 0.0f;
		}
		else
		{
			length = 1.0f/sqrtf( length );
			nx *= length;
			ny *= length;
			nz *= length;
		}

		mesh->vertices[i].nx = nx;
		mesh->vertices[i].ny = ny;
		mesh->vertices[i].nz = nz;

		cx += mesh->vertices[i].x;
		cy += mesh->vertices[i].y;
		cz += mesh->vertices[i].z;
	}

	mesh->cx = cx/(FLOAT)mesh->vertex;
	mesh->cy = cy/(FLOAT)mesh->vertex;
	mesh->cz = cz/(FLOAT)mesh->vertex;
}


/*
 *  usuwa liste obiektow
 */
void freeObjects(SCENE *s)
{
	TRIMESH		*mesh;
	LIGHT		*light;
	CAMERA 		*cam;
	TARGET		*target;
	OBJECT		*tmp;
	OBJECT		*root=s->objects;

	while (root)
	{
		tmp=root->next;
		switch (root->type)
		{
			case ID_MESH:
				mesh=(TRIMESH *)root->obj;
				free(mesh->faces);
			    free(mesh->vertices);
			    free(mesh);
			break;

			case ID_LIGHT:
				light=(LIGHT *)root->obj;
				free(light);
			break;

			case ID_TARGET:
				target =(TARGET *)root->obj;
				free( target->name );
				free(target);
			break;

			case ID_CAMERA:
				cam=(CAMERA *)root->obj;
				free(cam);
			break;
		}

		free(root->name);
		free(root);

		root=tmp;
	}
}

void freeMaterials( SCENE* s )
{
	OBJECT		*tmp;
	MATERIAL	*mat;
	OBJECT		*root=s->materials;

	while( root )
	{
		tmp=root->next;

		mat = (MATERIAL*)root->obj;
		free( mat->envmap );
		free( mat->texture);
		free( mat->name);
		free( mat );
		free( root );

		root = tmp;
	}
}

/*
 *  usuwa track
 */
void freeTrack(KEY *track)
{
	KEY	*tmp;

 while (track)
  {
   tmp=track->next;
   free(track);
   track=tmp;
  }
}

/*
 *  usuwa liste sciezek
 */
void freeTracks(SCENE *s)
{
	MESHTRACK*		meshtrc;
	LIGHTTRACK*		littrc;
	CAMERATRACK*	camtrc;
	TARGETTRACK*	camtrgtrc;
	KEYFRAMER*		track=s->tracks;
	KEYFRAMER*		tmp;

	while (track)
	{
		tmp=track->next;

		switch (track->type)
		{
		    case ID_MESHKEYFRAMER:
				meshtrc=(MESHTRACK *)track->track;
				freeTrack(meshtrc->position);
				freeTrack(meshtrc->rotation);
				freeTrack(meshtrc->scale);
				free(meshtrc);
			break;

			case ID_LIGHTKEYFRAMER:
				littrc=(LIGHTTRACK *)track->track;
				freeTrack(littrc->position);
				free(littrc);
			break;

			case ID_VIEWERKEYFRAMER:
				camtrc=(CAMERATRACK *)track->track;
				freeTrack(camtrc->position);
				free(camtrc);
			break;

			case ID_TARGETKEYFRAMER:
				camtrgtrc=(TARGETTRACK *)track->track;
				freeTrack(camtrgtrc->position);
				free(camtrgtrc);
			break;
		}

		free(track->objectname);
		free(track);

		track = tmp;
	}
}

/*
 *  usuwa cala scene
 */
void freeScene(SCENE *s)
{
 freeMaterials(s);
 freeObjects(s);
 freeTracks(s);
}


/*****************************************************************************
			  READER FUNCTION
*****************************************************************************/

/*
 *  odczytuje i zwraca z piku f pojedynczy bajt
 */
BYTE ReadByte(FILE *f)
{
	return fgetc(f);
}

/*
 *  odczytuje slowo
 */
WORD ReadWord(FILE *f)
{
	WORD		tmp;

	fread(&tmp,sizeof( WORD ),1,f);
	return tmp;
}

/*
 *  odczytuje podwojne slowo
 */
DWORD ReadDword(FILE *f)
{
	DWORD		tmp;

	fread(&tmp,sizeof(DWORD),1,f);
	return tmp;
}

/*
 *  odczytuje floata
 */
float ReadFloat(FILE *f)
{
	FLOAT		tmp;

	fread(&tmp,sizeof(FLOAT),1,f);
	return tmp;
}

/*
 *  odczytuje ciag znakow zakonczony 0, alokujac dla niego pamiec
 *  zwraca wskaznik na odczytany string
 */
char *ReadASCIIZ(FILE *f)
{
	char*				tmp=NULL;
	static	char		tmpstring[256];
	char				letter;
	int					size=0;

	memset(tmpstring,0,256);

	do{
		fread(&letter,sizeof(char),1,f);
		tmpstring[size++]=letter;
	}while ((size<256)&&(!feof(f))&&letter!=0);

	tmp=(char *)malloc(size+1);

	if (tmp)
		strcpy(tmp,tmpstring);

	strupr( tmp );

	return tmp;
}


/*
 *  przezkakuje pojedynczy chunk, kturego identyfikator zostal poprzednio
 *  odczytany
 */
int JumpThroughtChunk(FILE *f)
{
	DWORD		position;

	position=ReadDword(f)-6;
	fseek(f,position,SEEK_CUR);

	if (feof(f))
		return error_bad_file;

	return error_ok;
}

/*
 *  odczytuje z pliku liste wierzcholkow
 */
int ReadVerticesList(FILE *f, TRIMESH *mesh)
{
	DWORD			size;
	DWORD			pos;
	WORD			number;
	float			x, y, z;

	size=ReadDword(f)-6;
	pos=ftell(f);
	number=ReadWord(f);

	mesh->vertex=number;
	mesh->vertices=(VERTICE *)malloc(number*sizeof(VERTICE));

	if (!mesh->vertices)
		return error_memory;

	for (int i=0; (!feof(f))&&i<number; i++)
	{
		x=ReadFloat(f);
		y=ReadFloat(f);
		z=ReadFloat(f);

		mesh->vertices[i].x=x;
		mesh->vertices[i].y=z;	//swap vector
		mesh->vertices[i].z=y;

		mesh->vertices[i].u = 0.0f;
		mesh->vertices[i].v = 0.0f;
  }


 fseek(f,pos+size,SEEK_SET);

 if (feof(f))
  return error_bad_file;

 return error_ok;

}

/*
 *  odczytuje liste facow (scianek) obiektu, kazda scianka jest opisana
 *  przez trzy wierzcholki ktore ja twoza, oraz przez material ktory jest
 *  na nia nalozony
 */
int ReadFacesList(FILE *f, TRIMESH *mesh, SCENE *scena)
{
	DWORD			size;
	DWORD			pos;
	WORD			number;
	WORD			a, b, c;
	WORD			sign;
	char			*matname;	
	MATERIAL		*mat;

	size=ReadDword(f)-6;
	pos=ftell(f);
	number=ReadWord(f);

	mesh->group = 0;
	mesh->face=number;
	mesh->faces=(FACE *)malloc(number*sizeof(FACE));

	if (!mesh->faces)
		return error_memory;

	for (int i=0; (!feof(f))&&i<number; i++)
	{
		a=ReadWord(f);
		b=ReadWord(f);
		c=ReadWord(f);

		mesh->faces[i].a=a;
		mesh->faces[i].b=b;
		mesh->faces[i].c=c;
		mesh->faces[i].group = 0xffff;

		mesh->faces[i].mat=NULL;

		ReadWord(f);

	}

	if (feof(f))
		return error_bad_file;

	while ( (!feof(f))&& ftell(f) < (LONG)(size+pos))
	{
		sign=ReadWord(f);

		if (sign==FACES_MATERIAL_CHUNK)
		{			
			ReadDword(f);
			matname=ReadASCIIZ(f);

			mat = FindMaterial(scena,matname);						

			number=ReadWord(f);
			for (i=0; i<number; i++)
			{
				a=ReadWord(f);
				mesh->faces[a].mat=mat;
				mesh->faces[a].group = mesh->group;
			}

			mesh->group++;
		}
		else
			if (JumpThroughtChunk(f)!=error_ok)
				return error_bad_file;
  }

  if (feof(f))
	return error_bad_file;

 return error_ok;
}


/*
 *  wczytuje z pliku liste mapowania
 */
int ReadMappingList(FILE *f, TRIMESH *mesh)
{
	DWORD		size;
	DWORD		pos;
	WORD		number;
	FLOAT		u, v;

	size=ReadDword(f)-6;
	pos=ftell(f);
	number=ReadWord(f);

	for (int i=0; (!feof(f))&&i<number; i++)
	{
		u = ReadFloat(f);
		v = ReadFloat(f);

		mesh->vertices[i].u=u;
		mesh->vertices[i].v=v;
	}

	fseek(f,pos+size,SEEK_SET);

	if (feof(f))
		return error_bad_file;

	return error_ok;

}


/*
 *  odczytuje lokalny uklad wspolzednych i przeksztalca wspolzedne
 *  wierzcholkow obiektu tak aby byly w nim wyrazone (translacja i odpowiedni
 *  obrot poniewaz poczatkowe wspolzedne obiektu sa zapisane wzgledem
 *  globalego ukladu swiata)
 */
int ReadLocalCoords(FILE *f, TRIMESH *mesh)
{
	DWORD			size;
	DWORD			pos;
	FLOAT			a, b, c;		
	FLOAT			M[3][3], L[3][3];
	FLOAT			x, y, z;

	DWORD				i, j;

	size=ReadDword(f)-6;
	pos=ftell(f);

	for ( i=0;i<3;i++)
		for ( j=0;j<3;j++)
			M[i][j]=ReadFloat(f);	

	
	FLOAT		tmp;	

	for (i=0;i<3;i++)
	{
		tmp = M[i][1];
		M[i][1] = M[i][2];
		M[i][2] = tmp;
	}

	for (i=0;i<3;i++)
	{
		tmp = M[1][i];
		M[1][i] = M[2][i];
		M[2][i] = tmp;
	}

	
	FLOAT		scale;

	for( i = 0 ; i<3 ; i++ )
	{
		scale=sqr(M[i][0])+sqr(M[i][1])+sqr(M[i][2]);
		if (scale==0) scale=1;

		scale=1.0f/scale;

		for( j = 0 ; j < 3 ; j++ )
			L[i][j] = M[i][j]*scale;		
	}

	a=ReadFloat(f);
	b=ReadFloat(f);
	c=ReadFloat(f);

	for ( i=0 ; i < mesh->vertex ; i++)
	{
		mesh->vertices[i].x -= a;
		mesh->vertices[i].y -= c;
		mesh->vertices[i].z -= b;		

		x = mesh->vertices[i].x*L[0][0] + mesh->vertices[i].y*L[0][1] + mesh->vertices[i].z*L[0][2];
		y = mesh->vertices[i].x*L[1][0] + mesh->vertices[i].y*L[1][1] + mesh->vertices[i].z*L[1][2];
		z = mesh->vertices[i].x*L[2][0] + mesh->vertices[i].y*L[2][1] + mesh->vertices[i].z*L[2][2];		

		mesh->vertices[i].x = x;
		mesh->vertices[i].y = y;
		mesh->vertices[i].z = z;		
	}

	fseek(f,pos+size,SEEK_SET);

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje caly chunk obiektu
 */
int ReadTrimeshChunk(FILE *f, OBJECT *object, SCENE *scena)
{
	DWORD			size;
	DWORD			pos;
	WORD			sign;
	TRIMESH			*mesh;
	int				err;

	mesh=(TRIMESH *)malloc(sizeof(TRIMESH));

	if (!mesh)
		return error_memory;

	object->type = ID_MESH;
	object->obj	 = mesh;

	mesh->morph	 = 0;
	mesh->hide	 = 0;
	
	size=ReadDword(f)-6;
	pos=ftell(f);

	while ((!feof(f))&&ftell(f) < (LONG)(size+pos) )
	{
		sign = ReadWord(f);
		switch (sign)
		{
			case VERTICES_CHUNK:
				if ((err=ReadVerticesList(f, mesh))!=error_ok)
					return err;
			break;

			case FACES_CHUNK:
				if ((err=ReadFacesList(f, mesh, scena))!=error_ok)
					return err;
			break;

			case MAPPING_COORDS_CHUNK:
				if ((err=ReadMappingList(f, mesh))!=error_ok)
					return err;
			break;

			case LOCAL_COORDS_CHUNK:
				if ((err=ReadLocalCoords(f, mesh))!=error_ok)
					return err;
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;

		}
	}

	if (feof(f))
		return error_bad_file;

	CalculateNormals(mesh);

	return error_ok;
}

/*
 *  odczytuje informacje o obiekcia swiatla
 */
int ReadLightChunk(FILE *f, OBJECT *object)
{
	DWORD			size;
	DWORD			pos;
	float 			a, b, c;
	LIGHT			*light;


	light=(LIGHT *)malloc(sizeof(LIGHT));

	if (!light)
		return error_memory;

	object->type = ID_LIGHT;
	object->obj = light;

	size=ReadDword(f)-6;
	pos=ftell(f);

	a=ReadFloat(f);
	b=ReadFloat(f);
	c=ReadFloat(f);

	light->x = a;
	light->y = c;
	light->z = b;


	if( (LONG)(pos+size) < ftell(f) )
		if (ReadWord(f)==SPOTLIGHT_CHUNK)
		{
			a=ReadFloat(f);
			b=ReadFloat(f);
			c=ReadFloat(f);

			ReadFloat(f);
			ReadFloat(f);
		}

	fseek(f,pos+size,SEEK_SET);

	if (feof(f))
		return error_bad_file;

	return error_ok;

}

/*
 *  odczytuje chunk kamery
 */
int ReadCameraChunk(FILE *f, OBJECT *object1, OBJECT *object2 )
{
	DWORD		size;
	DWORD		pos;
	float 		a,b,c;
	CAMERA		*camera;
	TARGET		*target;

	camera=(CAMERA *)malloc(sizeof(CAMERA));
	target=(TARGET *)malloc(sizeof(TARGET));

	target->name = (char*)malloc( strlen(object1->name) + 1 );
	strcpy( target->name, object1->name );

	if (!camera)
		return error_memory;

	object1->type = ID_VIEWER;
	object1->obj = camera;

	object2->type = ID_TARGET;
	object2->obj = target;

	size=ReadDword(f)-6;
	pos=ftell(f);

	a=ReadFloat(f);
	b=ReadFloat(f);
	c=ReadFloat(f);

	camera->x=a;
	camera->y=c;
	camera->z=b;

	a=ReadFloat(f);
	b=ReadFloat(f);
	c=ReadFloat(f);
	target->x=a;
	target->y=c;
	target->z=b;

	a=ReadFloat(f);
	camera->roll=a;
	a=ReadFloat(f);
	camera->FOV=LensToFOV(a);

	fseek(f,pos+size,SEEK_SET);

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odchytuje chunk obiektu (w takim chunku moga byc zapisane informacje
 *  o zrodlach swiatla, obiektach 3d (mesh'e) oraz o kamerze)
 */
int ReadObjectChunk(FILE *f, SCENE *s)
{
	DWORD			size; //rozmiar chunka
	DWORD			pos;
	DWORD			sign;
	OBJECT			*object;
	int				err;

	object=(OBJECT *)malloc(sizeof(OBJECT));

	if (!object)
		return error_memory;

	size=ReadDword(f)-6;
	pos=ftell(f);
	object->name=ReadASCIIZ(f);

	if (!object->name)
		return error_memory;

	while ((!feof(f))&&ftell(f) < (LONG)(size+pos) )
	{
		sign=ReadWord(f);
		switch (sign)
		{
			case TRIMESH_CHUNK:
				if ((err=ReadTrimeshChunk(f, object, s))!=error_ok)
					return err;

				AddObject(object, s);

				printf( "....mesh %s loaded\n", object->name );
			break;

			case LIGHT_CHUNK:
				if ((err=ReadLightChunk(f, object))!=error_ok)
					return err;

				AddObject(object, s);

				printf( "....light %s loaded\n", object->name );
			break;

			case CAMERA_CHUNK:
				OBJECT*			target;

				target = (OBJECT*)malloc( sizeof(OBJECT) );
				target->name = (char*)malloc( strlen(object->name) + 10 );

				strcpy( target->name, object->name );
				strcat( target->name, ".TARGET" );
				
				if ((err=ReadCameraChunk(f, object, target))!=error_ok)
					return err;

				AddObject(object, s);
				AddObject(target, s);
				
				printf( "....camera %s loaded\n", object->name );
				printf( "....target %s loaded\n", target->name );				
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
		break;
		}
	}

	if (feof(f))
		return error_bad_file;	

	return error_ok;
}


int ReadMapChunk(FILE *f, char*& name )
{
	WORD		sign;
	DWORD		size;
	DWORD		pos;	

	size=ReadDword(f)-6;
	pos=ftell(f);

	while ((!feof(f))&&ftell(f)<(LONG)(size+pos) )
	{
		sign=ReadWord(f);
		if (sign==MAP_FILENAME_CHUNK)
		{
			ReadDword(f);
			name=ReadASCIIZ(f);				
		}
		else
			if (JumpThroughtChunk(f)!=error_ok)
				return error_bad_file;
	}

	if (feof(f))
		return error_bad_file;

	return error_ok;
}

/*
 *  odczytuje informacje o materiale
 */
int ReadMaterialChunk(FILE *f, SCENE *s)
{
	WORD			sign;
	DWORD			size;
	DWORD			pos;	
	MATERIAL		*mat;
	int				err;
 
	mat=(MATERIAL *)malloc(sizeof(MATERIAL));

	if( !mat )
		return error_memory; 

	mat->texture= NULL;
	mat->envmap	= NULL;
	mat->type	= MATERIALTYPE;

	size=ReadDword(f)-6;
	pos=ftell(f);

	while ((!feof(f))&&ftell(f) < (LONG)(size+pos) )
	{
		sign =ReadWord(f);
		
		switch( sign )
		{
			case MATERIAL_NAME_CHUNK:
				ReadDword(f);
				mat->name=ReadASCIIZ(f);

				if (!mat->name)
					return error_memory;				
			break;

			case TEXTURE_MAP1_CHUNK:      
				if ((err=ReadMapChunk(f,mat->texture))!=error_ok)
					return err;
			break;     

			case TEXTURE_MAP2_CHUNK:				
				if ((err=ReadMapChunk(f,mat->envmap))!=error_ok)
					return err;				
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;
		}
	}

	if (feof(f))
		return error_bad_file;

	if( mat->texture && mat->envmap )
		mat->type |= MULTITEXT;

	if( !mat->texture && mat->envmap )
	{
		mat->texture = mat->envmap;
		mat->envmap = NULL;
		mat->type &= 0xfffffffe;
		mat->type |= ENVONLY;
	}

	AddMaterial( mat, s );

	printf( "....material %s loaded\n", mat->name );

	return error_ok;
}


/*
 *  odczytuje jeden z gluwnych chunkow pliku - chunk edytora, jego
 *  subchunkami sa chanki zawierajace inforamcje o obiektach i materialach
 */
int ReadEditorChunk(FILE *f, SCENE *scene)
{
	WORD		sign;
	DWORD		size;
	DWORD		pos;
	int			err;

	size=ReadDword(f)-6;
	pos	=ftell(f);

	while ((!feof(f))&&ftell(f)<(LONG)(size+pos) )
	{
		sign=ReadWord(f);

		switch (sign)
		{
			case OBJECT_CHUNK:
				if ((err=ReadObjectChunk(f,scene))!=error_ok)
					return err;
			break;

			case MATERIAL_CHUNK:
				if ((err=ReadMaterialChunk(f,scene))!=error_ok)
					return err;
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;
		}
	}

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje chunk zawierajacy informacje o wersji 3ds'a ktory zostal
 *  uzyty do otrzymania czytanego pliku
 */
int ReadVersionChunk(SCENE *s, FILE *f)
{

	ReadDword(f);
	s->version=ReadDword(f);

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje nformacje o obiekcie ktorego sciezka jest wlasnie czytana
 *  (patrz nizej)
 */
int ReadObjectInfo(FILE *f, KEYFRAMER *node)
{
	WORD		parent;

	ReadDword(f);
	node->objectname=ReadASCIIZ(f);

	if (!node->objectname)
		return error_memory;

	ReadWord(f);
	ReadWord(f);
	parent=ReadWord(f);

	node->link=parent;

	if (feof(f))
		return error_bad_file;

	return error_ok;

}


/*
 *  czyta wpolrzedne pivota obiektu, jest to punkt wokol ktorego nastepuje
 *  rotacja obiektu
 */
int ReadPivot(FILE *f, MESHTRACK *track)
{
	float		a,b,c;

	ReadDword(f);

	a=ReadFloat(f);
	b=ReadFloat(f);
	c=ReadFloat(f);

	track->px=a;
	track->py=c;
	track->pz=b;

	if (feof(f))
		return error_bad_file;

	return error_ok;
}

/*
 *  odczytuje track obrotu, w 3ds sciezka jest zapisana jako os obrotu, oraz
 *  kat obrotu, w tej funkcji nastepuje konwersja ma kwaternion
 */
int ReadQuaternionTrack(FILE *f, KEY *&track, DWORD& number )
{	
	WORD		flag;
	FLOAT		angle, a, b, c;
	DWORD		frame;
	KEY			*key;	

	ReadDword(f);
	ReadWord(f);
	ReadDword(f);
	ReadDword(f);
	number=ReadDword(f);	

	for( DWORD i=0; (!feof(f))&&i<number; i++ )
	{
		key=(KEY *)malloc(sizeof(KEY));

		if (!key)
			return error_memory;

		frame=ReadDword(f);
		key->frame=(FLOAT)frame;

		key->T = 0;
		key->B = 0;
		key->C = 0;
		key->easefrom=0;
		key->easeto=0;

		flag = ReadWord(f);
		for( int j = 0 ; j < 16 ; j++ )
			if( flag&( 1<<j ) )
				switch( j )
				{
					case 0: key->T = ReadFloat(f); break;
					case 1: key->C = ReadFloat(f); break;
					case 2: key->B =ReadFloat(f); break;
					case 3: key->easeto=ReadFloat(f); break;
					case 4: key->easefrom=ReadFloat(f); break;
				}

		angle=ReadFloat(f);
		a=ReadFloat(f);
		b=ReadFloat(f);
		c=ReadFloat(f);

		key->data.val = angle;
		key->data.x = a;
		key->data.y = c;
		key->data.z = b;	

		AddKey(key,track);
	}

	if (feof(f))
		return error_bad_file;

	return error_ok;

}


/*
 *  odczytuje sciezke wektorowa (moze byc to sciezka pozycji, skalowania lub
 *  koloru)
 */
int ReadVectorTrack(FILE *f, KEY *&track, DWORD& number )
{
	WORD		flag;
	float		a, b, c;
	DWORD		frame;
	KEY			*key;

	ReadDword(f);
	ReadWord(f);
	ReadDword(f);
	ReadDword(f);
	number=ReadDword(f);

	for (DWORD i=0; (!feof(f))&&i<number; i++)
	{
		key=(KEY *)malloc(sizeof(KEY));

		if (!key)
			return error_memory;

		frame=ReadDword(f);
		key->frame = (FLOAT)frame;

		key->T=0;
		key->B=0;
		key->C=0;
		key->easefrom=0;
		key->easeto=0;

		flag=ReadWord(f);

		for( int j = 0 ; j < 16 ; j++ )
			if( flag&( 1<<j ) )
				switch (j)
				{
					case 0: key->T=ReadFloat(f); break;
					case 1: key->C=ReadFloat(f); break;
					case 2: key->B=ReadFloat(f); break;
					case 3: key->easeto=ReadFloat(f); break;
					case 4: key->easefrom=ReadFloat(f); break;
				}

		a=ReadFloat(f);
		b=ReadFloat(f);
		c=ReadFloat(f);
		key->data.x=a;
		key->data.y=c;	//wector swap
		key->data.z=b;

		AddKey(key,track);
	}

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje jednowymiarowa sciezke (roll, FOV)
 */
int ReadFloatTrack(FILE *f, KEY *&track, DWORD& number )
{
	WORD		flag;
	float		a;
	DWORD		frame;
	KEY			*key;

	ReadDword(f);
	ReadWord(f);
	ReadDword(f);
	ReadDword(f);
	number=ReadDword(f);

	for( DWORD i=0; (!feof(f))&&i<number; i++ )
	{
		key=(KEY *)malloc(sizeof(KEY));

		if (!key)
			return error_memory;

		frame = ReadDword(f);
		key->frame=(FLOAT)frame;

		key->T = 0;
		key->B = 0;
		key->C = 0;
		key->easefrom=0;
		key->easeto=0;

		flag=ReadWord(f);
		for( int j = 0 ; j < 16 ; j++ )
			if( flag&( 1<<j ) )
			{
				switch( j )
				{
			        case 0: key->T=ReadFloat(f); break;
					case 1: key->C=ReadFloat(f); break;
					case 2: key->B=ReadFloat(f); break;
					case 3: key->easeto=ReadFloat(f); break;
					case 4: key->easefrom=ReadFloat(f); break;
				}
			}


		a=ReadFloat(f);
		key->data.val=a;

		AddKey(key,track);
	}

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje sciezke morfingu, kazdy key zawiera nazwe obiektu do
 *  ktorego ma byc zmorphowany obiekt
 */
int ReadMorphTrack(FILE *f, KEY *&track, DWORD& number )
{
	WORD		flag;
	DWORD		frame;
	KEY			*key;
	char		*objname;	

	ReadDword(f);
	ReadWord(f);
	ReadDword(f);
	ReadDword(f);
	number=ReadDword(f);

	for( DWORD i=0; (!feof(f))&&i<number; i++)
	{
		key=(KEY *)malloc(sizeof(KEY));

		if (!key)
			return error_memory;

		frame=ReadDword(f);
		key->frame=(FLOAT)frame;

		key->T=0;
		key->B=0;
		key->C=0;
		key->easefrom=0;
		key->easeto=0;

		flag=ReadWord(f);
		for( int j=0 ; j<16 ; j++ )
			if (flag&(1<<j))
			{
				switch( j )
				{
					case 0: key->T=ReadFloat(f); break;
					case 1: key->C=ReadFloat(f); break;
					case 2: key->B=ReadFloat(f); break;
					case 3: key->easeto=ReadFloat(f); break;
					case 4: key->easefrom=ReadFloat(f); break;
				}
			}


		objname=ReadASCIIZ(f);
		if (!objname)
			return error_memory;

		key->data.name = objname;		

		AddKey(key,track);
	}

	if (feof(f))
		return error_bad_file;

	return error_ok;
}

/*
 *  wczytuje track hida, jest on przydatny przy animomaniu skomplikowanych
 *  scen jezeli wiadomo ze nie ktore obiekty w danej chwili sa niewidoczne
 *  si im atrybut na hide, co zapogiega ich transformacja i rendrowaniu
 */
int ReadHideTrack(FILE *f, KEY *&track, DWORD& number)
{
	WORD		flag;
	DWORD		frame;
	KEY			*key;
	int	     	hide=0;


	ReadDword(f);
	ReadWord(f);
	ReadDword(f);
	ReadDword(f);
	number=ReadDword(f);

	for( DWORD i=0 ; (!feof(f))&&i<number ; i++ )
	{
		key=(KEY *)malloc(sizeof(KEY));

		if (!key)
			return error_memory;

		frame=ReadDword(f);
		key->frame=(FLOAT)frame;

		key->T=0;
		key->B=0;
		key->C=0;
		key->easefrom=0;
		key->easeto=0;

		flag=ReadWord(f);
		for( int j=0; j<16; j++ )
			if( flag&(1<<j) )
			{
				switch (j)
				{
			       case 0: key->T = ReadFloat(f); break;
				   case 1: key->C = ReadFloat(f); break;
				   case 2: key->B = ReadFloat(f); break;
				   case 3: key->easeto=ReadFloat(f); break;
				   case 4: key->easefrom=ReadFloat(f); break;
				}
			}

		key->data.val=(FLOAT)(hide^=1);

		AddKey(key,track);
	}

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje informacje o hierarchi obiektu (identyfikator hierarchi)
 */
int ReadHierarchy(FILE *f, KEYFRAMER *node)
{
	WORD		hierarchy;

	ReadDword(f);
	hierarchy=ReadWord(f);

	node->hierarchy=hierarchy;

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje pelna informacje o sciezce obiektu 3d
 *  tj. sciezke translacji, obrotu, skalowania i morfingu, a takze
 *  informacje o obiekcie do ktorego odnosi sie ta sciezka, pivot point
 *  oraz hierarchie
 */
int ReadMeshTrackChunk(FILE *f, KEYFRAMER *node )
{
	DWORD			size;
	DWORD			pos;
	WORD			sign;
	MESHTRACK*		track;
	int				err;

	node->type	= ID_MESHKEYFRAMER;
	node->flags = 0;

	track=(MESHTRACK *)malloc(sizeof(MESHTRACK));

	if (!track)
		return error_memory;

	size=ReadDword(f)-6;
	pos=ftell(f);

	track->position=NULL;
	track->rotation=NULL;
	track->scale=NULL;
	track->morph=NULL;
	track->hide=NULL;

	while ((!feof(f))&&ftell(f)<(LONG)(size+pos) )
	{
		sign=ReadWord(f);

		switch( sign )
		{
			case OBJECT_INFO_CHUNK:
				if ((err=ReadObjectInfo(f,node))!=error_ok)
					return err;
			break;

			case ROTATION_TRACK_CHUNK:
				if ((err=ReadQuaternionTrack(f,track->rotation,track->dwRotCount))!=error_ok)
					return err;			
			break;

			case POSITION_TRACK_CHUNK:
				if ((err=ReadVectorTrack(f,track->position, track->dwPosCount))!=error_ok)
					return err;			
			break;

			case SCALE_TRACK_CHUNK:
				if ((err=ReadVectorTrack(f,track->scale, track->dwSclCount))!=error_ok)
					return err;				
			break;

			case MORPH_TRACK_CHUNK:
				if ((err=ReadMorphTrack(f, track->morph, track->dwMorphCount))!=error_ok)
					return err;
			break;

			case HIDE_TRACK_CHUNK:
				if ((err=ReadHideTrack(f,track->hide, track->dwHideCount ))!=error_ok)
					return err;
			break;

			case HIERARCHY_CHUNK:
				if ((err=ReadHierarchy(f,node))!=error_ok)
					return err;
			break;

			case OBJECT_PIVOT_CHUNK:
				if ((err=ReadPivot(f,track))!=error_ok)
					return err;
			break;

			case DUMMYNAME_CHUNK:
				ReadDword( f );
				free( node->objectname );
				node->object = ReadASCIIZ( f );
				node->type = ID_NULLKEYFRAMER;
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;
		}
	}

	node->track=track;

	printf( "....keyframer of mesh %s loaded\n", node->objectname );

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje informacje o ruchu zrodla swiatla
 */
int ReadLightTrackChunk(FILE *f, KEYFRAMER *node)
{

	DWORD			size;
	DWORD			pos;
	WORD			sign;
	LIGHTTRACK*		track;
	int				err;

	node->type = ID_LIGHTKEYFRAMER;
	node->flags= 0;

	track=(LIGHTTRACK *)malloc(sizeof(LIGHTTRACK));

	if (!track)
		return error_memory;

	size=ReadDword(f)-6;
	pos=ftell(f);

	track->position=NULL;

	while ((!feof(f))&&ftell(f)<(LONG)(size+pos) )
	{	
		sign=ReadWord(f);

		switch( sign )
		{
			case OBJECT_INFO_CHUNK:
				if ((err=ReadObjectInfo(f,node))!=error_ok)
					return err;
			break;

			case POSITION_TRACK_CHUNK:
				if ((err=ReadVectorTrack(f,track->position, track->dwPosCount ))!=error_ok)
					return err;      
			break;

			case HIERARCHY_CHUNK:
				if ((err=ReadHierarchy(f,node))!=error_ok)
					return err;
			break;

		    default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;
		}
	}

	node->track=track;

	printf( "....keyframer of light %s loaded\n", node->objectname );

	if (feof(f))
		return error_bad_file;

	return error_ok;
}



/*
 *  odczytuje informacje o ruchu celu kamery
 */
int ReadCameraTrgTrackChunk(FILE *f, KEYFRAMER *node)
{

	DWORD				size;
	DWORD				pos;
	WORD				sign;
	TARGETTRACK*		track;
	int 				err;


	node->type = ID_TARGETKEYFRAMER;
	node->flags = 0;
	
	track=(TARGETTRACK *)malloc(sizeof(TARGETTRACK));

	if (!track)
		return error_memory;

	size=ReadDword(f)-6;
	pos=ftell(f);

	track->position=NULL;

	while ((!feof(f))&&ftell(f)<(LONG)(size+pos) )
	{
		sign = ReadWord(f);

		switch( sign )
		{
			case OBJECT_INFO_CHUNK:
				if ((err=ReadObjectInfo(f,node))!=error_ok)
					return err;

				char*		name;

				name = (char*)malloc( strlen(node->objectname) + 10 );
				strcpy( name, node->objectname );
				free( node->objectname );
				strcat( name, ".TARGET" );
				node->objectname = name;
			break;

			case POSITION_TRACK_CHUNK:
				if ((err=ReadVectorTrack(f,track->position,track->dwPosCount))!=error_ok)
					return err;			
			break;

			case HIERARCHY_CHUNK:
				if ((err=ReadHierarchy(f,node))!=error_ok)
					return err;
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;
		}
	}

	node->track=track;

	printf( "....keyframer of target %s loaded\n", node->objectname );

	if (feof(f))
		return error_bad_file;

	return error_ok;

}


/*
 *  czyta sciezki kamery (pozycje i roll'a)
 */
int ReadCameraTrackChunk(FILE *f, KEYFRAMER *node)
{

	DWORD				size;
	DWORD				pos;
	WORD				sign;
	CAMERATRACK*		track;
	int					err;

	node->type=ID_VIEWERKEYFRAMER;
	node->flags = 0;

	track = (CAMERATRACK *)malloc(sizeof(CAMERATRACK));

	if (!track)
		return error_memory;

	size = ReadDword(f)-6;
	pos = ftell(f);

	track->roll =NULL;
	track->fov =NULL;
	track->position=NULL;

	while ((!feof(f))&&ftell(f)<(LONG)(size+pos) )
	{
		sign=ReadWord(f);

		switch( sign )
		{
			case OBJECT_INFO_CHUNK:
				if ((err=ReadObjectInfo(f,node))!=error_ok)
					return err;
			break;

			case POSITION_TRACK_CHUNK:
				if ((err=ReadVectorTrack(f,track->position,track->dwPosCount))!=error_ok)
					return err;      
			break;

			case ROLL_TRACK_CHUNK:
				if ((err=ReadFloatTrack(f,track->roll, track->dwRollCount ))!=error_ok)
					return err;      
			break;			

			case FOV_TRACK_CHUNK:
				if ((err=ReadFloatTrack(f,track->fov,track->dwFovCount))!=error_ok)
					return err;
			break;

			case HIERARCHY_CHUNK:
				if ((err=ReadHierarchy(f,node))!=error_ok)
					return err;
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;
		}

	}

	node->track=track;

	printf( "....keyframer of camera %s loaded\n", node->objectname );

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  odczytuje chunk klatek czyli informacje od ktorej klatki zaczyna sie
 *  animacja i na ktorej sie konczy
 */
int ReadFramesChunk(FILE *f, SCENE *scene)
{
	ReadDword(f);
	scene->start=ReadDword(f);
	scene->end=ReadDword(f);

	if (feof(f))
		return error_bad_file;

	return error_ok;

}

/*
 *  odchytuje informacje o wszystkich sciezkach obiektow
 */
int ReadKeyframerChunk(FILE *f, SCENE *scene)
{
	DWORD			size;
	DWORD			pos;
	WORD			sign;
	KEYFRAMER		*keyframer;
	int				err;

	size=ReadDword(f)-6;
	pos=ftell(f);

	while ((!feof(f))&&ftell(f)<(LONG)(size+pos) )
	{
		sign=ReadWord(f);

		switch( sign )
		{
			case MESH_TRACK_CHUNK:
				keyframer=(KEYFRAMER *)malloc(sizeof(KEYFRAMER));				

				if (!keyframer)
					return error_memory;

				keyframer->parent	= NULL;
				keyframer->child	= NULL;
				keyframer->brother	= NULL;

				if ((err=ReadMeshTrackChunk(f,keyframer ))!=error_ok)
					return err;

				AddKeyframer(keyframer,scene);
			break;

			case OMNILIGHT_TRACK_CHUNK:
				keyframer=(KEYFRAMER *)malloc(sizeof(KEYFRAMER));

				if (!keyframer)
					return error_memory;

				keyframer->parent	= NULL;
				keyframer->child	= NULL;
				keyframer->brother	= NULL;

				if ((err=ReadLightTrackChunk(f,keyframer))!=error_ok)
					return err;

				AddKeyframer(keyframer,scene);
			break;

			case CAMERA_TRACK_CHUNK:
				keyframer=(KEYFRAMER *)malloc(sizeof(KEYFRAMER));

				if (!keyframer)
					return error_memory;

				keyframer->parent	= NULL;
				keyframer->child	= NULL;
				keyframer->brother	= NULL;

				if ((err=ReadCameraTrackChunk(f,keyframer))!=error_ok)
					return err;

				AddKeyframer(keyframer,scene);
			break;

			case CAMERA_TARGET_TRACK_CHUNK:
				keyframer=(KEYFRAMER *)malloc(sizeof(KEYFRAMER));

				if (!keyframer)
					return error_memory;

				keyframer->parent	= NULL;
				keyframer->child	= NULL;
				keyframer->brother	= NULL;

				if ((err=ReadCameraTrgTrackChunk(f,keyframer))!=error_ok)
					return err;

				AddKeyframer(keyframer,scene);
			break;

			case FRAMES_CHUNK:
				if ((err=ReadFramesChunk(f,scene))!=error_ok)
					return err;
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;
		}
	}

	if (feof(f))
		return error_bad_file;

	return error_ok;
}


/*
 *  wczytuje plik 3DS
 */
int Read3DS(char name[], SCENE *scene)
{
	WORD		sign;
	FILE		*f;
	int			err;

	if (!(f=fopen(name,"rb")))
		return error_file_open;

	sign=ReadWord(f);
	if (sign!=MAIN_CHUNK)
		return error_bad_file;

	ReadDword(f);

	while( 1 )
	{
		sign = ReadWord(f);

		if( feof( f ) )
			break;

		switch (sign)
		{
			case EDITOR_CHUNK:
				if ((err=ReadEditorChunk(f,scene))!=error_ok)
					return err;
			break;

			case KEYFRAMER_CHUNK:
				if ((err=ReadKeyframerChunk(f,scene))!=error_ok)
					return err;
				PrepareKeyframer(scene);
			break;

			case VERSION_CHUNK:
				if ((err=ReadVersionChunk(scene,f))!=error_ok)
					return err;
			break;

			default:
				if (JumpThroughtChunk(f)!=error_ok)
					return error_bad_file;
			break;
		}
	}

	LinkHierarchy(scene);

	fclose( f );

	return error_ok;
}

void WriteWord( WORD w, FILE* out )
{
	fwrite( &w, sizeof(WORD), 1, out );	
}

void WriteDword( DWORD dw, FILE* out )
{
	fwrite( &dw, sizeof(DWORD), 1, out );	
}

void WriteFloat( FLOAT f, FILE* out )
{
	fwrite( &f, sizeof(DWORD), 1, out );
}

void WriteString( char* str, FILE* out )
{
	char		null = 0;

	if( !str )
		fwrite( &null, sizeof(char), 1, out );
	else
		fwrite( str, sizeof(char), strlen(str) + 1, out );
}

void WriteMesh( TRIMESH* mesh, FILE* out )
{
	WriteDword( mesh->vertex, out );
	WriteDword( mesh->face, out );
	WriteDword( mesh->face*3, out );
	WriteDword( mesh->hide, out );
	WriteFloat( mesh->cx, out );
	WriteFloat( mesh->cy, out );
	WriteFloat( mesh->cz, out );

	LPWORD				indices = (LPWORD)malloc( mesh->face*3*sizeof(WORD) );	
	DWORD				i, j;	
	LPDWORD				seg = (LPDWORD)malloc( mesh->group*sizeof(DWORD) );	
	MATERIAL**			mats = (MATERIAL**)malloc( mesh->group*sizeof(MATERIAL*) );
	DWORD				index = 0;
	DWORD				groups = mesh->group;
	DWORD				cleargroup = 0;

	for( i = 0 ; i < mesh->group ; i++ )
	{
		seg[i] = 0;

		for( j = 0 ; j < mesh->face ; j++ )
			if( mesh->faces[j].group == i )
			{
				seg[i]++;
				mats[i] = mesh->faces[j].mat;
				indices[index++] = (WORD)mesh->faces[j].a;
				indices[index++] = (WORD)mesh->faces[j].b;
				indices[index++] = (WORD)mesh->faces[j].c;
			}
	}

	for( j = 0 ; j < mesh->face ; j++ )
	{
		if( mesh->faces[i].group == 0xffff )
		{
			indices[index++] = (WORD)mesh->faces[j].a;
			indices[index++] = (WORD)mesh->faces[j].b;
			indices[index++] = (WORD)mesh->faces[j].c;

			cleargroup = TRUE;
		}
	}

	if( cleargroup )
		groups++;

	WriteDword( groups, out );
	WriteDword( mesh->morph, out );

	printf( "     + number of vertices: %d\n", mesh->vertex );
	printf( "     + number of faces: %d\n", mesh->face );
	printf( "     + number of group: %d\n", groups );
	
	for( i = 0 ; i < mesh->vertex ; i++ )
	{
		WriteFloat( mesh->vertices[i].x*__scale, out );
		WriteFloat( mesh->vertices[i].y*__scale, out );
		WriteFloat( mesh->vertices[i].z*__scale, out );

		WriteFloat( mesh->vertices[i].nx, out );
		WriteFloat( mesh->vertices[i].ny, out );
		WriteFloat( mesh->vertices[i].nz, out );

		WriteFloat( mesh->vertices[i].u, out );
		WriteFloat( mesh->vertices[i].v, out );
	}

	for( i = 0 ; i < mesh->face*3 ; i++ )
		WriteWord( indices[i], out );

	DWORD		startindex = 0;

	for( i = 0 ; i < mesh->group ; i++ )
	{
		WriteString( mats[i]->name, out );
		WriteDword( 3*startindex, out );
		WriteDword( seg[i], out );

		startindex += seg[i];
	}

	if( cleargroup )
	{
		WriteString( "$EMPTYMATERIAL", out );
		WriteDword( 3*startindex, out );
		WriteDword( mesh->face - startindex, out );
	}
}

void WriteVectorTrack( KEY* key, FILE* out )
{
	KEY*			tmp;
	DWORD			counter = 0;

	for( tmp = key ; tmp ; tmp = tmp->next, counter++ );

	WriteDword( counter, out );

	for( tmp = key ; tmp ; tmp = tmp->next )
	{
		WriteFloat( tmp->data.x*__scale, out );
		WriteFloat( tmp->data.y*__scale, out );
		WriteFloat( tmp->data.z*__scale, out );

		WriteFloat( tmp->frame, out );

		WriteFloat( tmp->T, out );
		WriteFloat( tmp->B, out );
		WriteFloat( tmp->C, out );

		WriteFloat( tmp->easefrom, out );
		WriteFloat( tmp->easeto, out );
	}
}

void WriteScaleTrack( KEY* key, FILE* out )
{
	KEY*			tmp;
	DWORD			counter = 0;

	for( tmp = key ; tmp ; tmp = tmp->next, counter++ );

	WriteDword( counter, out );

	for( tmp = key ; tmp ; tmp = tmp->next )
	{
		WriteFloat( tmp->data.x, out );
		WriteFloat( tmp->data.y, out );
		WriteFloat( tmp->data.z, out );

		WriteFloat( tmp->frame, out );

		WriteFloat( tmp->T, out );
		WriteFloat( tmp->B, out );
		WriteFloat( tmp->C, out );

		WriteFloat( tmp->easefrom, out );
		WriteFloat( tmp->easeto, out );
	}
}

void WriteRotationTrack( KEY* key, FILE* out )
{
	KEY*			tmp;
	DWORD			counter = 0;

	for( tmp = key ; tmp ; tmp = tmp->next, counter++ );

	WriteDword( counter, out );

	for( tmp = key ; tmp ; tmp = tmp->next )
	{
		WriteFloat( tmp->data.x, out );
		WriteFloat( tmp->data.y, out );
		WriteFloat( tmp->data.z, out );
		WriteFloat( tmp->data.val, out );

		WriteFloat( tmp->frame, out );

		WriteFloat( tmp->T, out );
		WriteFloat( tmp->B, out );
		WriteFloat( tmp->C, out );

		WriteFloat( tmp->easefrom, out );
		WriteFloat( tmp->easeto, out );
	}
}

void WriteFloatTrack( KEY* key, FILE* out )
{
	KEY*			tmp;
	DWORD			counter = 0;

	for( tmp = key ; tmp ; tmp = tmp->next, counter++ );

	WriteDword( counter, out );

	for( tmp = key ; tmp ; tmp = tmp->next )
	{
		WriteFloat( tmp->data.val, out );		

		WriteFloat( tmp->frame, out );

		WriteFloat( tmp->T, out );
		WriteFloat( tmp->B, out );
		WriteFloat( tmp->C, out );

		WriteFloat( tmp->easefrom, out );
		WriteFloat( tmp->easeto, out );
	}
}

void WriteMorphTrack( KEY* key, FILE* out )
{
	KEY*			tmp;
	DWORD			counter = 0;

	for( tmp = key ; tmp ; tmp = tmp->next, counter++ );

	WriteDword( counter, out );

	for( tmp = key ; tmp ; tmp = tmp->next )
	{
		WriteString( tmp->data.name, out );
		
		WriteFloat( tmp->frame, out );

		WriteFloat( tmp->easefrom, out );
		WriteFloat( tmp->easeto, out );
	}
}

void WriteHideTrack( KEY* key, FILE* out )
{
	KEY*			tmp;
	DWORD			counter = 0;

	for( tmp = key ; tmp ; tmp = tmp->next, counter++ );

	WriteDword( counter, out );

	for( tmp = key ; tmp ; tmp = tmp->next )
	{
		WriteDword( (DWORD)key->data.val, out );
		
		WriteFloat( tmp->frame, out );		

		WriteFloat( tmp->easefrom, out );
		WriteFloat( tmp->easeto, out );
	}
}

void WriteEmptyRotation( FILE* out )
{
	WriteDword( 1, out );

	WriteFloat( 1.0f, out );
	WriteFloat( 0.0f, out );
	WriteFloat( 0.0f, out );
	WriteFloat( 0.0f, out );
	
	WriteFloat( 0.0f, out );

	WriteFloat( 0.0f, out );
	WriteFloat( 0.0f, out );
	WriteFloat( 0.0f, out );

	WriteFloat( 0.0f, out );
	WriteFloat( 0.0f, out );
}

void WriteEmptyScale( FILE* out )
{
	WriteDword( 1, out );

	WriteFloat( 1.0f, out );
	WriteFloat( 1.0f, out );
	WriteFloat( 1.0f, out );	
	
	WriteFloat( 0.0f, out );

	WriteFloat( 0.0f, out );
	WriteFloat( 0.0f, out );
	WriteFloat( 0.0f, out );

	WriteFloat( 0.0f, out );
	WriteFloat( 0.0f, out );
}

static DWORD spaces = 0;

void WriteSpaces()
{
	printf( "     " );

	for( DWORD i = 0 ; i < spaces ; i++ )
		printf( " " );

	printf( "+ " );
}

void WriteKeyframer( KEYFRAMER* keyf, FILE* out )
{	
	MESHTRACK*		tmesh;
	CAMERATRACK*	tcamera;	
	LIGHTTRACK*		tlight;
	TARGETTRACK*	ttarget;

	WriteDword( keyf->type, out );
	WriteDword( keyf->flags, out );	
	WriteString( keyf->objectname, out );
	WriteString( keyf->objectname, out );

	WriteSpaces();

	switch( keyf->type )
	{
		case ID_TARGETKEYFRAMER:
		{
			ttarget = (TARGETTRACK*)keyf->track;
			WriteVectorTrack( ttarget->position, out );
			WriteEmptyRotation( out );
			WriteEmptyScale( out );

			printf( "node: %s (target)\n", keyf->objectname );
		}
		break;

		case ID_LIGHTKEYFRAMER:	
		{
			tlight = (LIGHTTRACK*)keyf->track;
			WriteVectorTrack( tlight->position, out );
			WriteEmptyRotation( out );
			WriteEmptyScale( out );

			printf( "node: %s (light)\n", keyf->objectname );
		}
		break;


		case ID_NULLKEYFRAMER:
		{
			tmesh = (MESHTRACK*)keyf->track;
			WriteVectorTrack( tmesh->position, out );
			WriteRotationTrack( tmesh->rotation, out );
			WriteVectorTrack( tmesh->scale, out );

			printf( "node: %s (null)\n", keyf->objectname );
		}
		break;

		case ID_MESHKEYFRAMER:
		{
			tmesh = (MESHTRACK*)keyf->track;
			WriteVectorTrack( tmesh->position, out );
			WriteRotationTrack( tmesh->rotation, out );
			WriteScaleTrack( tmesh->scale, out );

			if( tmesh->morph )			
				WriteMorphTrack( tmesh->morph, out );			

			if( tmesh->hide )			
				WriteHideTrack( tmesh->hide, out );			

			printf( "node: %s (mesh)\n", keyf->objectname );
		}
		break;

		case ID_VIEWERKEYFRAMER:
		{
			tcamera = (CAMERATRACK*)keyf->track;
			WriteVectorTrack( tcamera->position, out );
			WriteEmptyRotation( out );
			WriteEmptyScale( out );

			if( tcamera->roll )
				WriteFloatTrack( tcamera->roll, out );

			if( tcamera->fov )
				WriteFloatTrack( tcamera->fov, out );

			printf( "node: %s (camera)\n", keyf->objectname );
		}
		break;
	}

	if( keyf->child )
	{		
		spaces++;
		WriteKeyframer( keyf->child, out );		
		spaces--;
	}
	
	if( keyf->brother )
		WriteKeyframer( keyf->brother, out );
}

int WriteSCENE( char name[], SCENE* scene, FLOAT scale )
{
	FILE*			out = fopen( name, "wb" );
	OBJECT*			obj;		
	MATERIAL*		mat;	
	DWORD			counter;

	__scale = scale;

	if( !out )	
		return error_file_open;

//	WriteString( "FOX3D", out );
	WriteDword( scene->end - scene->start, out );

	WriteDword( ID_MATERIALBLOCK, out );

	for( obj = scene->materials, counter = 0 ; obj ; obj = obj->next, counter++ );

	WriteDword( counter, out );

	for( obj = scene->materials ; obj ; obj = obj->next )
	{
		mat = (MATERIAL*)obj->obj;
		WriteString( mat->name, out );
		WriteDword( mat->type, out );
		WriteFloat( 0.0f, out );
		WriteFloat( 1.0f, out );
		WriteFloat( 1.0f, out );
		WriteFloat( 1.0f, out );
		WriteString( mat->texture, out );

		if( mat->type&0x1 )
			WriteString( mat->envmap, out );

		printf( "....material %s saved\n", mat->name );
		printf( "	  + texture file name: %s\n", mat->texture );

		if( mat->type&0x1 )
			printf( "	  + enviromnet texture file name: %s\n", mat->envmap );
	}

	WriteDword( ID_OBJECTBLOCK, out );
	

	for( obj = scene->objects, counter = 0 ; obj ; obj = obj->next, counter++ );

	WriteDword( counter, out );
	
	CAMERA*			camera;	
	LIGHT*			light;
	TARGET*			target;

	for( obj = scene->objects ; obj ; obj = obj->next )
	{
		WriteString( obj->name, out );
		WriteDword( obj->type, out );

		switch( obj->type )
		{
			case ID_LIGHT:
			{
				light = (LIGHT*)obj->obj;

				WriteFloat( light->x*__scale, out );
				WriteFloat( light->y*__scale, out );
				WriteFloat( light->z*__scale, out );

				printf( "....light %s saved\n", obj->name );
				printf( "     + position: [%.2f,%.2f,%.2f]\n", light->x, light->y, light->z );
			}
			break;

			case ID_VIEWER:
			{
				camera = (CAMERA*)obj->obj;
				
				WriteFloat( camera->x*__scale, out );
				WriteFloat( camera->y*__scale, out );
				WriteFloat( camera->z*__scale, out );
				WriteFloat( camera->FOV, out );
				WriteFloat( camera->roll, out );

				printf( "....camera %s saved\n", obj->name );
				printf( "     + position: [%.2f,%.2f,%.2f]\n", camera->x, camera->y, camera->z );
				printf( "     + fov: %.2f\n", camera->FOV );
				printf( "     + roll: %.2f\n", camera->roll );
			}
			break;

			case ID_TARGET:
			{
				target = (TARGET*)obj->obj;
				
				WriteString( target->name, out );
				WriteFloat( target->x*__scale, out );
				WriteFloat( target->y*__scale, out );
				WriteFloat( target->z*__scale, out );

				printf( "....target %s saved\n", obj->name );
				printf( "     + position: [%.2f,%.2f,%.2f]\n", target->x, target->y, target->z );
			}
			break;

			case ID_MESH:
			{
				printf( "....mesh %s saved\n", obj->name );
				WriteMesh( (TRIMESH*)obj->obj, out );
			}
			break;
		}
	}

	WriteDword( ID_KEYFRAMERBLOCK, out );	

	printf( "....keyframer tree saved\n" );
	WriteKeyframer( scene->root, out );

	fclose( out );

	return error_ok;
}
