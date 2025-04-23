#include "track.h"

FLOAT G3D_CBaseTrack::Ease( FLOAT easefrom, FLOAT easeto, FLOAT t )
{
	FLOAT 			k;
	FLOAT			s = easefrom + easeto;

	if( ISZERO( s )  )
		return t;

	if( s > 1.0f )
	{
		easefrom	= easefrom/s;
		easeto		= easeto/s;
	}

	k = 1.0f/( 2.0f - easefrom - easeto );

	if( t < easefrom )
		return ( ( k/easefrom )*t*t );
	else
	{
		if( t < 1.0f - easeto )
			return ( k*( 2.0f*t-easeto ) );
		else
		{
			t = 1.0f - t;
			return ( 1.0f - ( k/easeto )*t*t );
		}
	}
}

G3D_CVectorTrack::~G3D_CVectorTrack()
{
	if( pkKeys )
		delete pkKeys;
}

void G3D_CVectorTrack::InitTrack()
{
	if( dwKeysCount <= 1 )
		return;

	FLOAT				delta1, delta2;
	FLOAT				coeff1, coeff2;
	G3D_CVector		v1, v2;	

	for( DWORD i = 0 ; i < dwKeysCount ; i++ )
	{		
		delta1 = 0.5f;
		delta2 = 0.5f;

		if( i != dwKeysCount - 1 )		
			v1 = pkKeys[i+1].key - pkKeys[i].key;		

		if( i != 0 )		
			v2 = pkKeys[i].key - pkKeys[i-1].key;
		
		if( i == dwKeysCount - 1 )
			v1 = v2;

		if( i == 0 )
			v2 = v1;

		if( i != 0 && i != dwKeysCount - 1 )
		{
			delta1 = ( pkKeys[i+1].frame - pkKeys[i].frame )/( pkKeys[i+1].frame - pkKeys[i-1].frame );
			delta2 = ( pkKeys[i].frame - pkKeys[i-1].frame )/( pkKeys[i+1].frame - pkKeys[i-1].frame );
		}

		coeff1 = ( 1 - pkKeys[i].T )*( 1 - pkKeys[i].C )*( 1 - pkKeys[i].B );
		coeff2 = ( 1 - pkKeys[i].T )*( 1 + pkKeys[i].C )*( 1 + pkKeys[i].B );

		pkKeys[i].t0 = delta1*( coeff1*v1 + coeff2*v2 );

		coeff1 = ( 1 - pkKeys[i].T )*( 1 + pkKeys[i].C )*( 1 - pkKeys[i].B );
		coeff2 = ( 1 - pkKeys[i].T )*( 1 - pkKeys[i].C )*( 1 + pkKeys[i].B );

		pkKeys[i].t1 = delta2*( coeff1*v1 + coeff2*v2 );
	}
}

G3D_CVector G3D_CVectorTrack::FrameVector( FLOAT frame )
{
	if( dwKeysCount <= 1 )
		return pkKeys[0].key;

	if( pkKeys[0].frame > frame )
		return pkKeys[0].key;

	if( pkKeys[dwKeysCount-1].frame < frame )
		return pkKeys[dwKeysCount-1].key;

	for( DWORD i = 0 ; i < dwKeysCount - 1  && pkKeys[i+1].frame < frame ; i++ );

	FLOAT					t, t2, t3;
	FLOAT					h[4];

	t = (frame - pkKeys[i].frame)/(pkKeys[i+1].frame - pkKeys[i].frame);
	t = Ease( pkKeys[i].easefrom, pkKeys[i].easeto, t );

	t2 = t*t;
	t3 = t2*t;	

	h[0] = 2*t3 - 3*t2 + 1;
	h[1] = -2*t3 + 3*t2;
	h[2] = t3 - 2*t2 + t;
	h[3] = t3 - t2;

	return h[0]*pkKeys[i].key + h[1]*pkKeys[i+1].key + h[2]*pkKeys[i].t0 + h[3]*pkKeys[i+1].t1;
}

