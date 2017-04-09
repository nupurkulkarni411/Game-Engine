#include <Windows.h>
#include "../BabyCryEngine/Engine.h"


class TestState : public State
{

	virtual void Load()
	{
		m_font = new Font;
		m_sound = new Sound( "./Assets/Sound.wav" );
	}

	virtual void Cloase()
	{
		SAFE_DELETE(m_font);
	}

	virtual void RequestViewer(ViewerSetup *viewer)
	{
		viewer->viewClearFlags = D3DCLEAR_TARGET;
	}

	virtual void Update( float elapsed )
	{
	  static float frameTime = 1.0f;
	  static int frameCount = 0;

	  frameTime += elapsed;
	  frameCount++;

	  if(frameTime > 1.0)
	  {
		  sprintf(m_fps, "%d FPS", frameCount);
		  frameTime = 0.0;
		  frameCount = 0;
	  }

	  if(g_engine->GetInput()->GetKeyPress(DIK_SPACE))
		  m_sound->Play();
	}

	virtual void Render()
	{
		m_font->Render( "This text is rendered using the new Direct3D device.", 10, 10 );
		m_font->Render( m_fps, 10, 50, D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 0.0f, 1.0f ) );
	}

private:

	Font* m_font;
	char m_fps[16];
	Sound* m_sound;
};

void StateSetup()
{
	g_engine->AddState(new TestState, true);
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int cmdShow )
{
	// Create the engine setup structure.
	EngineSetup setup;
	setup.instance = instance;
	setup.name = L"Rendering Test";
	setup.totalBackBuffers = 2;
	setup.StateSetup = StateSetup;

	// Create the engine (using the setup structure), then run it.
	new Engine( &setup );
	g_engine->Run();

	return true;
}