#include "Engine.h"


Variable::Variable( char *name, FILE *file )
{
	//store name of the variable
	m_name = new char[strlen(name) + 1];
	strcpy(m_name, name);

	if(file == NULL)
		return;

	//Read the variable`s type 
	char buffer[MAX_PATH];
	fscanf(file, "%s", buffer);
	if(strcmp(buffer, "bool") == 0)
	{
		m_type = VARIABLE_BOOL;

		bool value;
		fscanf(file, "%s", buffer);
		if(strcmp(buffer, "true") == 0)
			value = true;
		else
			value = false;

		m_data = new bool;
		memcpy(m_data, &value, sizeof(bool));
	}
	else if( strcmp( buffer, "colour" ) == 0 )
	{
		// The variable is a colour.
		m_type = VARIABLE_COLOUR;

		// Read and set the colour for the variable.
		D3DCOLORVALUE colour;
		fscanf( file, "%s", buffer );
		colour.r = (float)atof( buffer );
		fscanf( file, "%s", buffer );
		colour.g = (float)atof( buffer );
		fscanf( file, "%s", buffer );
		colour.b = (float)atof( buffer );
		fscanf( file, "%s", buffer );
		colour.a = (float)atof( buffer );
		m_data = new D3DCOLORVALUE;
		memcpy( m_data, &colour, sizeof( D3DCOLORVALUE ) );
	}
	else if( strcmp( buffer, "float" ) == 0 )
	{
		// The variable is a float.
		m_type = VARIABLE_FLOAT;

		// Read and set the float for the variable.
		float value;
		fscanf( file, "%s", buffer );
		value = (float)atof( buffer );
		m_data = new float;
		memcpy( m_data, &value, sizeof( float ) );
	}
	else if( strcmp( buffer, "number" ) == 0 )
	{
		// The variable is a number.
		m_type = VARIABLE_NUMBER;

		// Read and set the number for the variable.
		long value;
		fscanf( file, "%s", buffer );
		value = atol( buffer );
		m_data = new long;
		memcpy( m_data, &value, sizeof( long ) );
	}
	else if( strcmp( buffer, "string" ) == 0 )
	{
		// The variable is a string.
		m_type = VARIABLE_STRING;

		// Find the opening inverted commas.
		bool commasFound = false;
		ZeroMemory( buffer, MAX_PATH * sizeof( char ) );
		fscanf( file, "%c", buffer );
		while( true )
		{
			if( strcmp( buffer, "\"" ) == 0 )
			{
				commasFound = true;
				break;
			}

			if( strcmp( buffer, " " ) != 0 )
			{
				fpos_t pos;
				fgetpos( file, &pos );
				fsetpos( file, &--pos );
				break;
			}

			fscanf( file, "%c", buffer );
		}

		// Read and set the string for the variable.
		char completeString[MAX_PATH];
		ZeroMemory( completeString, MAX_PATH * sizeof( char ) );
		bool addSpacing = false;
		do
		{
			fscanf( file, "%s", buffer );
			if( strcmp( &buffer[strlen( buffer ) - 1], "\"" ) == 0 )
			{
				buffer[strlen( buffer ) - 1] = 0;
				commasFound = false;
			}

			if( addSpacing == false )
				addSpacing = true;
			else
				strcat( completeString, " " );

			strcat( completeString, buffer );
		} while( commasFound == true );

		m_data = new char[strlen( completeString ) + 1];
		strcpy( (char*)m_data, completeString );
	}
	else if( strcmp( buffer, "vector" ) == 0 )
	{
		// The variable is a vector.
		m_type = VARIABLE_VECTOR;

		// Read and set the vector for the variable.
		D3DXVECTOR3 vector;
		fscanf( file, "%s", buffer );
		vector.x = (float)atof( buffer );
		fscanf( file, "%s", buffer );
		vector.y = (float)atof( buffer );
		fscanf( file, "%s", buffer );
		vector.z = (float)atof( buffer );
		m_data = new D3DXVECTOR3;
		memcpy( m_data, &vector, sizeof( D3DXVECTOR3 ) );
	}
	else
	{
		// The variable has an unknown type.
		m_type = VARIABLE_UNKNOWN;

		// Read and set the data (same as a string) for the variable.
		fscanf( file, "%s", buffer );
		m_data = new char[strlen( buffer ) + 1];
		strcpy( (char*)m_data, buffer );
	}

}

