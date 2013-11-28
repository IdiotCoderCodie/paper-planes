#include "Graphics.h"


Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen) 
    :   m_d3d(screenWidth, screenHeight, false, hwnd, fullscreen),
        m_sceneMgr(m_d3d)
{
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
    m_d3d.BeginScene(1.0f, 0.2f, 0.2f, 0.1f);

    m_d3d.EndScene();

    //// GET TIME
    //currTime = GetTime()
    //float timeElapsed = prevTime - currTime;

    //// UPDATE
    //m_sceneMgr->Update(timeElapsed);

    //// RENDER
    //m_sceneMgr->Render(); 
    return true;
}
