#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class InputManager
{
public:
	InputManager();
	~InputManager();

    bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);

    bool Update();

    void GetMouseLocation(int mouseX, int mouseY) { mouseX = m_mouseX; mouseY = m_mouseY; }

    bool IsEscapeKeyPressed() { return m_keyboardState[DIK_ESCAPE] & 0x80; }

    bool IsKeyPressed(int keyId);// { return m_keyboardState[keyId] & 0x80; }

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);

	bool IsKeyDown(unsigned int key);

// Private Functions
private:
	InputManager(const InputManager& other) {};

    bool ReadKeyboard();
    bool ReadMouse();

    void ProcessInput();

// Private Data Members
private:
    IDirectInput8* m_directInput;
    IDirectInputDevice8* m_keyboard;
    IDirectInputDevice8* m_mouse;

    unsigned char m_keyboardState[256];
    DIMOUSESTATE m_mouseState;

    int m_screenWidth;
    int m_screenHeight;
    int m_mouseX;
    int m_mouseY;
};

