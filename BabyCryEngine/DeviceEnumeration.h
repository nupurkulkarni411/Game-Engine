#ifndef DEVICE_ENUMERATION_H
#define DEVICE_ENUMERATION_H

//D3D9 -> D3D9 Device->device has display mode-> display mode supports display formats
struct DisplayMode
{
	D3DDISPLAYMODE mode;
	char bpp[6];
};

class DeviceEnumeration
{
public:
	INT_PTR Enumerate(IDirect3D9 *d3d);
	INT_PTR SettingsDialogProc(HWND dialog, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	D3DDISPLAYMODE *GetSelectedDisplayMode();
	bool IsWindowed();
	bool IsVSynced();

private:
	void ComboBoxAdd(HWND dialog, int id, void *data, wchar_t *desc);
	void ComboBoxSelect(HWND dialog, int id, int index);
	void ComboBoxSelect(HWND dialog, int id, void *data);
	void *ComboBoxSelected( HWND dialog, int id );
	bool ComboBoxSomethingSelected(HWND dialog, int id);
	int ComboBoxCount(HWND dialog, int id);
	bool ComboBoxContainsText(HWND dialog, int id, wchar_t *text);

private:
	Script *m_settingsScript;

	D3DADAPTER_IDENTIFIER9 m_adapter;
	LinkedList<DisplayMode> *m_displayModes;
	D3DDISPLAYMODE m_selectedDisplayMode;
	bool m_windowed;
	bool m_vsync;
};

extern DeviceEnumeration *g_deviceEnumeration;

#endif