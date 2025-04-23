#include "oglCubes.h"
#include <windows.h>
#include "GLee.h"
#include <GL\GL.h>
#include <cstdio>
#include "GMath.h"
#include "GOpenGL.h"
#include "GScnFile.h"

float
	mode		= 0.0f,
	twirl		= 1.0f,
	ahead		= 1.0f,
	start		= 0.0f,
	end			= 1.0f,
	x			= 0.0f,
	y			= 0.0f,
	z			= 0.0f,
	rotx		= 0.0f,
	roty		= 0.0f,
	rotz		= 0.0f,
	speed		= 1.0f,
	freqxspeed	= 2.0f,
	freqyspeed	= 2.0f,
	freqzspeed	= 2.0f,
	ampspeed	= 5.0f
;


// THX Gargaj
static GParam p[] =
{
	{"mode",		0.0f, 5.0f,	mode,   &mode},
	{"twirl",		0.0f, 0.0f,	twirl,  &twirl},
	{"ahead",		0.0f, 0.0f,	ahead,  &ahead},
	{"start",		0.0f, 0.0f,	start,  &start},
	{"end",			0.0f, 0.0f,	end,	&end},
	{"x",			0.0f, 0.0f,	x,		&x},
	{"y",			0.0f, 0.0f,	y,		&y},
	{"z",			0.0f, 0.0f,	z,		&z},
	{"rotx",		0.0f, 0.0f,	rotx,	&rotx},
	{"roty",		0.0f, 0.0f,	roty,	&roty},
	{"rotz",		0.0f, 0.0f,	rotz,	&rotz},
	{"speed",		0.0f, 0.0f,	speed,  &speed},
	{"freqxspeed",	0.0f, 0.0f,	freqxspeed,  &freqxspeed},
	{"freqyspeed",	0.0f, 0.0f,	freqyspeed,  &freqyspeed},
	{"freqzspeed",	0.0f, 0.0f,	freqzspeed,  &freqzspeed},
	{"ampspeed",	0.0f, 0.0f,	ampspeed,	 &ampspeed}
};

static GCode c[] =
{
	{"mode", "TUNNEL",		0.0f,GCODE_COMBO},
	{"mode", "GENSPHERE",	1.0f,GCODE_COMBO}
};

GPLUGINS_IMPLEMENTATION

struct GRender
{
	std::vector<GVector3f>		vertices;
	std::vector<GVector3f>		normals;
	void Render(void);
	void Create(const float sizex,const float sizey,const float sizez);
};

void GRender::Render(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, &normals[0].x);
	glDrawArrays(GL_QUADS, 0, vertices.size() );
}

static float cubeNormals[72] =
{
	-1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f
};


void GRender::Create(const float sizex,const float sizey,const float sizez)
{
	float x = 1.0f * sizex;
	float y = 1.0f * sizey;
	float z = 1.0f * sizez;
	float cubeVerts[72] = 
	{
		-x, -y,  z, -x,  y,  z, -x,  y, -z, -x, -y, -z,
		 x, -y,  z,  x, -y, -z,  x,  y, -z,  x,  y,  z,
		-x, -y,  z,  x, -y,  z,  x,  y,  z, -x,  y,  z,
		-x, -y, -z, -x,  y, -z,  x,  y, -z,  x, -y, -z,
		-x,  y,  z,  x,  y,  z,  x,  y, -z, -x,  y, -z,
		-x, -y,  z, -x, -y, -z,  x, -y, -z,  x, -y,  z
	};

	for ( int i = 0; i<24; i++ )
	{
		vertices.push_back(GVector3f(cubeVerts[3*i+0],cubeVerts[3*i+1],cubeVerts[3*i+2]));
		normals.push_back(GVector3f(cubeNormals[3*i+0],cubeNormals[3*i+1],cubeNormals[3*i+2]));
	}
}

const float CUBE_SIZE1 = 0.275f;

