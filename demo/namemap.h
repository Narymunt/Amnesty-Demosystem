#ifndef	__NAMEMAP_H_
#define __NAMEMAP_H_

#include <string.h>
#include <stdlib.h>

// this class is not case sensitiv !

#define		HASHTABLESIZE		24

template<class T> 
class CNameMap
{
private:

	class Node
	{
	public:
		
		T*				TObj;
		char*			strName;

		Node*			next;
		Node*			listnext;
		
		Node( const char* name, T* obj ) : TObj( obj ), next( NULL ), listnext( NULL )
		{
			strName = new char[strlen(name)+1];
			strcpy( strName, name );			
		}

		~Node()
		{			
			delete TObj;
			delete strName;
		}
	};

	Node*			hashTable[HASHTABLESIZE];
	Node*			root;
	Node*			current;
	
	DWORD HashFunction( const char* str )
	{
		if( !str )
			return 0;

		DWORD			val = strlen( str );

		for( DWORD i = 0 ; i < (DWORD)strlen( str ) ; i++ )				
			val += (DWORD)str[i];		

		return val%HASHTABLESIZE;
	}

public:

	CNameMap() : root( NULL )
	{
		for( DWORD i = 0 ; i < HASHTABLESIZE ; i++ )
			hashTable[i] = NULL;
	}

	~CNameMap()
	{		
		for( Node* tmp = root ; tmp ; )
		{
			root = tmp->listnext;
			delete tmp;
			tmp = root;
		}
	}

	VOID Add( const char* name, T* obj )
	{	
		if( Get( name ) != NULL )
			return;

		Node*			node	= new Node( name, obj );
		Node*			tmp		= hashTable[HashFunction( name )];

		if( !tmp )
			hashTable[HashFunction( name )] = node;
		else
		{
			for( ; tmp->next ; tmp = tmp->next );

			tmp->next = node;
		}
		
		if( !root )
			root = node;
		else
		{
			for( tmp = root ; tmp->listnext ; tmp = tmp->listnext );

			tmp->listnext = node;
		}
	};

	T* Get( const char* name )
	{				
		Node*			tmp = hashTable[HashFunction( name )];

		for( ; tmp && _stricmp( name, tmp->strName ) ; tmp = tmp->next );		

		if( tmp )
			return tmp->TObj;

		return NULL;
	}

	VOID Remove( const char* name )
	{
		Node*			tmp = hashTable[HashFunction( name )];

		if( !tmp )
			return;

		if( !_stricmp( name, tmp->strName ) )
		{
			hashTable[HashFunction( name )] = tmp->next;
		}
		else
		{		
			for( ; tmp->next && _stricmp( name, tmp->next->strName ) ; tmp = tmp->next );

			if( !tmp->next )
				return;

			tmp->next = tmp->next->next;
		}

		if( !_stricmp( root->strName, name ) )
		{
			tmp = root->listnext;
			delete root;
			root = tmp;
			return;
		}

		for( tmp = root ; tmp->listnext && _stricmp( name, tmp->listnext->strName ) ; tmp = tmp->listnext );

		Node*			out = tmp->listnext;
		tmp->listnext = tmp->listnext->listnext;
		delete out;
	}

	VOID SetToRoot()
	{
		current = root;
	}

	T* GetNext()
	{
		T*				tmp = NULL;

		if( current )
		{		
			tmp = current->TObj;
			current = current->listnext;
		}

		return tmp;
	}

	char* GetCurrentName()
	{
		if( current )
			return current->strName;

		return NULL;
	}
};

#endif

	



