#pragma once
#include "D3D.h"

class Graphics
{
public:
	Graphics(/*HWND hwnd*/);
	~Graphics(void);

	bool IsInitialized();

	bool Frame();

// Private Functions
private:
	Graphics(const Graphics& other) {};

// Private Data Members
private:
	D3D* m_d3d;
	// SceneManager m_sceneMgr;
};
