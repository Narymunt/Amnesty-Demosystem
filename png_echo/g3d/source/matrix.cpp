#include "math3d.h"
#include <math.h>

//	class:	CMatrix
//  desc:	one float parameter init all matrix fields (default value is 0)

G3D_CMatrix::G3D_CMatrix( FLOAT val  ) : _11( val ), _12( val ), _13( val ), _14( val ),
										   _21( val ), _22( val ), _23( val ), _24( val ),
										   _31( val ), _32( val ), _33( val ), _34( val ),
										   _41( val ), _42( val ), _43( val ), _44( val )
{
 
}

//	class:	CMatrix
//  desc:	construct matrix from D3DXMATRIX struct

G3D_CMatrix::G3D_CMatrix( const D3DMATRIX& d3dmat )
{
	_11 = d3dmat._11; _12 = d3dmat._12; _13 = d3dmat._13; _14 = d3dmat._14;
	_21 = d3dmat._11; _22 = d3dmat._22; _23 = d3dmat._13; _24 = d3dmat._24;
	_31 = d3dmat._11; _32 = d3dmat._32; _33 = d3dmat._13; _34 = d3dmat._34;
	_41 = d3dmat._11; _42 = d3dmat._42; _43 = d3dmat._13; _44 = d3dmat._44;
}

//	class:	CMatrix
//  desc:	construct matrix from D3DXMATRIX struct

G3D_CMatrix::G3D_CMatrix( const D3DXMATRIX& d3dxmat )
{
	_11 = d3dxmat._11; _12 = d3dxmat._12; _13 = d3dxmat._13; _14 = d3dxmat._14;
	_21 = d3dxmat._21; _22 = d3dxmat._22; _23 = d3dxmat._23; _24 = d3dxmat._24;
	_31 = d3dxmat._31; _32 = d3dxmat._32; _33 = d3dxmat._33; _34 = d3dxmat._34;
	_41 = d3dxmat._41; _42 = d3dxmat._42; _43 = d3dxmat._43; _44 = d3dxmat._44;	
}

//	class:	CMatrix
//  desc:	copy constructor

G3D_CMatrix::G3D_CMatrix( const G3D_CMatrix& mtx ) : _11( mtx._11 ), _12( mtx._12 ), _13( mtx._13 ), _14( mtx._14 ),
														_21( mtx._21 ), _22( mtx._22 ), _23( mtx._23 ), _24( mtx._24 ),
														_31( mtx._31 ), _32( mtx._32 ), _33( mtx._33 ), _34( mtx._34 ),
														_41( mtx._41 ), _42( mtx._42 ), _43( mtx._43 ), _44( mtx._44 )
{
}

G3D_CMatrix::G3D_CMatrix( const FLOAT* pfmtx )
{
	FLOAT*		mtx = (FLOAT*)(&pfmtx[0]);

	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++, mtx++ )
			m[i][j] = *mtx;
}

G3D_CMatrix& G3D_CMatrix::operator=( const G3D_CMatrix& mtx )
{
	if( &mtx == this )
		return *this;

	_11 = mtx._11; _12 = mtx._12; _13 = mtx._13; _14 = mtx._14;
	_21 = mtx._21; _22 = mtx._22; _23 = mtx._23; _24 = mtx._24;
	_31 = mtx._31; _32 = mtx._32; _33 = mtx._33; _34 = mtx._34;
	_41 = mtx._41; _42 = mtx._42; _43 = mtx._43; _44 = mtx._44;

	return *this;
}

G3D_CMatrix& G3D_CMatrix::operator+=( const G3D_CMatrix& mtx )
{
	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			m[i][j] += mtx.m[i][j];

	return *this;
}

G3D_CMatrix& G3D_CMatrix::operator-=( const G3D_CMatrix& mtx )
{
	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			m[i][j] -= mtx.m[i][j];

	return *this;
}

