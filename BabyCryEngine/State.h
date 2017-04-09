#ifndef STATE_H
#define STATE_H

//Currently Empty
struct ViewerSetup
{
	unsigned long viewClearFlags; // Flags used for clearing the view.

	ViewerSetup()
	{
		viewClearFlags = 0;
	};
};

/*Engine will not implement this load and close methods
Also we dont provide any destructor here as close function
is supposed to do all the cleanup.
Application code will override this methods load state define it.*/
class State
{
public:
	/*State has unique id which used to identify it in different states*/
	State( unsigned long id = 0 );

	virtual void Load();
	virtual void Close();

	/*Later we will use this function to get the ViewerSetup Structure*/
	virtual void RequestViewer(ViewerSetup *viewer);
	/*Update and Render functions will be called after the execution of
	Game loop Update and Render all main rendering will be handled in those
	Game loop functions*/
	virtual void Update(float elapsed);
	virtual void Render();

	unsigned long GetID();

private:
	unsigned long m_id;
};
#endif