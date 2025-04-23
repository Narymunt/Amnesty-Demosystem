#ifndef	__KEYFRAMER_H_
#define __KEYFRAMER_H_

#include "math3d.h"

class G3D_CTrack;
class G3D_CObject;
class G3D_CScene;

class G3D_CKeyframer
{
public:

	G3D_CScene*			pScene;
	FLOAT				frame;

	G3D_CKeyframer*		pkfNext;
	G3D_CKeyframer*		pkfSub;

	G3D_CObject*		poKeyframerOwner;

	G3D_CMatrix			mtxFrame;	

	G3D_CTrack*			ptPosition;
	G3D_CTrack*			ptRotation;
	G3D_CTrack*			ptScale;
	
public:

	G3D_CKeyframer( G3D_CScene* scene ) : pScene( scene ), 
											poKeyframerOwner( NULL ), 
											pkfNext( NULL ), 
											pkfSub( NULL ),
											frame( 0.0f )
	{
		// pusto
	}

	virtual ~G3D_CKeyframer();

	virtual void Transform( FLOAT _frame, G3D_CMtxStack& stack );
};

#endif