G3D_CMatrix G3D_CPositionTrack::FrameMtx( FLOAT frame )
{
	return TranslationMtx( FrameVector( frame ) );
}

G3D_CMatrix G3D_CScaleTrack::FrameMtx( FLOAT frame )
{
	return ScaleMtx( FrameVector( frame ) );
}

G3D_CMatrix G3D_CEulerRotTrack::FrameMtx( FLOAT frame )
{
	return RotationMtx( FrameVector( frame ) );
}

G3D_CQuaternionTrack::~G3D_CQuaternionTrack()
{
	if( pkKeys )
		delete pkKeys;
}

void G3D_CQuaternionTrack::InitTrack()
{	
	pkKeys->key = FromAxisAngle( pkKeys->axis, pkKeys->angle );
	pkKeys->spin= 0;

	G3D_CQuaternion		t0, t1;
	G3D_CQuaternion		log1, log2;
	FLOAT					delta1, delta2;
	FLOAT					coeff1, coeff2;
	DWORD					i;
	FLOAT					log_angle;

	for( i = 0 ; i < dwKeysCount ; i++ )
	{
		if( pkKeys[i].angle < 0 )
		{
			pkKeys[i].angle = -pkKeys[i].angle;
			pkKeys[i].axis	= -pkKeys[i].axis;
		}
	}

	if( dwKeysCount <= 1 )
		return;

	for( i = 1 ; i < dwKeysCount ; i++ )
	{			
		pkKeys[i].spin	= (LONG)( floorf( pkKeys[i].angle/H_2PI ) );
		pkKeys[i].angle	= fmodf( pkKeys[i].angle, H_2PI );	
		pkKeys[i].key	= pkKeys[i-1].key*FromAxisAngle( pkKeys[i].axis, pkKeys[i].angle );
	}

	for( i = 0 ; i < dwKeysCount ; i++ )
	{
		delta1 = 0.5f;
		delta2 = 0.5f;

		if( i != dwKeysCount - 1 )
		{
			if( !pkKeys[i+1].spin )
			{
				log_angle = pkKeys[i+1].angle*0.5f;

				if( log_angle > H_PI2 )
					log_angle -= H_PI2;
			}
			else
				log_angle = H_PI2;

			log1.w = 0;
			log1.x = pkKeys[i+1].axis.x*log_angle;
			log1.y = pkKeys[i+1].axis.y*log_angle;
			log1.z = pkKeys[i+1].axis.z*log_angle;
		}

		if( i != 0 )
		{
			if( !pkKeys[i].spin )
			{
				log_angle = pkKeys[i].angle*0.5f;

				if( log_angle > H_PI2 )
					log_angle -= H_PI2;
			}
			else
				log_angle = H_PI2;

			log2.w = 0;
			log2.x = pkKeys[i].axis.x*log_angle;
			log2.y = pkKeys[i].axis.y*log_angle;
			log2.z = pkKeys[i].axis.z*log_angle;
		}		
		
		if( i == dwKeysCount - 1 )
			log1 = log2;

		if( i == 0 )
			log2 = log1;		

		if( i != 0 && i != dwKeysCount - 1 )
		{
			delta1 = ( pkKeys[i+1].frame - pkKeys[i].frame )/( pkKeys[i+1].frame - pkKeys[i-1].frame );
			delta2 = ( pkKeys[i].frame - pkKeys[i-1].frame )/( pkKeys[i+1].frame - pkKeys[i-1].frame );
		}

		coeff1 = ( 1 - pkKeys[i].T )*( 1 - pkKeys[i].C )*( 1 - pkKeys[i].B );
		coeff2 = ( 1 - pkKeys[i].T )*( 1 + pkKeys[i].C )*( 1 + pkKeys[i].B );

		t0 = delta1*( coeff1*log1 + coeff2*log2 );

		coeff1 = ( 1 - pkKeys[i].T )*( 1 + pkKeys[i].C )*( 1 - pkKeys[i].B );
		coeff2 = ( 1 - pkKeys[i].T )*( 1 - pkKeys[i].C )*( 1 + pkKeys[i].B );

		t1 = delta2*( coeff1*log1 + coeff2*log2 );

		pkKeys[i].an = pkKeys[i].key*exp( 0.5*( t0 - log1 ) );
		pkKeys[i].bn = pkKeys[i].key*exp( 0.5*( log2 - t1 ) );
	}
}

