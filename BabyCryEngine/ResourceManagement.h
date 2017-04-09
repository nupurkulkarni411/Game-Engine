#ifndef RESOURCE_MANAGEMENT_H
#define RESOURCE_MANAGEMENT_H

/*This resource can be anything like
sound file mesh file or script file
with every resource there is associated
resource manager*/
template< class Type > class Resource
{
public:
	Resource( char *name, char *path = "./" )
	{
		if( name != NULL )
		{
			m_name = new char[strlen( name ) + 1];
			memcpy( m_name, name, ( strlen( name ) + 1 ) * sizeof( char ) );
		}

		if( path != NULL )
		{
			m_path = new char[strlen( path ) + 1];
			memcpy( m_path, path, ( strlen( path ) + 1 ) * sizeof( char ) );
		}

		//Create the filename
		if( name != NULL && path != NULL )
		{
			m_fileName = new char[strlen( name ) + strlen( path ) + 1];
			sprintf( m_fileName, "%s%s", path, name );
		}

		//Start ref count
		m_refCount = 1;
	}

	virtual ~Resource()
	{
		SAFE_DELETE_ARRAY( m_name );
		SAFE_DELETE_ARRAY( m_path );
		SAFE_DELETE_ARRAY( m_fileName );
	}

	char *GetName()
	{
		return m_name;
	}

	char *GetPath()
	{
		return m_path;
	}

	char *GetFileName()
	{
		return m_fileName;
	}

	void IncRef()
	{
		m_refCount++;
	}

	void DecRef()
	{
		m_refCount--;
	}

	unsigned long GetRefCount()
	{
		return m_refCount;
	}

private:
	char *m_name; //Name of the resource
	char *m_path; //Path to the resource
	char *m_fileName; // Full path to the resource
	unsigned long m_refCount; //Reference Count
};

template < class Type > class ResourceManager
{
public:
	ResourceManager( void (*CreateResourceFunction)( Type **resource, char *name, char *path) = NULL )
	{
		m_list = new LinkedList< Type >;
		CreateResource = CreateResourceFunction;
	}

	~ResourceManager()
	{
		SAFE_DELETE( m_list );
	}

	Type *Add( char *name, char *path = "./" )
	{
		if( m_list == NULL || name == NULL || path == NULL)
		  return NULL;

		//If element already exists then return pointer to it
		Type *element = GetElement( name, path );
		if( element != NULL )
		{
			element->IncRef();
			return element;
		}

		//Create Resource from application specific function
		//This is new resource creation 
		Type *reource = NULL;
		if(CreateResource != NULL)
			CreateResource( &resource, name, path );
		else
			resource = new Type( name , path );

		return m_list->Add( resource );
	}

	//Removes resource from resource manager
	void Remove( Type **resource)
	{
		if( *resource == NULL || m_list == NULL)
			return;

		(*resource)->DecRef();

		if( (*resource)->GetRefCount() == 0 )
			m_list->remove( resource );
	}

	void EmptyList()
	{
		if( m_list != NULL )
		{
			m_list->Empty();
		}
	}

	LinkedList < Type > *GetLit()
	{
		return m_list;
	}

	Type *GetElement( char *name, char *path = "./" )
	{
		// Ensure the name and path are valid, and the list is valid and not empty.
		if( name == NULL || path == NULL || m_list == NULL )
			return NULL;
		if( m_list->GetFirst() == NULL )
			return NULL;

		// Iterate the list looking for the specified resource.
		m_list->Iterate( true );
		while( m_list->Iterate() )
			if( strcmp( m_list->GetCurrent()->GetName(), name ) == 0 )
				if( strcmp( m_list->GetCurrent()->GetPath(), path ) == 0 )
					return m_list->GetCurrent();

		// Return NULL if the resource was not found.
		return NULL;
	}

private:
	LinkedList< Type > *m_list; // List list of resources
	void (*CreateResource)( Type **resource, char *name, char *path); // User will call resource specific function
};

#endif