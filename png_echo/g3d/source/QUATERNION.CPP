//	name:	G3D_quaternion.cpp
//  desc:	implementation file for CQuaternion class

#include "math3d.h"
#include <math.h>

G3D_CQuaternion::G3D_CQuaternion( FLOAT _w, FLOAT _x, FLOAT _y, FLOAT _z ) : w(_w), x(_x), y(_y), z(_z)
{
}

G3D_CQuaternion::G3D_CQuaternion( FLOAT _w, const G3D_CVector& vec ) : w(_w), x(vec.x), y(vec.y), z(vec.z)
{	
}

G3D_CQuaternion::G3D_CQuaternion( const D3DXQUATERNION& quat ) : w(quat.w), x(quat.x), y(quat.y), z(quat.z)
{
}

G3D_CQuaternion::G3D_CQuaternion( const G3D_CQuaternion& quat ) : w(quat.w), x(quat.x), y(quat.y), z(quat.z)
{
}

G3D_CQuaternion& G3D_CQuaternion::operator=( const G3D_CQuaternion& quat )
{
	if( &quat == this )
		return *this;

	w = quat.w;
	x = quat.x;
	y = quat.y;
	z = quat.z;

	return *this;
}

G3D_CQuaternion& G3D_CQuaternion::operator*=( FLOAT a )
{
	w *= a;
	x *= a;
	y *= a;
	z *= a;

	return *this;	
}

G3D_CQuaternion& G3D_CQuaternion::operator/=( FLOAT a )
{
	w /= a;
	x /= a;
	y /= a;
	z /= a;

	return *this;	
}

G3D_CQuaternion& G3D_CQuaternion::operator^=( FLOAT a )
{
	FLOAT				ang = a*acosf( w );
	FLOAT				len = sqrtf( x*x + y*y + z*z );	
	FLOAT				_sin = sinf( ang );

	len = _sin/len;

	w = cosf( ang );
	x *= len;
	y *= len;
	z *= len;

	return *this;
}

G3D_CQuaternion& G3D_CQuaternion::operator+=( const G3D_CQuaternion& quat )
{
	w += quat.w;
	x += quat.x;
	y += quat.y;
	z += quat.z;

	return *this;
}

G3D_CQuaternion& G3D_CQuaternion::operator-=( const G3D_CQuaternion& quat )
{
	w -= quat.w;
	x -= quat.x;
	y -= quat.y;
	z -= quat.z;

	return *this;
}

G3D_CQuaternion G3D_CQuaternion::operator+( const G3D_CQuaternion& quat ) const
{
	G3D_CQuaternion	out;

	out.w = w + quat.w;
	out.x = x + quat.x;
	out.y = y + quat.y;
	out.z = z + quat.z;

	return out;
}

G3D_CQuaternion G3D_CQuaternion::operator-( const G3D_CQuaternion& quat ) const
{
	G3D_CQuaternion	out;

	out.w = w - quat.w;
	out.x = x - quat.x;
	out.y = y - quat.y;
	out.z = z - quat.z;

	return out;
}

G3D_CQuaternion G3D_CQuaternion::operator*( const G3D_CQuaternion& q ) const
{
	G3D_CQuaternion	out;

	out.w = w*q.w - x*q.x - y*q.y - z*q.z;
    out.x = w*q.x + x*q.w + y*q.z - z*q.y;
    out.y = w*q.y + y*q.w + z*q.x - x*q.z;
    out.z = w*q.z + z*q.w + x*q.y - y*q.x;

	return out;
}

G3D_CQuaternion G3D_CQuaternion::operator*( FLOAT a ) const
{
	G3D_CQuaternion	out;

	out.w = w*a;
	out.x = x*a;
	out.y = y*a;
	out.z = z*a;

	return out;
}

G3D_CQuaternion G3D_CQuaternion::operator/( FLOAT a ) const
{
	G3D_CQuaternion	out;

	out.w = w/a;
	out.x = x/a;
	out.y = y/a;
	out.z = z/a;

	return out;
}

G3D_CQuaternion G3D_CQuaternion::operator^( FLOAT a ) const
{
	G3D_CQuaternion	out;
	FLOAT				ang = a*acosf( w );
	FLOAT				len = sqrtf( x*x + y*y + z*z );	
	FLOAT				_sin = sinf( ang );

	len = _sin/len;

	out.w = cosf( ang );
	out.x = x*len;
	out.y = y*len;
	out.z = z*len;

	return out;
}

G3D_CQuaternion G3D_CQuaternion::operator-() const
{
	return G3D_CQuaternion( -w, -x, -y, -z );
}

G3D_CQuaternion G3D_CQuaternion::operator+() const
{
	return G3D_CQuaternion( *this );
}

G3D_CQuaternion operator*( FLOAT a, const G3D_CQuaternion& quat )
{
	return G3D_CQuaternion( a*quat.w, a*quat.x, a*quat.y, a*quat.z );
}

