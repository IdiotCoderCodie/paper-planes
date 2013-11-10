#pragma once
#include "D3D.h"

#include "Scenes/SceneManager.h"

class Graphics
{
public:
	Graphics(int screenWidth, int screenHeight, HWND hwnd);

	~Graphics(void);

	bool IsInitialized();

	bool Frame();

// Private Functions
private:
    Graphics(const Graphics&);
    Graphics& operator=(const Graphics&);

// Private Data Members
private:
	D3D m_d3d;
	SceneManager m_sceneMgr;
};