Variable::Variable( char *name, char type, void *value )
{
	// Store the name of the variable.
	m_name = new char[strlen( name ) + 1];
	strcpy( m_name, name );

	// Store the type of the variable.
	m_type = type;

	// Set the variable's data based on its type.
	switch( m_type )
	{
	case VARIABLE_BOOL:
		m_data = new bool;
		memcpy( m_data, (bool*)value, sizeof( bool ) );
		return;

	case VARIABLE_COLOUR:
		m_data = new D3DCOLORVALUE;
		memcpy( m_data, (D3DCOLORVALUE*)value, sizeof( D3DCOLORVALUE ) );
		return;

	case VARIABLE_FLOAT:
		m_data = new float;
		memcpy( m_data, (float*)value, sizeof( float ) );
		return;

	case VARIABLE_NUMBER:
		m_data = new long;
		memcpy( m_data, (long*)value, sizeof( long ) );
		return;

	case VARIABLE_STRING:
		m_data = new char[strlen( (char*)value ) + 1];
		strcpy( (char*)m_data, (char*)value );
		return;

	case VARIABLE_VECTOR:
		m_data = new D3DXVECTOR3;
		memcpy( m_data, (D3DXVECTOR3*)value, sizeof( D3DXVECTOR3 ) );
		return;

	default:
		m_data = new char[strlen( (char*)value ) + 1];
		strcpy( (char*)m_data, (char*)value );
		return;
	}
}

Variable::~Variable()
{
	SAFE_DELETE_ARRAY( m_name );
	SAFE_DELETE( m_data );
}

char Variable::GetType()
{
	return m_type;
}

char *Variable::GetName()
{
	return m_name;
}

void *Variable::GetData()
{
	switch(m_type)
	{
	case VARIABLE_BOOL:
		return (bool*)m_data;

	case VARIABLE_COLOUR:
		return (D3DCOLORVALUE*)m_data;

	case VARIABLE_FLOAT:
		return (float*)m_data;

	case VARIABLE_NUMBER:
		return (long*)m_data;

	case VARIABLE_STRING:
		return (char*)m_data;

	case VARIABLE_VECTOR:
		return (D3DXVECTOR3*)m_data;
	
	default:
		return m_data;

	}
}


Script::Script( char *name, char *path ):Resource< Script >( name, path)
{
	m_variables = new LinkedList<Variable>;

	FILE *file = fopen(GetFileName(), "r");
	if(file == NULL)
		return;

	bool read = false;
	char buffer[MAX_PATH];
	fscanf(file, "%s", buffer);
	while(feof(file) == 0)
	{
		/*whatever is in between this begin and end will have one 
		variable per line in form name type value*/
		if(read == true)
		{
			if(strcmp(buffer, "#end") == 0)
				read = false;
			else 
				m_variables->Add( new Variable(buffer, file) );
		}
		else if(strcmp(buffer, "#begin") == 0)
			read = true;

		//Read next line
		fscanf(file, "%s", buffer);
	}

	fclose(file);
}

Script::~Script()
{
	/*This safe delete will call destructor of linked list
	   which calls empty function which intern deletes all 
	   the nodes of linkedlist one by one in this case list
	   is of the type Variable and hece will result in call
	   to destructure of Variable intern freeing all variables
	   inside it so clearing everything*/
	SAFE_DELETE(m_variables);
}

void Script::AddVariable(char *name, char type, void *value)
{
	m_variables->Add(new Variable(name, type, value));
}


void Script::SetVariable(char *name, void *value)
{
	Variable *variable = NULL;
	m_variables->Iterate(true);
	while(m_variables->Iterate() != NULL)
	{
		if(strcmp(m_variables->Iterate()->GetName(), name) == 0)
		{
			variable = m_variables->GetCurrent();
			break;
		}
	}

	//Ensure variable was found
	if (variable == NULL)
		return;

	//Get Variables type
	char type = variable->GetType();

	//Destroy variable
	m_variables->Remove( &variable );

	//Add variable with new value
	AddVariable(name, type, value);
}

