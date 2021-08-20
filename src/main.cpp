#include "ofMain.h"
#include "ofApp.h"
#include "resource.h"
///Strange code to make stuff work on main GPU
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	ofGLFWWindowSettings settings;
	settings.setSize(420, 380);
	settings.resizable = false;
	ofCreateWindow(settings);

	HWND hwnd = ofGetWin32Window();
	HICON hMyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hMyIcon);

	return ofRunApp(new ofApp());

}
