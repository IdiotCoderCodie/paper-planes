#include "InputManager.h"


InputManager::InputManager(void)
{
	for(int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}


InputManager::~InputManager(void)
{
}


void InputManager::KeyDown(unsigned int key)
{
	m_keys[key] = true;
}


void InputManager::KeyUp(unsigned int key)
{
	m_keys[key] = false;
}


bool InputManager::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
