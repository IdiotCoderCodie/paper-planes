#include "DXWindow.h"
#include <map>

DXWindow::DXWindow(void) :	
    m_applicationName(),
    m_hinstance(),
    m_hwnd(0),
    m_InputMgr(0),
    m_Graphics(0)
{
    int screenWidth, screenHeight;

    if(InitializeWindow(screenWidth, screenHeight))
    {
        m_InputMgr = new InputManager;

        m_Graphics = new Graphics(screenWidth, screenHeight, m_hwnd);
    }	
}


DXWindow::~DXWindow(void)
{
    if(m_Graphics)
    {
        delete m_Graphics;
        m_Graphics = 0;
    }

    if(m_InputMgr)
    {
        delete m_InputMgr;
        m_InputMgr = 0;
    }

    ShutdownWindow();
}


bool DXWindow::IsInitialized()
{
    if(!m_InputMgr || !m_Graphics || !m_Graphics->IsInitialized())
        return false;
    else
        return true;
}


void DXWindow::Run()
{
    MSG msg;
    // Init message structure.
    ZeroMemory(&msg, sizeof(MSG));

    bool done = false;
    while(!done)
    {
        // Handle mwindows messages.
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            done = true;
        else
        { // If frame returns fase, user wanted to quit.
            done = !(Frame());
        }
    }
}


bool DXWindow::Frame()
{
    if(m_InputMgr->IsKeyDown(VK_ESCAPE))
        return false;

    return m_Graphics->Frame();
    //return m_Graphics->Frame();
    return true;
}


LRESULT CALLBACK DXWindow::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch(umsg)
    {
    case WM_KEYDOWN:
        m_InputMgr->KeyDown((unsigned int)wparam);
        break;

    case WM_KEYUP:
        m_InputMgr->KeyUp((unsigned int)wparam);
        break;

    default:
        break;
    }
    // Send all messages to default message handler.
    return DefWindowProc(hwnd, umsg, wparam, lparam);
}


bool DXWindow::InitializeWindow(int& screenWidth, int& screenHeight)
{
    // Set external pointer to this object.
    ApplicationHandle = this;

    // Get the instance of the application
    m_hinstance = GetModuleHandle(NULL);

    m_applicationName = L"Paper Planet";

    // Setup windows class with default settings.
    WNDCLASSEX wc;
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc		= WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= m_hinstance;
    wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm			= wc.hIcon;
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName		= NULL;
    wc.lpszClassName	= m_applicationName;
    wc.cbSize			= sizeof(WNDCLASSEX);

    // Register window class.
    RegisterClassEx(&wc);

    // Get the display resolution.
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Set to fullscreen.
    DEVMODE dmScreenSettings;
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth	= (unsigned long)screenWidth;
    dmScreenSettings.dmPelsHeight	= (unsigned long)screenHeight;
    dmScreenSettings.dmBitsPerPel	= 32;
    dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

    // Create the window with the settings generated.
    m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        0, 0, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

    // Bring the window up on screen and set to main focus.
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    // Hide cursor.
    ShowCursor(false);

    return true;
}


void DXWindow::ShutdownWindow()
{
    ShowCursor(true);

    ChangeDisplaySettings(NULL, 0);

    // Remove Window.
    DestroyWindow(m_hwnd);
    m_hwnd = nullptr;

    // Remove Application instance.
    UnregisterClass(m_applicationName, m_hinstance);
    m_hinstance = nullptr;

    ApplicationHandle = nullptr;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    default: // Pass all other messages to the message handler in DXWindow.
        return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);
    }
}