G3D_CMatrix& G3D_CMatrix::operator*=( const G3D_CMatrix& mtx )
{	
	FLOAT		o1, o2, o3, o4;

	o1 = _11*mtx._11 + _12*mtx._21 + _13*mtx._31 + _14*mtx._41;
	o2 = _11*mtx._12 + _12*mtx._22 + _13*mtx._32 + _14*mtx._42;
	o3 = _11*mtx._13 + _12*mtx._23 + _13*mtx._33 + _14*mtx._43;
	o4 = _11*mtx._14 + _12*mtx._24 + _13*mtx._34 + _14*mtx._44;

	_11 = o1; _12 = o2; _13 = o3; _14 = o4;

	o1 = _21*mtx._11 + _22*mtx._21 + _23*mtx._31 + _24*mtx._41;
	o2 = _21*mtx._12 + _22*mtx._22 + _23*mtx._32 + _24*mtx._42;
	o3 = _21*mtx._13 + _22*mtx._23 + _23*mtx._33 + _24*mtx._43;
	o4 = _21*mtx._14 + _22*mtx._24 + _23*mtx._34 + _24*mtx._44;

	_21 = o1; _22 = o2; _23 = o3; _24 = o4;

	o1 = _31*mtx._11 + _32*mtx._21 + _33*mtx._31 + _34*mtx._41;
	o2 = _31*mtx._12 + _32*mtx._22 + _33*mtx._32 + _34*mtx._42;
	o3 = _31*mtx._13 + _32*mtx._23 + _33*mtx._33 + _34*mtx._43;
	o4 = _31*mtx._14 + _32*mtx._24 + _33*mtx._34 + _34*mtx._44;

	_31 = o1; _32 = o2; _33 = o3; _34 = o4;

	o1 = _41*mtx._11 + _42*mtx._21 + _43*mtx._31 + _44*mtx._41;
	o2 = _41*mtx._12 + _42*mtx._22 + _43*mtx._32 + _44*mtx._42;
	o3 = _41*mtx._13 + _42*mtx._23 + _43*mtx._33 + _44*mtx._43;
	o4 = _41*mtx._14 + _42*mtx._24 + _43*mtx._34 + _44*mtx._44;	

	_41 = o1; _42 = o2; _43 = o3; _44 = o4;

	return *this;
}

G3D_CMatrix& G3D_CMatrix::operator*=( FLOAT a )
{
	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			m[i][j] *= a;

	return *this;	
}

G3D_CMatrix& G3D_CMatrix::operator/=( FLOAT a )
{
	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			m[i][j] /= a;

	return *this;	
}

G3D_CMatrix G3D_CMatrix::operator+( const G3D_CMatrix& mtx ) const
{
	G3D_CMatrix	out;

	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			out.m[i][j] = m[i][j] + mtx.m[i][j];

	return out;
}

G3D_CMatrix G3D_CMatrix::operator-( const G3D_CMatrix& mtx ) const
{
	G3D_CMatrix	out;

	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			out.m[i][j] = m[i][j] - mtx.m[i][j];

	return out;
}

G3D_CMatrix G3D_CMatrix::operator*( const G3D_CMatrix& mtx ) const
{
	G3D_CMatrix	out;

	out._11 = _11*mtx._11 + _12*mtx._21 + _13*mtx._31 + _14*mtx._41;
	out._12 = _11*mtx._12 + _12*mtx._22 + _13*mtx._32 + _14*mtx._42;
	out._13 = _11*mtx._13 + _12*mtx._23 + _13*mtx._33 + _14*mtx._43;
	out._14 = _11*mtx._14 + _12*mtx._24 + _13*mtx._34 + _14*mtx._44;

	out._21 = _21*mtx._11 + _22*mtx._21 + _23*mtx._31 + _24*mtx._41;
	out._22 = _21*mtx._12 + _22*mtx._22 + _23*mtx._32 + _24*mtx._42;
	out._23 = _21*mtx._13 + _22*mtx._23 + _23*mtx._33 + _24*mtx._43;
	out._24 = _21*mtx._14 + _22*mtx._24 + _23*mtx._34 + _24*mtx._44;

	out._31 = _31*mtx._11 + _32*mtx._21 + _33*mtx._31 + _34*mtx._41;
	out._32 = _31*mtx._12 + _32*mtx._22 + _33*mtx._32 + _34*mtx._42;
	out._33 = _31*mtx._13 + _32*mtx._23 + _33*mtx._33 + _34*mtx._43;
	out._34 = _31*mtx._14 + _32*mtx._24 + _33*mtx._34 + _34*mtx._44;

	out._41 = _41*mtx._11 + _42*mtx._21 + _43*mtx._31 + _44*mtx._41;
	out._42 = _41*mtx._12 + _42*mtx._22 + _43*mtx._32 + _44*mtx._42;
	out._43 = _41*mtx._13 + _42*mtx._23 + _43*mtx._33 + _44*mtx._43;
	out._44 = _41*mtx._14 + _42*mtx._24 + _43*mtx._34 + _44*mtx._44;

	return out;
}

