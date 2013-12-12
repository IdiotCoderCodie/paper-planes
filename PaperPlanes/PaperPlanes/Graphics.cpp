#include "Graphics.h"

// TESTING ONLY. SHOULD JUST MAKE A NEW SCENE OR SOMETHING. 
#include "Scenes\Scene.h"
#include "Scenes\PlaneScene.h"

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen) 
    :   m_d3d(screenWidth, screenHeight, true, hwnd, fullscreen),
        m_sceneMgr(m_d3d),
        m_timer()
{
    m_sceneMgr.AddScene(new PlaneScene("PlaneName", &m_sceneMgr));
    m_timer.Start();
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
    
    double elapsedTime = m_timer.GetTimeInSeconds();

    m_sceneMgr.Update(elapsedTime);

	m_sceneMgr.Draw(m_d3d);

    m_d3d.EndScene();

    return true;
}
