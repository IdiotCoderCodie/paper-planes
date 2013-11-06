#pragma once
#include <Windows.h>

#include "InputManager.h"
#include "Graphics.h"

class DXWindow
{
public:
	DXWindow(void);
	~DXWindow(void);

	bool IsInitialized();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

// Private Functions
private:
	// Private Copy Constructor.
	DXWindow(const DXWindow& other) {};
	bool Initialize();
	bool InitializeWindow(int& screenWidth, int& screenHeight);
	void ShutdownWindow();
	bool Frame();

// Private Data Members
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputManager* m_InputMgr;
	Graphics* m_Graphics;
};

// Used to redirect windows system messaging.
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

// Global pointer to window to redirect windows messaging.
static DXWindow* ApplicationHandle = 0;