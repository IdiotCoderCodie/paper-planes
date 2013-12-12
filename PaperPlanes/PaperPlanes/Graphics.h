#pragma once
#include "D3D.h"

#include "Scenes/SceneManager.h"
#include "HiResTimer.h"

class Graphics
{
public:
	Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen);

	~Graphics(void);

	bool IsInitialized();

	bool Frame();

// Private Functions
private:
    Graphics(const Graphics& other);
    Graphics& operator=(const Graphics& other);

// Private Data Members
private:
	D3D m_d3d;
	SceneManager m_sceneMgr;
    HiResTimer m_timer;
};