G3D_CMatrix G3D_CMatrix::operator*( FLOAT a ) const
{
	G3D_CMatrix	out;

	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			out.m[i][j] = a*m[i][j];

	return out;	
}

G3D_CMatrix G3D_CMatrix::operator/( FLOAT a ) const
{
	G3D_CMatrix	out;

	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			out.m[i][j] = m[i][j]/a;

	return out;	
}

G3D_CMatrix G3D_CMatrix::operator-() const
{
	G3D_CMatrix	out;

	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			out.m[i][j] = -m[i][j];

	return out;	
}

G3D_CMatrix G3D_CMatrix::operator+() const
{
	return G3D_CMatrix( *this );
}

FLOAT& G3D_CMatrix::operator()( INT i, INT j )
{
	return m[i-1][j-1];
}

G3D_CMatrix::operator D3DMATRIX() const
{
	D3DMATRIX		d3dmat;

	d3dmat._11 = _11; d3dmat._12 = _12; d3dmat._13 = _13; d3dmat._14 = _14; 
	d3dmat._21 = _21; d3dmat._22 = _22; d3dmat._23 = _23; d3dmat._24 = _24; 
	d3dmat._31 = _31; d3dmat._32 = _32; d3dmat._33 = _33; d3dmat._34 = _34; 
	d3dmat._41 = _41; d3dmat._42 = _42; d3dmat._43 = _43; d3dmat._44 = _44; 

	return d3dmat;
}

G3D_CMatrix::operator D3DXMATRIX() const
{
	D3DXMATRIX		d3dxmat;

	d3dxmat._11 = _11; d3dxmat._12 = _12; d3dxmat._13 = _13; d3dxmat._14 = _14; 
	d3dxmat._21 = _21; d3dxmat._22 = _22; d3dxmat._23 = _23; d3dxmat._24 = _24; 
	d3dxmat._31 = _31; d3dxmat._32 = _32; d3dxmat._33 = _33; d3dxmat._34 = _34; 
	d3dxmat._41 = _41; d3dxmat._42 = _42; d3dxmat._34 = _43; d3dxmat._44 = _44; 

	return d3dxmat;
}

G3D_CMatrix::operator D3DMATRIX*()
{
	return (D3DMATRIX*)(&m[0][0]);
}

G3D_CMatrix::operator const D3DMATRIX*() const
{
	return (const D3DMATRIX*)(&m[0][0]);
}

G3D_CMatrix::operator D3DXMATRIX*()
{
	return (D3DXMATRIX*)(&m[0][0]);
}

G3D_CMatrix::operator const D3DXMATRIX*() const
{
	return (const D3DXMATRIX*)(&m[0][0]);
}

G3D_CMatrix ProjectionMtx( FLOAT fov, FLOAT aspect, FLOAT zMin, FLOAT zMax )
{
	FLOAT			ang = 0.5f*fov*3.14159f/180.0f;			
	FLOAT			_ctg = 1.0f/tanf( ang );

	FLOAT			w = aspect * _ctg;
	FLOAT			h = _ctg;  
    FLOAT			Q = zMax/(zMax - zMin );

	G3D_CMatrix	out;
    
    out._11 = w;
    out._22 = h;
    out._33 = Q;
    out._34 = 1.0f;
    out._43 = -Q*zMin;

	return out;
}

G3D_CMatrix CameraMtx( const G3D_CVector& pos, const G3D_CVector& trg, FLOAT roll )
{
	
	G3D_CVector	dir = trg - pos;
	FLOAT			focus = Length( dir );

	FLOAT			alpha = -atan2f(dir.x,dir.z);
	FLOAT			beta  = asinf(dir.y/focus);
	FLOAT			gamma = -roll*3.14159f/180.0f;

	FLOAT			sina = sinf(alpha); 
	FLOAT			cosa = cosf(alpha);
	FLOAT			sinb = sinf(beta);  
	FLOAT			cosb = cosf(beta);
	FLOAT			sing = sinf(gamma); 
	FLOAT			cosg = cosf(gamma);

	G3D_CMatrix	out;

	out._11 = sina*sinb*sing + cosa*cosg;
	out._21 = cosb*sing;
	out._31 = sina*cosg - cosa*sinb*sing;

	out._12 = sina*sinb*cosg - cosa*sing;
	out._22 = cosb*cosg;
    out._32 = -cosa*sinb*cosg - sina*sing;

	out._13 = -sina*cosb;
    out._23 = sinb;
    out._33 = cosa*cosb;

	out._44 = 1.0f;

	out = TranslationMtx( -pos )*out;

    return out;
}