G3D_CQuaternion G3D_CQuaternionTrack::FrameQuaternion( FLOAT frame )
{
	FLOAT					t;

	if( dwKeysCount <= 1 )
		return pkKeys[0].key;

	if( pkKeys[0].frame > frame )
		return pkKeys[0].key;

	if( pkKeys[dwKeysCount-1].frame < frame )
		return pkKeys[dwKeysCount-1].key;

	for( DWORD i = 0 ; i < dwKeysCount - 1  && pkKeys[i+1].frame < frame ; i++ );	

	t = (frame - pkKeys[i].frame)/(pkKeys[i+1].frame - pkKeys[i].frame);
	t = Ease( pkKeys[i].easefrom, pkKeys[i].easeto, t );

	if( !pkKeys[i+1].spin )
		return SQUAD( t, pkKeys[i].key, pkKeys[i].an, pkKeys[i+1].bn, pkKeys[i+1].key );

	FLOAT					angle = pkKeys[i+1].angle + (FLOAT)pkKeys[i+1].spin*H_2PI;
	FLOAT					p0 = H_PI/angle;
	FLOAT					p1 = 1.0f - p0;
	G3D_CQuaternion		s0, s1;
	G3D_CQuaternion		q0, q1;

	if(t < p0)
	{
		
		s0 = pkKeys[i].key*FromAxisAngle( pkKeys[i+1].axis, p0*angle );
		t /= p0;

		q0 = SLERP( t, pkKeys[i].key, s0 );
		q1 = SLERP( t, pkKeys[i].an, s0 );
	} 
	else if( t <= p1 )
	{
		t -= p0;
		t /= p1 - p0;

		q0 = FromAxisAngle( pkKeys[i+1].axis, fmodf( H_PI + t*( angle-H_2PI ), H_2PI ) );
			
		return pkKeys[i].key*q0;
	} 
	else
	{			
		s1 = pkKeys[i].key*FromAxisAngle( pkKeys[i+1].axis, p1*angle );	

		if( pkKeys[i+1].spin&0x1 )
			s1 = -s1;

		t -= p1;
		t /= p0;

		q0 = SLERP( t, s1, pkKeys[i+1].key );
		q1 = SLERP( t, s1, pkKeys[i+1].bn );		
	};

	return SLERP( 2.0f*t*( 1.0f-t ), q0, q1 );
}

G3D_CMatrix G3D_CQuaternionTrack::FrameMtx( FLOAT frame )
{
	return RotationMtx( FrameQuaternion( frame ) );	
}

void G3D_CValueEnvelope::InitEnvelope()
{
	if( dwKeysCount <= 1 )
		return;

	FLOAT				delta1, delta2;
	FLOAT				coeff1, coeff2;
	FLOAT				f1, f2;	

	for( DWORD i = 0 ; i < dwKeysCount ; i++ )
	{		
		delta1 = 0.5f;
		delta2 = 0.5f;

		if( i != dwKeysCount - 1 )		
			f1 = pkKeys[i+1].key - pkKeys[i].key;		

		if( i != 0 )		
			f2 = pkKeys[i].key - pkKeys[i-1].key;
		
		if( i == dwKeysCount - 1 )
			f1 = f2;

		if( i == 0 )
			f2 = f1;

		if( i != 0 && i != dwKeysCount - 1 )
		{
			delta1 = ( pkKeys[i+1].frame - pkKeys[i].frame )/( pkKeys[i+1].frame - pkKeys[i-1].frame );
			delta2 = ( pkKeys[i].frame - pkKeys[i-1].frame )/( pkKeys[i+1].frame - pkKeys[i-1].frame );
		}

		coeff1 = ( 1 - pkKeys[i].T )*( 1 - pkKeys[i].C )*( 1 - pkKeys[i].B );
		coeff2 = ( 1 - pkKeys[i].T )*( 1 + pkKeys[i].C )*( 1 + pkKeys[i].B );

		pkKeys[i].t0 = delta1*( coeff1*f1 + coeff2*f2 );

		coeff1 = ( 1 - pkKeys[i].T )*( 1 + pkKeys[i].C )*( 1 - pkKeys[i].B );
		coeff2 = ( 1 - pkKeys[i].T )*( 1 - pkKeys[i].C )*( 1 + pkKeys[i].B );

		pkKeys[i].t1 = delta2*( coeff1*f1 + coeff2*f2 );
	}
}