FLOAT Length( const G3D_CQuaternion& quat )
{
	return sqrtf( quat.w*quat.w + quat.x*quat.x + quat.y*quat.y + quat.z* quat.z );
}

G3D_CQuaternion Normalize( const G3D_CQuaternion& quat )
{
	G3D_CQuaternion	out;
	FLOAT				len = sqrtf( quat.w*quat.w + quat.x*quat.x + quat.y*quat.y + quat.z* quat.z );

	len = 1/len;

	out.w = quat.w*len;
	out.x = quat.x*len;
	out.y = quat.y*len;
	out.z = quat.z*len;

	return out;
}

G3D_CQuaternion Inverse( const G3D_CQuaternion& quat )
{
	FLOAT				len = sqrtf( quat.w*quat.w + quat.x*quat.x + quat.y*quat.y + quat.z* quat.z );

	len = 1/len;

	return G3D_CQuaternion( quat.w*len, -quat.x*len, -quat.y*len, -quat.z*len );
}

G3D_CQuaternion UnaryInverse( const G3D_CQuaternion& quat )
{
	return G3D_CQuaternion( quat.w, -quat.x, -quat.y, -quat.z );
}

G3D_CQuaternion Conjunction( const G3D_CQuaternion& quat )
{
	return G3D_CQuaternion( quat.w, -quat.x, -quat.y, -quat.z );
}

G3D_CQuaternion ln( const G3D_CQuaternion& quat )
{
	G3D_CQuaternion	out;
	FLOAT				ang = acosf( quat.w );
	FLOAT				_sin = sinf( ang );

	out.w = 0;

	if( ISZERO( _sin ) )
		_sin = 1.0f;
	else		
		_sin = ang/_sin;

	out.x = quat.x*_sin;
	out.y = quat.y*_sin;
	out.z = quat.z*_sin;

	return out;
}

G3D_CQuaternion exp( const G3D_CQuaternion& quat )
{
	G3D_CQuaternion	out;
	FLOAT				ang = sqrtf( quat.x*quat.x + quat.y*quat.y + quat.z*quat.z );		

	out.w = cosf( ang );

	if( ISZERO( ang ) )
		ang = 1;
	else
		ang = sinf( ang )/ang;

	out.x = quat.x*ang;
	out.y = quat.y*ang;
	out.z = quat.z*ang;

	return out;
}

G3D_CQuaternion CrossProd( const G3D_CQuaternion& p, const G3D_CQuaternion& q )
{
	G3D_CQuaternion	out;

	out.w = p.w*q.w - p.x*q.x - p.y*q.y - p.z*q.z;
    out.x = p.w*q.x + p.x*q.w + p.y*q.z - p.z*q.y;
    out.y = p.w*q.y + p.y*q.w + p.z*q.x - p.x*q.z;
    out.z = p.w*q.z + p.z*q.w + p.x*q.y - p.y*q.x;

	return out;
}

FLOAT DotProd( const G3D_CQuaternion& p, const G3D_CQuaternion& q )
{
	return p.w*q.w + p.x*q.x + p.y*q.y + p.z*q.z;
}

G3D_CQuaternion SLERP( FLOAT t, const G3D_CQuaternion& p, const G3D_CQuaternion& q )
{
	FLOAT			ang;
	FLOAT			_sin;
	FLOAT			dot;
	FLOAT			t1, t2;
	
	dot = DotProd( p, q );

	if( dot>1.0f )
		dot = 1.0f;
	
	ang = acosf( dot );
	_sin = sinf( ang );

	if ( ISZERO( _sin ) )
	{
		t1 = 1-t;
		t2 = t;
	}
	else
	{
		_sin = 1.0f/_sin;
		t1 = sinf( ang*( 1-t ) )*_sin;
		t2 = sinf( ang*t )*_sin;
	}

	return t1*p + t2*q;
}

G3D_CQuaternion SQUAD( FLOAT t, const G3D_CQuaternion& p, const G3D_CQuaternion& a, 
								 const G3D_CQuaternion& b, const G3D_CQuaternion& q )
{
	return SLERP(  2*t*(1-t), SLERP( t, p, q ), SLERP( t, a, b ) );
}

G3D_CQuaternion FromAxisAngle( const G3D_CVector& axis, FLOAT angle )
{
	FLOAT				ang = 0.5f*angle;
	FLOAT				_sin = sinf( ang );
	G3D_CQuaternion	out;

	out.w = cosf( ang );
	out.x = _sin*axis.x;
	out.y = _sin*axis.y;
	out.z = _sin*axis.z;

	return out;
}

G3D_CQuaternion FromAxisAngle( FLOAT x, FLOAT y, FLOAT z, FLOAT angle )
{
	FLOAT				ang = 0.5f*angle;
	FLOAT				_sin = sinf( ang );
	G3D_CQuaternion	out;

	out.w = cosf( ang );
	out.x = _sin*x;
	out.y = _sin*y;
	out.z = _sin*z;

	return out;

}