struct GCube
{ 
	GColor3f	color;
	GColor3f	diff_color;
	GVector3f	position;
	GVector3f	diff_position;
	GVector3f	angle;
};

struct GCubes
{
	GFrustum frustum;
	unsigned int	size;
	bool			rotating;
	std::vector<GCube> cubes;
	GRender cube;
	float radius;
	GVector3f center;
	void Create(const float size)
	{
		cube.Create(size,size,size);
		float c = sqrtf( size * size + size * size );
		radius = sqrtf( size * size + c * c );
		rotating = true;
	}
	virtual void Render(unsigned int time,float fullstart,float fullend);
	virtual void Anim(GCube * c,unsigned int time,unsigned int i) {}
};

void GCubes::Render(unsigned int time,float fullstart,float fullend)
{
	if ( cubes.empty() ) return;
	frustum.Extract();
	float frame = time / 10.0f;
	glPushMatrix();
	glTranslatef(center.x,center.y,center.z);

	if ( fullend >= 1.0f ) fullend = 1.0f;
	if ( fullend <= 0.0f ) fullend = 0.0f;
	if ( fullstart >= 1.0f ) fullstart = 1.0f;
	if ( fullstart <= 0.0f ) fullstart = 0.0f;

	for ( unsigned int i = size * fullstart; i<size * fullend; i++)
	{
		GCube * c   = &cubes[i];
		Anim(c,time,i);
		GVector3f p = c->position + c->diff_position;
		if ( frustum.SphereInFrustum(center + p,radius) )
		{
			glPushMatrix();
			glTranslatef(p.x,p.y,p.z);
			if ( rotating )
			{
				glRotatef(frame + c->angle.x,1.0f,0.0f,0.0f);
				glRotatef(frame + c->angle.y,0.0f,1.0f,0.0f);
				glRotatef(frame + c->angle.z,0.0f,0.0f,1.0f);
			}
			else
			{
				glRotatef(c->angle.x,1.0f,0.0f,0.0f);
				glRotatef(c->angle.y,0.0f,1.0f,0.0f);
				glRotatef(c->angle.z,0.0f,0.0f,1.0f);
			}
			GColor3f color = c->color + c->diff_color;
			glColor3fv(&color.r);
			cube.Render();
			glPopMatrix();
		}
	}
	glPopMatrix();
}

struct GCubeTorusTunnel : public GCubes
{
	void TorusPoint(double r1,double r2,double phi,double psi,int i, int j);
	GCubeTorusTunnel(const double r1, const double r2, const int n1, const int n2);
} * cubetorustunnel = NULL;

void GCubeTorusTunnel::TorusPoint(double r1,double r2,double phi,double psi,int i, int j)
{
	double nx = cos(phi)*cos(psi);
	double ny = sin(psi);
	double nz = sin(phi)*cos(psi);
	GCube c;
	c.position = GVector3f(r1*cos(phi) + r2*nx,r2*ny,r1*sin(phi) + r2*nz);
	
	c.color = GColor3f(sinf(i / 1.5f + nx),cosf(j + ny),0);
	
	// c.angle.x = rand()%360, c.angle.y = rand()%360, c.angle.z = rand()%360;
	cubes.push_back(c);
}

GCubeTorusTunnel::GCubeTorusTunnel(const double r1, const double r2, const int n1, const int n2)
{
	for(int i = 0;i<n1;i++)
	{
		int i2 = (i<n1-1)?(i+1):(0);
		double phi1 = 2*i*G_PIF/n1;
		double phi2 = 2*i2*G_PIF/n1;
		for(int j = 0;j<n2;j++)
		{
			int j2 = (j<n2-1)?(j+1):(0);
			double psi1 = 2*j*G_PIF/n2;
			double psi2 = 2*j2*G_PIF/n2;
			TorusPoint(r1,r2,phi1,psi1,i,j);
		}
	}
	Create(CUBE_SIZE1);
	size = cubes.size();
}

