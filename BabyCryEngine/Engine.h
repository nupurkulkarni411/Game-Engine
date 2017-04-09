#ifndef ENGINE_H
#define ENGINE_H

#define DIRECTINPUT_VERSION 0x0800

//System includes
#include <stdio.h>
#include <tchar.h>
/*This file was introduced in windows 3.1
to help identify lots if compile time error
like it had STRICT predecessor it was
favoured over windows.h
#include<windowsx.h>*/
//This file also includes everything capital W 
#include <windowsx.h>

//DirectX includes
#include <d3dx9.h>
#include <dinput.h>
#include <dplay8.h>
#include <dmusici.h>
#include <string>


//Macros 
#define SAFE_DELETE( p )       { if( p ) { delete( p );    ( p ) = NULL;} }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL;} }
#define SAFE_RELEASE( p )      { if( p ) {( p )->Release(); ( p ) = NULL;} }

/* All Engine Specific Includes will be in this file so 
application can just include single header and have acess to 
entire engine*/
#include "Utility.h"
#include "Resource.h"
#include "LinkedList.h"
#include "ResourceManagement.h"
#include "Geometry.h"
#include "Scripting.h"
#include "Input.h"
#include "State.h"
#include "DeviceEnumeration.h"
#include "Font.h"
#include "SoundSystem.h"
#include "BoundingVolume.h"
#include "Material.h"
/*This structure helps user to give some control and tell 
how our engine instance should be initiated*/
struct EngineSetup
{
	HINSTANCE instance; //Application instance handle
	wchar_t *name;         //Application windows namee
	float scale;        //Unit scale in meters/unit
	unsigned char totalBackBuffers; // No. of back buffers to use
	/*As part of engine setup application code can set state
	setup function which will do the task of adding states*/
	void (*StateSetup)();
	void (*CreateMaterialResource)( Material **resource, char *name, char *path );

	/*we want to keep it such that if user is not interested in 
	setting up the engine we have some default values to which
	it will be intantiated*/
	EngineSetup()
	{
		instance = NULL;
		name = L"BabyCry";
		scale = 1.0f;
		totalBackBuffers = 1;
		StateSetup = NULL;
		CreateMaterialResource = NULL;
	}
};

//Engine Class
class Engine
{
public :
	Engine(EngineSetup *setup = NULL);
	virtual ~Engine();

	void Run(); // Game loop 

	HWND GetWindow();
	void SetDeactivateFlag(bool deactive);

	//State Related functions
	void AddState( State *state, bool change = true );
	void RemoveState( State *state );
	void ChangeState( unsigned long id );
	State *GetCurrentState();

	//Device related functions
	float GetScale();
	IDirect3DDevice9 *GetDevice();
	D3DDISPLAYMODE *GetDisplayMode();
	ID3DXSprite *GetSprite();

	//Resource Manager related functions
	ResourceManager<Script> *GetScriptManager();
	ResourceManager< Material > *GetMaterialManager();

	//Input related functions
	Input *GetInput();
	SoundSystem *GetSoundSystem();

private:
	//Window Related Variables
	bool m_loaded; // Indicates if engine is loaded or not
	HWND m_window; // Handle to the application window
	bool m_deactivate; // Indicates if application is deactivated

	IDirect3DDevice9 *m_device; // Direct3D device interface.
	D3DDISPLAYMODE m_displayMode; // Display mode of the current Direct3D device.
	ID3DXSprite *m_sprite; // Sprite interface for rendering 2D textured primitives.
	unsigned char m_currentBackBuffer; // Keeps track of which back buffer is at the front of the swap chain.

	//State Related Variables
	LinkedList< State > *m_states;
	State *m_currentState;
	bool m_stateChanged;

	//Input Related Variables
	Input *m_input;
	SoundSystem *m_soundSystem;

	//Script manager
	ResourceManager<Script> *m_scriptManager;
	ResourceManager< Material > *m_materialManager;

	EngineSetup *m_setup;
};

/*
This global variable is single point if acess to application 
when use is done it can be destroyed.
*/
extern Engine *g_engine;

#endif
