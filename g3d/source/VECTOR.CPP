//	name:	G3D_vector.cpp
//  desc:	implementation file for CVector class

#include "math3d.h"
#include <math.h>

G3D_CVector::G3D_CVector( FLOAT _x, FLOAT _y, FLOAT _z ) : x(_x), y(_y), z(_z)
{
}

G3D_CVector::G3D_CVector( const D3DVECTOR& vec ) : x(vec.x), y(vec.y), z(vec.z)
{
}

G3D_CVector::G3D_CVector( const D3DXVECTOR3& vec ) : x(vec.x), y(vec.y), z(vec.z)
{
}

G3D_CVector::G3D_CVector( const G3D_CVector& vec ) : x(vec.x), y(vec.y), z(vec.z)
{
}

G3D_CVector& G3D_CVector::operator=( const G3D_CVector& vec )
{
	if( &vec == this )		
		return *this;
	
	x = vec.x;
	y = vec.y;
	z = vec.z;	

	return *this;
}

G3D_CVector& G3D_CVector::operator+=( const G3D_CVector& vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;

	return *this;
}

G3D_CVector& G3D_CVector::operator-=( const G3D_CVector& vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;

	return *this;
}

G3D_CVector& G3D_CVector::operator*=( FLOAT a )
{
	x *= a;
	y *= a;
	z *= a;
	
	return *this;
}

G3D_CVector& G3D_CVector::operator*=( const G3D_CMatrix& mtx )
{	
	FLOAT		outx = x*mtx._11 + y*mtx._21 + z*mtx._31 + mtx._41;
	FLOAT		outy = x*mtx._12 + y*mtx._22 + z*mtx._32 + mtx._42;

	
	z = x*mtx._13 + y*mtx._23 + z*mtx._33 + mtx._43;
	x = outx;
	y = outy;

	return *this;
}

G3D_CVector& G3D_CVector::operator/=( FLOAT a ) 
{
	x /= a;
	y /= a;
	z /= a;

	return *this;
}

G3D_CVector G3D_CVector::operator+( const G3D_CVector& vec ) const
{
	return G3D_CVector( x + vec.x, y + vec.y, z + vec.z );
}

G3D_CVector G3D_CVector::operator-( const G3D_CVector& vec ) const
{
	return G3D_CVector( x - vec.x, y - vec.y, z - vec.z );
}

G3D_CVector G3D_CVector::operator*( const G3D_CVector& v ) const
{
	G3D_CVector	out;

	out.x = y*v.z - z*v.y;
	out.y = z*v.x - x*v.z;
	out.z = x*v.y - y*v.x;

	return out;
}

G3D_CVector G3D_CVector::operator*( FLOAT a ) const
{
	return G3D_CVector( a*x, a*y, a*z );
}

G3D_CVector G3D_CVector::operator*( const G3D_CMatrix& mtx ) const
{
	G3D_CVector	out;

	out.x = x*mtx._11 + y*mtx._21 + z*mtx._31 + mtx._41;
	out.y = x*mtx._12 + y*mtx._22 + z*mtx._32 + mtx._42;
	out.z = x*mtx._13 + y*mtx._23 + z*mtx._33 + mtx._43;

	return out;
}

G3D_CVector G3D_CVector::operator/( FLOAT a ) const
{
	return G3D_CVector( x/a, y/a, z/a );
}

G3D_CVector G3D_CVector::operator-() const
{
	return G3D_CVector( -x, -y, -z );
}

G3D_CVector G3D_CVector::operator+() const
{
	return G3D_CVector( x, y, z );
}

G3D_CVector::operator D3DVECTOR() const
{
	D3DVECTOR		out;

	out.x = x;
	out.y = y;
	out.z = z;

	return out;
}

G3D_CVector::operator D3DXVECTOR3() const
{
	return D3DXVECTOR3( x, y, z );
}

G3D_CVector::operator D3DVECTOR*()
{
	return (D3DVECTOR*)&x;
}

G3D_CVector::operator const D3DVECTOR*() const
{
	return (const D3DVECTOR*)&x;
}

G3D_CVector::operator D3DXVECTOR3*() 
{
	return (D3DXVECTOR3*)&x;
}

G3D_CVector::operator const D3DXVECTOR3*() const
{
	return (const D3DXVECTOR3*)&x;
}

G3D_CVector Normalize( const G3D_CVector& vec )
{
	FLOAT			len = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
	G3D_CVector	out;

	len = 1/sqrtf( len );

	out.x = vec.x*len;
	out.y = vec.y*len;
	out.z = vec.z*len;

	return out;
}

FLOAT Length( const G3D_CVector& vec )
{
	return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

G3D_CVector CrossProd( const G3D_CVector& u, const G3D_CVector& v )
{
	G3D_CVector	out;

	out.x = u.y*v.z - u.z*v.y;
	out.y = u.z*v.x - u.x*v.z;
	out.z = u.x*v.y - u.y*v.x;

	return out;
}

FLOAT DotProd( const G3D_CVector& u, const G3D_CVector& v )
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

G3D_CVector operator*( FLOAT a, const G3D_CVector& vec )
{
	return G3D_CVector( a*vec.x, a*vec.y, a*vec.z );
}

G3D_CVector LERP( FLOAT t, const G3D_CVector& u, const G3D_CVector& v )
{
	FLOAT		_1t  = 1 - t;
	
	return G3D_CVector( _1t*u.x + t*v.x, _1t*u.y + t*v.y, _1t*u.z + t*v.z );
}

// test it!
VOID FaceNormal( const G3D_CVector& v1, const G3D_CVector& v2, const G3D_CVector& v3, G3D_CVector& n )
{
	FLOAT	ux = v2.x - v1.x, vx = v3.x - v1.x;
	FLOAT	uy = v2.y - v1.y, vy = v3.y - v1.y;
	FLOAT	uz = v2.z - v1.z, vz = v3.z - v1.z;

	n.x = uy*vz - uz*vy;
	n.y = uz*vx - ux*vz;
	n.z = ux*vy - uy*vx;	
}