G3D_CValueEnvelope::~G3D_CValueEnvelope()
{
	if( pkKeys )
		delete pkKeys;
}

FLOAT G3D_CValueEnvelope::EnvelopeValue( FLOAT frame )
{	
	if( dwKeysCount <= 1 )
		return pkKeys[0].key;

	if( pkKeys[0].frame > frame )
		return pkKeys[0].key;

	if( pkKeys[dwKeysCount-1].frame < frame )
		return pkKeys[dwKeysCount-1].key;

	for( DWORD i = 0 ; i < dwKeysCount - 1  && pkKeys[i+1].frame < frame ; i++ );

	FLOAT					t, t2, t3;
	FLOAT					h[4];

	t = (frame - pkKeys[i].frame)/(pkKeys[i+1].frame - pkKeys[i].frame);
	t = Ease( pkKeys[i].easefrom, pkKeys[i].easeto, t );

	t2 = t*t;
	t3 = t2*t;	

	h[0] = 2*t3 - 3*t2 + 1;
	h[1] = -2*t3 + 3*t2;
	h[2] = t3 - 2*t2 + t;
	h[3] = t3 - t2;

	return h[0]*pkKeys[i].key + h[1]*pkKeys[i+1].key + h[2]*pkKeys[i].t0 + h[3]*pkKeys[i+1].t1;
}

G3D_CDataEnvelope::~G3D_CDataEnvelope()
{
	if( pkKeys )
		delete pkKeys;
}

DWORD G3D_CDataEnvelope::PrevData( FLOAT frame )
{
	if( dwKeysCount <= 1 )
		return pkKeys[0].key;

	if( pkKeys[0].frame > frame )
		return pkKeys[0].key;

	if( pkKeys[dwKeysCount-1].frame < frame )
		return pkKeys[dwKeysCount-1].key;

	for( DWORD i = 0 ; i < dwKeysCount - 1  && pkKeys[i+1].frame < frame ; i++ );

	return pkKeys[i].key;
}

DWORD G3D_CDataEnvelope::NextData( FLOAT frame )
{
	if( dwKeysCount <= 1 )
		return pkKeys[0].key;

	if( pkKeys[0].frame > frame )
		return pkKeys[0].key;

	if( pkKeys[dwKeysCount-1].frame < frame )
		return pkKeys[dwKeysCount-1].key;

	for( DWORD i = 0 ; i < dwKeysCount - 1  && pkKeys[i+1].frame < frame ; i++ );

	return pkKeys[i+1].key;
}

FLOAT G3D_CDataEnvelope::LocalTime( FLOAT frame )
{
	if( dwKeysCount <= 1 )
		return 0.0f;

	if( pkKeys[0].frame > frame )
		return 0.0f;

	if( pkKeys[dwKeysCount-1].frame < frame )
		return 1.0f;

	for( DWORD i = 0 ; i < dwKeysCount - 1  && pkKeys[i+1].frame < frame ; i++ );

	FLOAT					t;

	t = (frame - pkKeys[i].frame)/(pkKeys[i+1].frame - pkKeys[i].frame);
	t = Ease( pkKeys[i].easefrom, pkKeys[i].easeto, t );

	return t;
}

void G3D_CDataEnvelope::InitEnvelope()
{
}