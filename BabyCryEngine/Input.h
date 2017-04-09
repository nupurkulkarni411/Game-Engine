#ifndef INPUT_H
#define INPUT_H

class Input
{

public:
	Input( HWND window );
	virtual ~Input();

	/*Game loop has many action processing input of user is
	one of them so for each action we haver seperate Update and
	render function that does specific tasks*/
	void Update();

	//For Keyboard
	bool GetKeyPress( char key, bool ignorePressStamp = false );
	//For Mouse
	bool GetButtonPress( char button, bool ignorePressStamp = false );
	long GetPosX();
	long GetPosY();
	long GetDeltaX();
	long GetDeltaY();
	long GetDeltaWheel();

private:

	HWND m_window; // Handle to parent window for which capture input
	IDirectInput8 *m_di;
	unsigned long m_pressStamp ; // Current Press stamp which increamented every frame

	IDirectInputDevice8 *m_keyboard; // DirectInput handle to keyboard
	char m_KeyState[256];
	unsigned long m_keyPressStamp[256];//Stamps the last frame each key was pressed

	IDirectInputDevice8 *m_mouse;
	DIMOUSESTATE m_mouseState;
	unsigned long m_buttonPressStamp[3];
	POINT m_position;

};

#endif