G3D_CMatrix XRotationMtx( FLOAT ang )
{
	G3D_CMatrix	out = IdentMtx();

	out._22 = cosf(ang);
	out._23 = sinf(ang);
	out._32 = -sinf(ang);
	out._33 = cosf(ang);

	return out;
}

G3D_CMatrix YRotationMtx( FLOAT ang )
{
	G3D_CMatrix	out = IdentMtx();

	out._11 = cosf( ang );
	out._13 = -sinf( ang );
	out._31 = sinf( ang );
	out._33 = cosf( ang );

	return out;
}

G3D_CMatrix ZRotationMtx( FLOAT ang )
{
	G3D_CMatrix	out = IdentMtx();

	out._11 = cosf( ang );
	out._12 = sinf( ang );
	out._21 = -sinf( ang );
	out._22 = cosf( ang );

	return out;
}

G3D_CMatrix RotationMtx( FLOAT alpha, FLOAT beta, FLOAT gamma )
{	
	FLOAT			cosa = cosf( alpha ), sina = sinf( alpha );
	FLOAT			cosb = cosf( beta ),  sinb = sinf( beta );
	FLOAT			cosg = cosf( gamma ), sing = sinf( gamma );

	G3D_CMatrix	out;

	//out._11 = sina*sinb*sing + cosa*cosg;
	//out._21 = cosb*sing;
	//out._31 = sina*cosg - cosa*sinb*sing;

	//out._12 = sina*sinb*cosg - cosa*sing;
	//out._22 = cosb*cosg;
	//out._32 = -cosa*sinb*cosg - sina*sing;

    //out._13 = -sina*cosb;
	//out._23 = sinb;
	//out._33 = cosa*cosb;	

	//out._11 = cosb*cosg;
	//out._12 = -cosb*sing;

	out._11	= cosb*cosg;
	out._12	= cosb*sing;
	out._13	= -sinb;	

	out._21 = sina*sinb*cosg - cosa*sing;
	out._22 = sina*sinb*sing + cosa*cosg;
	out._23 = sina*cosb;

	out._31 = cosa*sinb*cosg + sina*sing;
	out._32 = cosa*sinb*sing - sina*cosg;
	out._33 = cosa*cosb;

	out._44 = 1.0f;

    return out;

}

G3D_CMatrix RotationMtx( const G3D_CVector& ang  )
{	
	/*FLOAT			cosa = cosf( ang.x ), sina = sinf( ang.x );
	FLOAT			cosb = cosf( ang.y ), sinb = sinf( ang.y );	
	FLOAT			cosg = cosf( ang.z ), sing = sinf( ang.z );	

	G3D_CMatrix	out;

	out._11	= cosb*cosg;
	out._12	= cosb*sing;
	out._13	= -sinb;	

	out._21 = sina*sinb*cosg - cosa*sing;
	out._22 = sina*sinb*sing + cosa*cosg;
	out._23 = sina*cosb;

	out._31 = cosa*sinb*cosg + sina*sing;
	out._32 = cosa*sinb*sing - sina*cosg;
	out._33 = cosa*cosb;

	out._44 = 1.0f;*/

    return ZRotationMtx( ang.z )*XRotationMtx( ang.x )*YRotationMtx( ang.y );

}

G3D_CMatrix RotationMtx( const G3D_CVector& axis, FLOAT ang )
{		
	FLOAT			cost	= cosf( ang );
	FLOAT			_1cost	= 1-cost;
	FLOAT			sint	= sinf( ang );
	FLOAT			x		= axis.x;
	FLOAT			y		= axis.y;
	FLOAT			z		= axis.z;
	FLOAT			x2		= x*x;
	FLOAT			y2		= y*y;
	FLOAT			z2		= z*z;
	FLOAT			xy		= x*y;
	FLOAT			xz		= x*z;
	FLOAT			yz		= y*z;

	G3D_CMatrix	out;

	out._11 = x2 + cost*(1 - x2);
	out._12 = xy*_1cost + z*sint;
	out._13 = xz*_1cost - y*sint;

	out._21 = xy*_1cost - z*sint;
	out._22 = y2 + cost*(1 - y2);
	out._23 = yz*_1cost + x*sint;

	out._31 = xz*_1cost - y*sint;
	out._32 = yz*_1cost - x*sint;	
	out._33 = z2 + cost*(1 - z2);

	out._44 = 1.0f;
    
	return out;
}

