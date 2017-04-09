#include "Engine.h"

Input::Input( HWND window )
{
	m_window = window;

	//Create Direct Input Interface 
	DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_di, NULL);

	//Create, prepare & aquire the keyboard device
	m_di->CreateDevice( GUID_SysKeyboard, &m_keyboard, NULL );
	m_keyboard->SetDataFormat( &c_dfDIKeyboard );
	m_keyboard->SetCooperativeLevel( m_window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_keyboard->Acquire();

	//Create,prepare & aquire mouse
	m_di->CreateDevice( GUID_SysMouse, &m_mouse, NULL );
	m_mouse->SetDataFormat( &c_dfDIMouse );
	m_mouse->SetCooperativeLevel( m_window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	m_mouse->Acquire();

	// Start the press stamp.
	m_pressStamp = 0;
}

Input::~Input()
{
	SAFE_RELEASE( m_di );
	SAFE_RELEASE( m_keyboard );
	SAFE_RELEASE( m_mouse );

}

void Input::Update()
{
	static HRESULT result;

	//Poll the keyboard until we succeed or it return unknown error
	while(true)
	{
		m_keyboard->Poll();
		if( SUCCEEDED( result = m_keyboard->GetDeviceState( 256, &m_KeyState ) ) )
		   break;

		if( result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED )
			return;

		//Reacquire device if focused was lost in between
		if( FAILED( m_keyboard->Acquire() ) )
			return;
	}

	// Poll the mouse until it succeeds or returns an unknown error.
	while( true )
	{
		m_mouse->Poll();
		if( SUCCEEDED( result = m_mouse->GetDeviceState( sizeof( DIMOUSESTATE ), &m_mouseState ) ) )
			break;
		if( result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED )
			return;

		// Reacquire the device if the focus was lost.
		if( FAILED( m_mouse->Acquire() ) )
			return;
	}

	GetCursorPos( &m_position );
	ScreenToClient( m_window, &m_position);

	//Increament press stamp
	m_pressStamp++;
}


bool Input::GetKeyPress( char key, bool ignorePressStamp)
{
	if( ( m_KeyState[key] & 0x80 ) == false )
		return false;

	bool pressed = true;

	/*This code is added to check false pressed keys scenarios
	like holding key pressed and so on*/
	if( ignorePressStamp == false )
		if( m_keyPressStamp[key] == m_pressStamp - 1 || m_keyPressStamp[key] == m_pressStamp )
			pressed = false;

	m_keyPressStamp[key] = m_pressStamp;

	return pressed;
}

bool Input::GetButtonPress( char button, bool ignorePressStamp )
{
	if( ( m_mouseState.rgbButtons[button] & 0x80 ) == false )
		return false;

	bool pressed = true;

	if( ignorePressStamp == false )
		if( m_buttonPressStamp[button] == m_pressStamp - 1 || m_buttonPressStamp[button] == m_pressStamp )
			pressed = false;

	m_buttonPressStamp[button] = m_pressStamp;

	return pressed;
}

//-----------------------------------------------------------------------------
// Returns the x position of the mouse.
//-----------------------------------------------------------------------------
long Input::GetPosX()
{
	return m_position.x;
}

//-----------------------------------------------------------------------------
// Returns the y position of the mouse.
//-----------------------------------------------------------------------------
long Input::GetPosY()
{
	return m_position.y;
}

//-----------------------------------------------------------------------------
// Returns the change in the mouse's x coordinate.
//-----------------------------------------------------------------------------
long Input::GetDeltaX()
{
	return m_mouseState.lX;
}

//-----------------------------------------------------------------------------
// Returns the change in the mouse's y coordinate.
//-----------------------------------------------------------------------------
long Input::GetDeltaY()
{
	return m_mouseState.lY;
}

//-----------------------------------------------------------------------------
// Returns the change in the mouse's scroll wheel.
//-----------------------------------------------------------------------------
long Input::GetDeltaWheel()
{
	return m_mouseState.lZ;
}

