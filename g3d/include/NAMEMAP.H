#ifndef	__NAMEMAP_H_
#define __NAMEMAP_H_

#include <string.h>
#include <stdlib.h>

template<class T> 
class CNameMap
{
private:

	class Node
	{
	public:
		
		T*				pTObj;
		char*			strName;

		Node*			next;
		Node*			prev;
		
		Node( const char* name, T* obj ) : next( NULL ), prev( NULL )
		{
			pTObj = obj;
			strName = new char[strlen(name)+1];
			strcpy( strName, name );				
		}

		~Node()
		{			
			delete pTObj;
			delete strName;
		}
	};
	
	Node*			root;
	Node*			current;
	
public:

	CNameMap() : root( NULL )
	{		
	}

	~CNameMap()
	{		
		for( Node* tmp = root ; tmp ; )
		{
			root = tmp->next;
			delete tmp;
			tmp = root;
		}
	}

	VOID Add( const char* name, T* obj )
	{	
		if( Get( name ) != NULL )
			return;		
		
		Node*				node = new Node( name, obj );
		Node*				tmp;
		
		if( !root )
			root = node;
		else
		{
			for( tmp = root ; tmp->next ; tmp = tmp->next );

			tmp->next = node;
			node->prev = tmp;
		}
	};

	T* Get( const char* name )
	{				
		Node*			tmp;

		for( tmp = root ; tmp && _stricmp( tmp->strName, name ) ; tmp = tmp->next );

		if( tmp )
			return tmp->pTObj;

		return NULL;
	}

	VOID Remove( const char* name )
	{	
		Node*				tmp;

		if( !_stricmp( root->strName, name ) )
		{
			tmp = root->next;
			tmp->prev = NULL;

			delete root;

			root = tmp;			
		}
		else
		{		
			for( tmp = root ; tmp && _stricmp( name, tmp->strName ) ; tmp = tmp->next );
			
			if( tmp )
			{			
				tmp->next->prev = tmp->prev;
				tmp->prev->next = tmp->next;

				delete tmp;
			}
		}
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
			tmp = current->pTObj;
			current = current->next;			
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

	



