//
//	CBaseTrack
//	 |
//	 +--------------------+--------------------+
//	 |					  |					   |				   
//	CTrack				 CDataEnvelope		  CValueEnvelope
//	 |
//	 +--------------------+
//	 |					  |
//	CVectorTrack		 CQuaternionTrack
//	 |
//	 +--------------------+--------------------+
//	 |					  |					   |
//	CPositionTrack	 CScaleTrack		  CEulerRotTrack

#ifndef __TRACK_H_
#define __TRACK_H_

#include "math3d.h"

class G3D_CBaseTrack
{
public:

	DWORD			dwKeysCount;

public:

	G3D_CBaseTrack() : dwKeysCount( 0 )
	{
	}
	
	virtual ~G3D_CBaseTrack()
	{
	}

	FLOAT Ease( FLOAT easefrom, FLOAT easeto, FLOAT t );
};

class G3D_CTrack : public G3D_CBaseTrack
{
public:

	G3D_CTrack() 
	{
	}	

	virtual G3D_CMatrix FrameMtx( FLOAT frame ) = 0;
	virtual void InitTrack() = 0;
};
 
class G3D_CVectorTrack : public G3D_CTrack
{
public:

	class CKey
	{
	public:

		FLOAT				frame;
		G3D_CVector		key;

		G3D_CVector		t0;
		G3D_CVector		t1;

		FLOAT				T, B, C;

		FLOAT				easeto, easefrom;
	};

	CKey*				pkKeys;

public:

	G3D_CVectorTrack() : pkKeys( NULL )
	{
	}

	virtual ~G3D_CVectorTrack();

	virtual G3D_CMatrix FrameMtx( FLOAT frame ) = 0;
	virtual void InitTrack();

	G3D_CVector FrameVector( FLOAT frame );
};

class G3D_CScaleTrack : public G3D_CVectorTrack
{
public:

	G3D_CScaleTrack()
	{
	}

	virtual G3D_CMatrix FrameMtx( FLOAT frame );	
};

class G3D_CEulerRotTrack : public G3D_CVectorTrack
{
public:

	G3D_CEulerRotTrack()
	{
	}

	virtual G3D_CMatrix FrameMtx( FLOAT frame );
};

class G3D_CPositionTrack : public G3D_CVectorTrack
{
public:

	G3D_CPositionTrack()
	{
	}

	virtual G3D_CMatrix FrameMtx( FLOAT frame );
};

class G3D_CQuaternionTrack : public G3D_CTrack
{
public:

	class CKey
	{
	public:

		FLOAT				frame;
		G3D_CQuaternion	key;

		G3D_CQuaternion	an;
		G3D_CQuaternion	bn;

		G3D_CVector		axis;
		FLOAT				angle;

		LONG				spin;

		FLOAT				T, B, C;

		FLOAT				easefrom, easeto;
	};

	CKey*				pkKeys;

public:

	G3D_CQuaternionTrack() : pkKeys( NULL )
	{
	}

	virtual ~G3D_CQuaternionTrack();

	virtual G3D_CMatrix FrameMtx( FLOAT frame );
	virtual void InitTrack();

	G3D_CQuaternion FrameQuaternion( FLOAT frame );
};

class G3D_CDataEnvelope : public G3D_CBaseTrack
{
public:

	class CKey
	{
	public:
		
		FLOAT			frame;
		DWORD			key;

		FLOAT			easeto, easefrom;
	};
	
	CKey*			pkKeys;

public:

	G3D_CDataEnvelope() : pkKeys( NULL )
	{
	}

	virtual ~G3D_CDataEnvelope();

	DWORD PrevData( FLOAT frame );
	DWORD NextData( FLOAT frame );

	FLOAT LocalTime( FLOAT frame );

	void InitEnvelope();
};

class G3D_CValueEnvelope : public G3D_CBaseTrack
{
public:

	class CKey
	{
	public:

		FLOAT				frame;
		FLOAT				key;

		FLOAT				t0;
		FLOAT				t1;

		FLOAT				T, B, C;
		
		FLOAT				easeto, easefrom;
	};

	CKey*			pkKeys;	

public:

	G3D_CValueEnvelope() : pkKeys( NULL )
	{
	}
	
	virtual ~G3D_CValueEnvelope();	

	FLOAT EnvelopeValue( FLOAT frame );

	void InitEnvelope();
};


#endif