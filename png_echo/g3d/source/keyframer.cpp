
#include "scene.h"
#include "keyframer.h"
#include "object.h"
#include "track.h"

// destruktor

G3D_CKeyframer::~G3D_CKeyframer()
{
	if( ptRotation )
		delete ptRotation;

	if( ptScale )
		delete ptScale;

	if( ptPosition )	
		delete ptPosition;
}

// transformacja

void G3D_CKeyframer::Transform( FLOAT _frame, G3D_CMtxStack& stack )
{
	G3D_CMatrix				mtx = stack.GetTop();
	G3D_CPositionTrack*		track = (G3D_CPositionTrack*)ptPosition;

	mtxFrame	= IdentMtx();
	frame		= _frame;	

	if( ptScale ) mtxFrame *= ptScale->FrameMtx( frame );
	if( ptRotation ) mtxFrame *= ptRotation->FrameMtx( frame );
	if( ptPosition ) mtxFrame *= ptPosition->FrameMtx( frame );	

	mtxFrame *= mtx;

	if( poKeyframerOwner ) poKeyframerOwner->UpdateFrame( this );	

	if( pkfSub )
	{
		stack.Push( mtxFrame );
		pkfSub->Transform( frame, stack );
		stack.Pop();
	}

	if( pkfNext ) pkfNext->Transform( frame, stack );	
}
