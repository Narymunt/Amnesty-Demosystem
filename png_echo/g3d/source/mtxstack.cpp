#include "math3d.h"
#include <math.h>

G3D_CMtxStack::G3D_CMtxStack() : top( NULL )
{
}

G3D_CMtxStack::~G3D_CMtxStack()
{
	Clear();
}

void G3D_CMtxStack::Clear()
{
	Node*				node;

	while( top )
	{
		node = top->prev;
		delete top;
		top	 = node;
	}

	top = NULL;
}

void G3D_CMtxStack::Push( G3D_CMatrix& mtx )
{
	Node*				node = new Node;

	node->mtx	= mtx;
	node->prev	= top;

	top = node;
}

G3D_CMatrix G3D_CMtxStack::Pop()
{
	G3D_CMatrix		out;
	Node*				node = top;

	if( top )
	{	
		out = top->mtx;
		top = top->prev;
		delete node;
	}

	return out;
}

G3D_CMatrix G3D_CMtxStack::GetTop()
{
	G3D_CMatrix		out;

	if( top )
		out = top->mtx;

	return out;
}

void G3D_CMtxStack::MulAndPush( G3D_CMatrix& mtx )
{
	if( !top )
	{
		Push( mtx );		
	}
	else
	{
		G3D_CMatrix	tmp = mtx*top->mtx;
		Push( tmp );
	}
}
