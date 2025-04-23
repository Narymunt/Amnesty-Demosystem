//	name:	G3D_math.h
//  desc:	header for helium3d math util lib including matrix, vector and quaternion classes

#ifndef __G3D_MATH_H_
#define __G3D_MATH_H_

#include <d3dx8.h>

#define	H_2PI			6.283185307f
#define H_PI			3.141592654f
#define H_PI2			1.570796327f

#define RAD2DEG(x)		( (x)*180.0f )/H_PI
#define DEG2RAD(x)		( (x)*H_PI )/180.0f

#define H_EPSILON		0.001f

#define	ISZERO(x)		( fabs(x) <= H_EPSILON )


class G3D_CVector;
class G3D_CQuaternion;


//=G3D=class===========================================================================================
//	Helium3d lib	(c) 2000 acid.z51 
//------------------------------------------------------------------------------------------------------
//
//	name:	CMatrix
//	desc:	representation of commonly use 4x4 matrices
//
//======================================================================================================
class G3D_CMatrix
{
public:

	union
	{
		struct
		{
			FLOAT	_11, _12, _13, _14;
			FLOAT	_21, _22, _23, _24;
			FLOAT	_31, _32, _33, _34;
			FLOAT	_41, _42, _43, _44;
		};

		FLOAT	m[4][4];
	};

public:

	G3D_CMatrix( FLOAT val = 0.0f );
	G3D_CMatrix( const D3DMATRIX& d3dmat );
	G3D_CMatrix( const D3DXMATRIX& d3dxmat );
	G3D_CMatrix( const G3D_CMatrix& mtx );
	G3D_CMatrix( const FLOAT* pfmtx );

	// assigment operator
	G3D_CMatrix& operator=( const G3D_CMatrix& mtx );
	G3D_CMatrix& operator+=( const G3D_CMatrix& mtx );
	G3D_CMatrix& operator*=( const G3D_CMatrix& mtx );	
	G3D_CMatrix& operator*=( FLOAT a );
	G3D_CMatrix& operator/=( FLOAT a );	
	G3D_CMatrix& operator-=( const G3D_CMatrix& mtx );

	// arithmetic operators
	G3D_CMatrix operator*( const G3D_CMatrix& mtx ) const;
	G3D_CMatrix operator*( FLOAT a ) const;	
	G3D_CMatrix operator/( FLOAT a ) const;
	G3D_CMatrix operator+( const G3D_CMatrix& mtx ) const;
	G3D_CMatrix operator-( const G3D_CMatrix& mtx ) const;

	// unary operators
	G3D_CMatrix operator+() const;
	G3D_CMatrix operator-() const;

	// selector	
	FLOAT& operator()( INT i, INT j );

	// typecast operators
	operator D3DMATRIX() const;
	operator D3DXMATRIX() const;

	operator D3DMATRIX*();
	operator const D3DMATRIX*() const;	

	operator D3DXMATRIX*();
	operator const D3DXMATRIX*() const;		

	// friend function represent common matrix and other operation

	friend G3D_CMatrix ProjectionMtx( FLOAT fov, FLOAT aspect, FLOAT zMin, FLOAT zMax );
	friend G3D_CMatrix CameraMtx( const G3D_CVector& pos, const G3D_CVector& trg, FLOAT roll );
	//friend G3D_CMatrix EulerCameraMtx( FLOAT yaw, FLOAT pitch, FLOAT roll );
	friend G3D_CMatrix XRotationMtx( FLOAT ang );
	friend G3D_CMatrix YRotationMtx( FLOAT ang );
	friend G3D_CMatrix ZRotationMtx( FLOAT ang );
	friend G3D_CMatrix RotationMtx( FLOAT alpha, FLOAT beta, FLOAT gamma );
	friend G3D_CMatrix RotationMtx( const G3D_CVector& ang );
	friend G3D_CMatrix RotationMtx( const G3D_CVector& axis, FLOAT ang );
	friend G3D_CMatrix RotationMtx( const G3D_CQuaternion& quat );
	friend G3D_CMatrix TranslationMtx( const G3D_CVector& vec );
	friend G3D_CMatrix TranslationMtx( FLOAT x, FLOAT y, FLOAT z );
	friend G3D_CMatrix ScaleMtx( const G3D_CVector& vec );
	friend G3D_CMatrix ScaleMtx( FLOAT sx, FLOAT sy, FLOAT sz );
	friend G3D_CMatrix ScaleMtx( FLOAT scale );
	friend G3D_CMatrix IdentMtx();
	friend G3D_CMatrix ZeroMtx();
	friend G3D_CMatrix InverseMtx( const G3D_CMatrix& mtx );
	friend G3D_CMatrix InverseCameraMtx( const G3D_CMatrix& mtx );
	friend G3D_CMatrix GaussInverseMtx( const G3D_CMatrix& mtx );
	friend G3D_CMatrix TransposeMtx( const G3D_CMatrix& mtx );
	friend G3D_CMatrix operator*( FLOAT a, const G3D_CMatrix& mtx );
};


//=G3D=class===========================================================================================
//	Helium3d lib	(c) 2000 acid.z51 
//------------------------------------------------------------------------------------------------------
//
//	name:	CVector
//	desc:	3 elements vector representation
//
//======================================================================================================
class G3D_CVector
{
public:

	FLOAT		x, y, z;

public:

	G3D_CVector( FLOAT _x = 0.0f, FLOAT _y = 0.0f, FLOAT _z = 0.0f );
	G3D_CVector( const D3DVECTOR& vec );
	G3D_CVector( const D3DXVECTOR3& vec );
	G3D_CVector( const G3D_CVector& vec );

