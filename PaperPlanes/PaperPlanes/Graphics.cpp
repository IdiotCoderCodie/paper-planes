#include "Graphics.h"


Graphics::Graphics(void) 
	: m_d3d()
{
}


Graphics::~Graphics(void)
{
}


bool Graphics::IsInitialized()
{
	return m_d3d;
}

bool Graphics::Frame()
{
	//// GET TIME
	//currTime = GetTime()
	//float timeElapsed = prevTime - currTime;

	//// UPDATE
	//m_sceneMgr->Update(timeElapsed);

	//// RENDER
	//m_sceneMgr->Render(); 
    return true;
}