G3D_CMatrix RotationMtx( const G3D_CQuaternion& quat )
{
	FLOAT			xx = quat.x*quat.x; 
	FLOAT			yy = quat.y*quat.y; 
	FLOAT			zz = quat.z*quat.z;
    FLOAT			xy = quat.x*quat.y; 
	FLOAT			xz = quat.x*quat.z; 
	FLOAT			yz = quat.y*quat.z;
    FLOAT			wx = quat.w*quat.x; 
	FLOAT			wy = quat.w*quat.y; 
	FLOAT			wz = quat.w*quat.z;

	G3D_CMatrix	out;
    
    out._11 = 1 - 2 * ( yy + zz ); 
    out._12 =     2 * ( xy - wz );
    out._13 =     2 * ( xz + wy );

    out._21 =     2 * ( xy + wz );
    out._22 = 1 - 2 * ( xx + zz );
    out._23 =     2 * ( yz - wx );

    out._31 =     2 * ( xz - wy );
    out._32 =     2 * ( yz + wx );
    out._33 = 1 - 2 * ( xx + yy );

	out._44 = 1.0f;

	return out;
}

G3D_CMatrix TranslationMtx( const G3D_CVector& vec )
{
	G3D_CMatrix	out = IdentMtx();

	out._41 = vec.x;
	out._42 = vec.y;
	out._43 = vec.z;

	return out;
}

G3D_CMatrix TranslationMtx( FLOAT x, FLOAT y, FLOAT z )
{
	G3D_CMatrix	out = IdentMtx();

	out._41 = x;
	out._42 = y;
	out._43 = z;

	return out;
}

G3D_CMatrix ScaleMtx( const G3D_CVector& vec )
{
	G3D_CMatrix	out;

	out._11 = vec.x;
	out._22 = vec.y;
	out._33 = vec.z;
	out._44 = 1.0f;

	return out;
}

G3D_CMatrix ScaleMtx( FLOAT sx, FLOAT sy, FLOAT sz )
{
	G3D_CMatrix	out;

	out._11 = sx;
	out._22 = sy;
	out._33 = sz;
	out._44 = 1.0f;

	return out;
}

G3D_CMatrix ScaleMtx( FLOAT scale )
{
	G3D_CMatrix	out;

	out._11 = scale;
	out._22 = scale;
	out._33 = scale;
	out._44 = 1.0f;

	return out;
}

G3D_CMatrix IdentMtx()
{
	G3D_CMatrix	out;

	out._11 = 1.0f;
	out._22 = 1.0f;
	out._33 = 1.0f;
	out._44 = 1.0f;

	return out;
}

G3D_CMatrix ZeroMtx()
{
	return G3D_CMatrix();
}


G3D_CMatrix TransposeMtx( const G3D_CMatrix& mtx )
{
	G3D_CMatrix	out;

	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			out.m[j][i] = mtx.m[i][j];

	return out;
}


G3D_CMatrix GaussInverseMtx( const G3D_CMatrix& mtx )
{
	G3D_CMatrix	src = mtx;
	G3D_CMatrix	out = IdentMtx();

	FLOAT			tmp;

	INT				selRow;
	INT				i, j, k;

	for( i = 1 ; i <= 4 ; i++ )
	{	
		for( j = i ; j <= 4 ; j++ )
		{
			if( src( j, i ) != 0 )
				selRow = j;
		}

		// swap rows if necessary
		if( selRow != i )
		{
			for( k = 1 ; k <= 4 ; k++ )
			{
				tmp = src( selRow, k );
				src( selRow, k ) = src( i, k );
				src( i, k ) = tmp;

				tmp = out( selRow, k );
				out( selRow, k ) = out( i, k );
				out( i, k ) = tmp;
			}
		}

		for( j = 1 ; j <= 4 ; j++ )
		{
			if( j != i )
			{
				tmp = src( j, i )/src( i, i );

				for( k = 1 ; k <= 4 ; k++ )
				{
					src( j, k ) = src( j, k ) - src( i, k )*tmp;
					out( j, k ) = out( j, k ) - out( i, k )*tmp;
				}
			}
		}

		tmp = src( i, i );

		for( k = 1 ; k <= 4 ; k++ )
		{
			src( i, k ) = src( j, k )/tmp;
			out( i, k ) = src( j, k )/tmp;
		}
	}

	return out;
}

