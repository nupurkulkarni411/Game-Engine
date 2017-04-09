#ifndef SCRIPTING_H
#define SCRIPTING_H

enum{ VARIABLE_BOOL, VARIABLE_COLOUR, VARIABLE_FLOAT, VARIABLE_NUMBER, VARIABLE_STRING, VARIABLE_VECTOR, VARIABLE_UNKNOWN };

class Variable
{
public:
	Variable( char *name, FILE *file);
	Variable( char *name, char type, void *value);
	virtual ~Variable();

	char GetType();
	char *GetName();
	void *GetData();

private:
	/*Variables are stored in string format and have name type data form*/
	char m_type;
	char *m_name;
	void *m_data;
};

/*We are keeping script file as resource and for that we will also create
resource manager in our engine class.
Also see we are creating our class based on data they are accepting and not
functionality they provide so they are decoupled and combining such decoupled
we can make bigger types for example look here Resource->script->variable*/

class Script : public Resource< Script >
{
public: 
	Script( char *name, char *path = "./" );
	virtual ~Script();

	void AddVariable( char *name, char type, void *value ); // Add variable to linked list of resource
	void SetVariable( char *name, void *value );

	void SaveScript( char *filename );

	bool *GetBoolData( char* variable );
	D3DCOLORVALUE *GetColorData( char *variable );
	float *GetFloatData( char *variable );
	long *GetNumberData( char *variable );
	char *GetStringData( char *variable );
	D3DXVECTOR3 *GetVectorData( char *variable );
	D3DCOLORVALUE *GetColourData( char *variable );
	void *GetUnknownData( char *variable );

private:
	/*Script contains linked list of variables
	  Script is put inside Resource and 
	  ResourceManage contains linked list of
	  these resources*/
	LinkedList< Variable > *m_variables;
};
#endif