struct GCubeSphere : public GCubes
{
	GCubeSphere(void);
	void Anim(GCube * c, unsigned int time,unsigned int i);
} * cubesphere = NULL;

GCubeSphere::GCubeSphere(void)
{
	#define DETAIL_NUMAROS 40
	#define DETAIL_ARO 40

	float alpha_inc = G_PIF / (float)DETAIL_NUMAROS;
	float theta_inc = 2 * G_PIF / (float)DETAIL_ARO;

	for ( float alpha = - G_PIF / 2; alpha < G_PIF / 2; alpha+=alpha_inc)
	{
		for (float theta = 0; theta < 2 * G_PIF; theta+=theta_inc)
		{
			float nx = cosf(alpha) * sinf(theta);  
			float ny = cosf(alpha) * cosf(theta);
			float nz = -sinf(alpha);
			GCube c;
			c.position	= GVector3f(nx * 6.0f,ny * 6.0f,nz * 6.0f);
			c.color = GColor3f(sin(theta * nx),ny,0);
			// c.angle.x = rand()%360, c.angle.y = rand()%360, c.angle.z = rand()%360;

			cubes.push_back(c);
		}
	}
	Create(CUBE_SIZE1);
	rotating = false;
	size = cubes.size();
}

void GCubeSphere::Anim(GCube * c, unsigned int time,unsigned int i)
{
	float frame = time / 500.0f;
	c->diff_position.z	= sinf( c->position.z + frame ) * cosf( c->position.z + frame ) * 2.0f;
	c->diff_color = GColor3f(c->diff_position.z,c->diff_position.z,c->diff_position.z) /= 4.0f * sin(time/1000.0f);
}

DLLIMPORT bool	Init(GResourceParam * params,unsigned int size)
{
	static bool init = false;
	if ( init ) return init;
	cubesphere		= new GCubeSphere();
	cubetorustunnel = new GCubeTorusTunnel(10.0,5.0,60,35);
	init = true;
	return true;
}

struct GScnShape : public GCubes
{
	GScnShape(scnFile * scn,int color);
};

GScnShape::GScnShape(scnFile * scn,int color)
{
	int count = 0;
	for ( unsigned int i = 0; i<scn->SHAPEOBJECT.size(); i++)
	{
		scnShapeObject * scnO = scn->SHAPEOBJECT[i];
		for ( unsigned int j = 0; j<scnO->SHAPE_LINE.size(); j++)
		{
			scnShapeLine * scnL = scnO->SHAPE_LINE[j];

			for ( unsigned int k = 0; k<scnL->SHAPE_VERTEX.size(); k++)
			{
				scnShapeVertex v = scnL->SHAPE_VERTEX[k];
				GCube c;
				const float SIZE = 5.0f;
				
				c.position	= GVector3f(
					v.VERTEX.x / SIZE,
					v.VERTEX.y / SIZE,
					v.VERTEX.z / SIZE
				);
				if ( color == 0 )
				{
					float col = sin(count / 50.0f) * 0.25f + 0.5f;
					c.color = GColor3f(col,col,col);
				}
				count++;
				c.angle.x = rand()%360, c.angle.y = rand()%360, c.angle.z = rand()%360;
				cubes.push_back(c);
			}
		}
	}
	size = cubes.size();
	Create(0.2f);
}

struct GScnMesh : public GCubes
{
	std::vector<GVector3f> origPos;
	std::vector<GVector3f> origNor;
	GScnMesh(scnFile * scn,int color);
};