G3D_CMatrix InverseMtx( const G3D_CMatrix& mtx )
{
	G3D_CMatrix	out;

    FLOAT fDetInv = 1.0f / ( mtx._11*( mtx._22*mtx._33 - mtx._23*mtx._32 ) -
                             mtx._12*( mtx._21*mtx._33 - mtx._23*mtx._31 ) +
                             mtx._13*( mtx._21*mtx._32 - mtx._22*mtx._31 ) );

    out._11 =  fDetInv*( mtx._22*mtx._33 - mtx._23*mtx._32 );
    out._12 = -fDetInv*( mtx._12*mtx._33 - mtx._13*mtx._32 );
    out._13 =  fDetInv*( mtx._12*mtx._23 - mtx._13*mtx._22 );
    out._14 = 0.0f;

    out._21 = -fDetInv*( mtx._21*mtx._33 - mtx._23*mtx._31 );
    out._22 =  fDetInv*( mtx._11*mtx._33 - mtx._13*mtx._31 );
    out._23 = -fDetInv*( mtx._11*mtx._23 - mtx._13*mtx._21 );
    out._24 = 0.0f;

    out._31 =  fDetInv*( mtx._21*mtx._32 - mtx._22*mtx._31 );
    out._32 = -fDetInv*( mtx._11*mtx._32 - mtx._12*mtx._31 );
    out._33 =  fDetInv*( mtx._11*mtx._22 - mtx._12*mtx._21 );
    out._34 = 0.0f;

    out._41 = -( mtx._41*out._11 + mtx._42*out._21 + mtx._43*out._31 );
    out._42 = -( mtx._41*out._12 + mtx._42*out._22 + mtx._43*out._32 );
    out._43 = -( mtx._41*out._13 + mtx._42*out._23 + mtx._43*out._33 );
    out._44 = 1.0f;

    return out;
}

G3D_CMatrix InverseCameraMtx( const G3D_CMatrix& mtx )
{
	G3D_CMatrix	out;

    FLOAT fDetInv = 1.0f / ( mtx._11*( mtx._22*mtx._33 - mtx._23*mtx._32 ) -
                             mtx._12*( mtx._21*mtx._33 - mtx._23*mtx._31 ) +
                             mtx._13*( mtx._21*mtx._32 - mtx._22*mtx._31 ) );

    out._11 =  fDetInv*( mtx._22*mtx._33 - mtx._23*mtx._32 );
    out._12 = -fDetInv*( mtx._12*mtx._33 - mtx._13*mtx._32 );
    out._13 =  fDetInv*( mtx._12*mtx._23 - mtx._13*mtx._22 );
    out._14 = 0.0f;

    out._21 = -fDetInv*( mtx._21*mtx._33 - mtx._23*mtx._31 );
    out._22 =  fDetInv*( mtx._11*mtx._33 - mtx._13*mtx._31 );
    out._23 = -fDetInv*( mtx._11*mtx._23 - mtx._13*mtx._21 );
    out._24 = 0.0f;

    out._31 =  fDetInv*( mtx._21*mtx._32 - mtx._22*mtx._31 );
    out._32 = -fDetInv*( mtx._11*mtx._32 - mtx._12*mtx._31 );
    out._33 =  fDetInv*( mtx._11*mtx._22 - mtx._12*mtx._21 );
    out._34 = 0.0f;

    out._41 = 0.0f;
    out._42 = 0.0f;
    out._43 = 0.0f;
    out._44 = 1.0f;

	return out;
}

G3D_CMatrix operator*( FLOAT a, const G3D_CMatrix& mtx )
{
	G3D_CMatrix	out;

	for( INT i = 0 ; i < 4 ; i++ )
		for( INT j = 0 ; j < 4 ; j++ )
			out.m[i][j] = a*mtx.m[i][j];

	return out;	
}