void Script::SaveScript(char *filename)
{
	FILE *file = NULL;
	char output[MAX_PATH];

	// Open the given filename if available, otherwise the internal filename.
	if( filename != NULL )
	{
		if( ( file = fopen( filename, "w" ) ) == NULL )
			return;
	}
	else
	{
		if( ( file = fopen( GetFileName(), "w" ) ) == NULL )
			return;
	}

	// Write the #begin statement to the file.
	fputs( "#begin\n", file );

	// Write each variable to the file.
	m_variables->Iterate( true );
	while( m_variables->Iterate() != NULL )
	{
		switch( m_variables->GetCurrent()->GetType() )
		{
		case VARIABLE_BOOL:
			if( *((bool*)m_variables->GetCurrent()->GetData()) == true )
				sprintf( output, "%s bool true", m_variables->GetCurrent()->GetName() );
			else
				sprintf( output, "%s bool false", m_variables->GetCurrent()->GetName() );
			fputs( output, file );
			fputs( "\n", file );
			continue;

		case VARIABLE_COLOUR:
			sprintf( output, "%s colour %f %f %f %f", m_variables->GetCurrent()->GetName(), ( (D3DCOLORVALUE*)m_variables->GetCurrent()->GetData() )->r, ( (D3DCOLORVALUE*)m_variables->GetCurrent()->GetData() )->g, ( (D3DCOLORVALUE*)m_variables->GetCurrent()->GetData() )->b, ( (D3DCOLORVALUE*)m_variables->GetCurrent()->GetData() )->a );
			fputs( output, file );
			fputs( "\n", file );
			continue;

		case VARIABLE_FLOAT:
			sprintf( output, "%s float %f", m_variables->GetCurrent()->GetName(), *(float*)m_variables->GetCurrent()->GetData() );
			fputs( output, file );
			fputs( "\n", file );
			continue;

		case VARIABLE_NUMBER:
			sprintf( output, "%s number %d", m_variables->GetCurrent()->GetName(), *(long*)m_variables->GetCurrent()->GetData() );
			fputs( output, file );
			fputs( "\n", file );
			continue;

		case VARIABLE_STRING:
			sprintf( output, "%s string \"%s\"", m_variables->GetCurrent()->GetName(), (char*)m_variables->GetCurrent()->GetData() );
			fputs( output, file );
			fputs( "\n", file );
			continue;

		case VARIABLE_VECTOR:
			sprintf( output, "%s vector %f %f %f", m_variables->GetCurrent()->GetName(), ( (D3DXVECTOR3*)m_variables->GetCurrent()->GetData() )->x, ( (D3DXVECTOR3*)m_variables->GetCurrent()->GetData() )->y, ( (D3DXVECTOR3*)m_variables->GetCurrent()->GetData() )->z );
			fputs( output, file );
			fputs( "\n", file );
			continue;

		default:
			sprintf( output, "%s unknown %s", m_variables->GetCurrent()->GetName(), (char*)m_variables->GetCurrent()->GetData() );
			fputs( output, file );
			fputs( "\n", file );
			continue;
		}
	}

	// Write the #end statement to the file.
	fputs( "#end", file );

	// Close the script file.
	fclose( file );
}

bool *Script::GetBoolData(char *name)
{
	m_variables->Iterate(true);
	while(m_variables->Iterate() != NULL)
		if(strcmp(m_variables->GetCurrent()->GetName(), name) == 0)
			return (bool*)m_variables->GetCurrent()->GetData();
}

D3DCOLORVALUE *Script::GetColorData(char *name)
{
	m_variables->Iterate(true);
	while(m_variables->Iterate() != NULL)
		if(strcmp(m_variables->GetCurrent()->GetName(), name) == 0)
			return (D3DCOLORVALUE*)m_variables->GetCurrent()->GetData();
}

float *Script::GetFloatData(char *name)
{
	m_variables->Iterate(true);
	while(m_variables->Iterate() != NULL)
		if(strcmp(m_variables->GetCurrent()->GetName(), name) == 0)
			return (float*)m_variables->GetCurrent()->GetData();
}

long *Script::GetNumberData(char *name)
{
	m_variables->Iterate(true);
	while(m_variables->Iterate() != NULL)
		if(strcmp(m_variables->GetCurrent()->GetName(), name) == 0)
			return (long*)m_variables->GetCurrent()->GetData();
}

char *Script::GetStringData(char *name)
{
	m_variables->Iterate(true);
	while(m_variables->Iterate() != NULL)
		if(strcmp(m_variables->GetCurrent()->GetName(), name) == 0)
			return (char*)m_variables->GetCurrent()->GetData();
}

D3DXVECTOR3 *Script::GetVectorData(char *name)
{
	m_variables->Iterate(true);
	while(m_variables->Iterate() != NULL)
		if(strcmp(m_variables->GetCurrent()->GetName(), name) == 0)
			return (D3DXVECTOR3*)m_variables->GetCurrent()->GetData();
}

D3DCOLORVALUE *Script::GetColourData( char *variable )
{
	m_variables->Iterate( true );
	while( m_variables->Iterate() != NULL )
		if( strcmp( m_variables->GetCurrent()->GetName(), variable ) == 0 )
			return (D3DCOLORVALUE*)m_variables->GetCurrent()->GetData();

	return NULL;
}


void *Script::GetUnknownData(char *name)
{
	m_variables->Iterate(true);
	while(m_variables->Iterate() != NULL)
		if(strcmp(m_variables->GetCurrent()->GetName(), name) == 0)
			return m_variables->GetCurrent()->GetData();
}