	// assigment operators
	G3D_CVector& operator=( const G3D_CVector& vec );
	G3D_CVector& operator+=( const G3D_CVector& vec );
	G3D_CVector& operator-=( const G3D_CVector& vec );
	G3D_CVector& operator*=( FLOAT a );
	G3D_CVector& operator/=( FLOAT a );
	G3D_CVector& operator*=( const G3D_CMatrix& mtx );

	// arithmetic operator
	G3D_CVector operator+( const G3D_CVector& vec ) const;
	G3D_CVector operator-( const G3D_CVector& vec ) const;
	G3D_CVector operator*( FLOAT a ) const;
	G3D_CVector operator*( const G3D_CVector& v ) const;
	G3D_CVector operator*( const G3D_CMatrix& mtx ) const;
	G3D_CVector operator/( FLOAT a ) const;

	// unary operator
	G3D_CVector operator+() const;
	G3D_CVector operator-() const;

	//typecast operator
	operator D3DVECTOR() const;
	operator D3DXVECTOR3() const;

	operator D3DVECTOR*();
	operator const D3DVECTOR*() const;

	operator D3DXVECTOR3*();
	operator const D3DXVECTOR3*() const;

	//friend function
	friend FLOAT Length( const G3D_CVector& vec );
	friend G3D_CVector Normalize( const G3D_CVector& vec );
	friend G3D_CVector CrossProd( const G3D_CVector& u, const G3D_CVector& v );
	friend FLOAT DotProd( const G3D_CVector& u, const G3D_CVector& v );
	friend G3D_CVector LERP( FLOAT t, const G3D_CVector& u, const G3D_CVector& v );
	friend G3D_CVector operator*( FLOAT a, const G3D_CVector& vec );
	friend VOID FaceNormal( const G3D_CVector& v1, const G3D_CVector& v2, const G3D_CVector& v3, G3D_CVector& n );
};

//=G3D=class===========================================================================================
//	Helium3d lib	(c) 2000 acid.z51 
//------------------------------------------------------------------------------------------------------
//
//	name:	CQuaternion
//	desc:	quaternion representation
//
//======================================================================================================
class G3D_CQuaternion
{
public:

	FLOAT		w, x, y, z;

public:

	G3D_CQuaternion( FLOAT _w = 0.0f, FLOAT _x = 0.0f, FLOAT _y = 0.0f, FLOAT _z = 0.0f );
	G3D_CQuaternion( FLOAT _w, const G3D_CVector& vec );
	G3D_CQuaternion( const D3DXQUATERNION& quat );
	G3D_CQuaternion( const G3D_CQuaternion& quat );

	// assigment operators
	G3D_CQuaternion& operator=( const G3D_CQuaternion& quat );
	G3D_CQuaternion& operator*=( FLOAT a );
	G3D_CQuaternion& operator/=( FLOAT a );
	G3D_CQuaternion& operator^=( FLOAT a );
	G3D_CQuaternion& operator+=( const G3D_CQuaternion& quat );
	G3D_CQuaternion& operator-=( const G3D_CQuaternion& quat );

	// arithmetic operators
	G3D_CQuaternion operator+( const G3D_CQuaternion& quat ) const;
	G3D_CQuaternion operator-( const G3D_CQuaternion& quat ) const;
	G3D_CQuaternion operator*( FLOAT a ) const;
	G3D_CQuaternion operator*( const G3D_CQuaternion& q ) const;
	G3D_CQuaternion operator/( FLOAT a ) const;
	G3D_CQuaternion operator^( FLOAT a ) const;

	// unary operators
	G3D_CQuaternion operator-() const;
	G3D_CQuaternion operator+() const;

	// friend functions
	friend G3D_CQuaternion operator*( FLOAT a, const G3D_CQuaternion& quat );
	friend FLOAT Length( const G3D_CQuaternion& quat );
	friend G3D_CQuaternion Normalize( const G3D_CQuaternion& quat );
	friend G3D_CQuaternion Inverse( const G3D_CQuaternion& quat );
	friend G3D_CQuaternion UnaryInverse( const G3D_CQuaternion& quat );
	friend G3D_CQuaternion Conjunction( const G3D_CQuaternion& quat );
	friend G3D_CQuaternion ln( const G3D_CQuaternion& quat );
	friend G3D_CQuaternion exp( const G3D_CQuaternion& quat );
	friend G3D_CQuaternion CrossProd( const G3D_CQuaternion& p, const G3D_CQuaternion& q );
	friend FLOAT DotProd( const G3D_CQuaternion& p, const G3D_CQuaternion& q );
	friend G3D_CQuaternion FromAxisAngle( FLOAT x, FLOAT y, FLOAT z, FLOAT angle );
	friend G3D_CQuaternion FromAxisAngle( const G3D_CVector& axis, FLOAT angle );
	friend G3D_CQuaternion SLERP( FLOAT t, const G3D_CQuaternion& p, const G3D_CQuaternion& q );
	friend G3D_CQuaternion SQUAD( FLOAT t, const G3D_CQuaternion& p, const G3D_CQuaternion& a, 
							                const G3D_CQuaternion& b, const G3D_CQuaternion& q );	
};

class G3D_CMtxStack
{
private:

	class Node
	{
	public:

		G3D_CMatrix	mtx;
		Node*			prev;
	};

	Node*			top;

public:

	G3D_CMtxStack();
	~G3D_CMtxStack();

	void Clear();
	void Push( G3D_CMatrix& mtx );
	G3D_CMatrix Pop();
	void MulAndPush( G3D_CMatrix& mtx );
	G3D_CMatrix GetTop();
};

#endif