#pragma once
class InputManager
{
public:
	InputManager(void);
	~InputManager(void);

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);

	bool IsKeyDown(unsigned int key);

// Private Functions
private:
	InputManager(const InputManager& other) {};

// Private Data Members
private:
	bool m_keys[256];
};