GScnMesh::GScnMesh(scnFile * scn,int color)
{
	for ( unsigned int i = 0; i<scn->GEOMOBJECT.size(); i++)
	{
		scnGeomObject * g = scn->GEOMOBJECT[i];
		unsigned int size = g->MESH.MESH_VERTEX_LIST.size();
		if ( i != 0 ) continue;
		for ( unsigned int j = 0; j<size; j++)
		{
			GCube c;
			c.position = g->MESH.MESH_VERTEX_LIST[j] / 10;
			origPos.push_back(c.position);
			if ( !g->MESH.MESH_VERTEXNORMAL.empty() )
				origNor.push_back(g->MESH.MESH_VERTEXNORMAL[j]);
			switch ( color )
			{
				case 0:
					c.color = GColor3f(1.0f,0.5f - cosf(j / 50.0f ) * 0.5f ,0.5f - cosf(j / 50.0f ) * 0.5f);
					break;
				case 1:
					c.color = GColor3f(1.0f,1.0f,1.0f);
					break;
				case 2:
					c.color = GColor3f(0.5f,0.0f,0.0f);
					break;
			}
			c.angle.x = rand()%360, c.angle.y = rand()%360, c.angle.z = rand()%360;
			cubes.push_back(c);
		}
	}
	
	size = cubes.size();
	Create(0.2f);
}

std::vector<GScnMesh*>	storeMesh;
std::vector<GScnShape*>	storeShape;

DLLIMPORT void*	Load(GResourceParam * params,unsigned int size)
{
	const char * filename = "";
	unsigned int i = 0;
	int mode = 0;
	int color = 0;
	for ( i = 0; i<size; i++)
	{
		if ( strcmp(params[i].name,"filename") == 0 ) 
			filename = (const char*)params[i].value;
		if ( strcmp(params[i].name,"mode") == 0 ) 
		{
			if ( std::string((char*)params[i].value) == "0" ) mode = 0;
			if ( std::string((char*)params[i].value) == "1" ) mode = 1;
		}
		if ( strcmp(params[i].name,"color") == 0 ) 
		{
			if ( std::string((char*)params[i].value) == "0" ) color = 0;
			if ( std::string((char*)params[i].value) == "1" ) color = 1;
			if ( std::string((char*)params[i].value) == "2" ) color = 2;
		}
	}

	scnFile * r = new scnFile();

	FILE * file = fopen(filename,"rb");
	if ( file )
	{
		char * result = r->Load(file);
		fclose(file);
		if ( result != NULL ) return NULL;
	}
	else fprintf(stderr,"NOT FOUND");
	
	if ( mode == 0 )
	{
		GScnShape * scnShp = new GScnShape(r,color);
		storeShape.push_back(scnShp);
		delete r;
		return scnShp;
	}
	else if ( mode == 1 )
	{
		GScnMesh * scnMesh = new GScnMesh(r,color);
		delete r;
		storeMesh.push_back(scnMesh);
		return scnMesh;
	}
	return NULL;
}

