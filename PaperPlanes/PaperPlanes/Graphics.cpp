#include "Graphics.h"

// TESTING ONLY. SHOULD JUST MAKE A NEW SCENE OR SOMETHING. 
#include "Scenes\Scene.h"
#include "Scenes\PlaneScene.h"

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen) 
    :   m_d3d(screenWidth, screenHeight, true, hwnd, fullscreen),
        m_sceneMgr(m_d3d)
{
    m_sceneMgr.AddScene(new PlaneScene("PlaneName", &m_sceneMgr));
}


Graphics::~Graphics(void)
{
}


bool Graphics::IsInitialized()
{
    return m_d3d.IsInitialized();
}


bool Graphics::Frame()
{
    m_d3d.BeginScene(0.8f, 0.8f, 0.8f, 0.1f);

    // NEED TO GET TIME.
	m_sceneMgr.Update(1.0);

	m_sceneMgr.Draw(m_d3d);

    m_d3d.EndScene();

    return true;
}