DLLIMPORT float Render(const float frame,void* id,float * sample)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_COLOR_MATERIAL);
	
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);glColor4f(0.0f, 0.0f, 0.0f, 1.0f); 
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);glColor4f(0.0f, 0.0f, 0.0f, 1.0f); 
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );// glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
	
	if ( mode == 0.0f )
	{
		float subframe = frame * speed;
		glRotatef(subframe,0.0f,0.0f,twirl);
		glTranslatef(10.0f,0.0f,-10.0f );
		glRotatef(subframe,0.0f,ahead,0.0f); 
		cubetorustunnel->Render(subframe,start,end);
	}
	else if ( mode == 1.0f )
	{
		float subframe = frame * speed;
		glTranslatef(0.0f,0.0f,-10.0f + 10 - 10.0f + cosf(subframe / 20.0f ) * 2.0f );
		glRotatef(subframe,0.0f,0.0f,1.0f);
		glRotatef(subframe,0.0f,1.0f,0.0f);
		cubesphere->Render(subframe * 70.0f,start,end);
	}
	else if ( mode == 2.0f && id != NULL )
	{
		float subframe = frame * speed;
		glTranslatef(x,y,z);
		glRotatef(rotx,1.0f,0.0f,0.0f);
		glRotatef(roty,0.0f,1.0f,0.0f);
		glRotatef(rotz,0.0f,0.0f,1.0f);
		glRotatef(-90,1.0f,0.0f,0.0f);
		GScnShape * scnShp = (GScnShape*)id;
		if ( scnShp != NULL )
			scnShp->Render(frame / 100,start,end);
	}
	else if ( mode == 3.0f && id != NULL )
	{
		float subframe = frame * speed;
		glTranslatef(x,y,z);
		glRotatef(-90,1.0f,0.0f,0.0f);
		glRotatef(subframe,1.0f,0.0f,0.0f);
		glRotatef(subframe, 0.0f,1.0f,0.0f);
		glRotatef(subframe,0.0f,0.0f,1.0f);
		GScnMesh * scnMsh = (GScnMesh*)id;
		if ( scnMsh != NULL )
			scnMsh->Render(frame / 100,start,end);
	}
	else if ( mode == 4.0f && id != NULL )
	{
		float subframe = frame * speed;
		glTranslatef(x,y,z);
		glRotatef(50,0.0f,1.0f,0.0f);
		glRotatef(20,1.0f,0.0f,0.0f);
		glRotatef(subframe,0.0f,1.0f,0.0f);
		GScnMesh * scnMsh = (GScnMesh*)id;
		if ( scnMsh != NULL )
			scnMsh->Render(frame / 100,start,end);
	}
	else if ( mode == 5.0f && id != NULL )
	{
		float subframe = frame * speed;
		glTranslatef(x,y,z);
		glRotatef(subframe * 100.0f,1.0f,0.0f,0.0f);
		glRotatef(subframe * 100.0f,0.0f,1.0f,0.0f);
		glRotatef(subframe * 100.0f,0.0f,0.0f,1.0f);
		GScnMesh * scnMsh = (GScnMesh*)id;
		if ( scnMsh != NULL )
		{
			float freqx = 5.0f+sin(subframe)*4.0f;
			float freqy = 5.0f+sin(subframe*1.3f)*4.0f;
			float freqz = 5.0f+sin(subframe*1.1f)*4.0f;
			float amp = 35.0f+sin(subframe*1.4)*30.0f;
			
			freqx *= freqxspeed;
			freqy *= freqyspeed;
			freqz *= freqzspeed;
			amp   *= ampspeed;

			for (int i = 0; i < scnMsh->cubes.size(); i++)
			{
				GVector3f *v = &scnMsh->cubes[i].position;
				GVector3f *o = &scnMsh->origPos[i];
				GVector3f *n = &scnMsh->origNor[i];
				float f = sin(n->x*freqx + subframe) * sin(n->y*freqy + subframe) * sin(n->z*freqz + subframe);
				v->x = o->x + n->x * amp * f;
				v->y = o->y + n->y * amp * f;
				v->z = o->z + n->z * amp * f;
				float color = 0.5f * (f / 2.0f) * 4.0f;
				scnMsh->cubes[i].color = GColor3f(color * 2.0f,color,color);
			}
			scnMsh->Render(frame / 100,start,end);
		}
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();
	return 0.0f;
}

DLLIMPORT void Free(void)
{
	if ( cubesphere != NULL )
	{
		delete cubesphere;
		cubesphere = NULL;
	}
	if ( cubetorustunnel != NULL )
	{
		delete cubetorustunnel;
		cubetorustunnel = NULL;
	}
	
	unsigned int i;
	
	for ( i = 0; i<storeMesh.size(); i++)
	{
		GScnMesh * s = storeMesh[i];
		if ( s == NULL ) continue;
		delete s;
		s = NULL;
	}

	for ( i = 0; i<storeShape.size(); i++)
	{
		GScnShape * s = storeShape[i];
		if ( s == NULL ) continue;
		delete s;
		s = NULL;
	}
}

BOOL APIENTRY DllMain (HINSTANCE hInst, DWORD reason, LPVOID reserved) { return TRUE; }
