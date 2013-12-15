#pragma once
#include "Scene.h"
#include "../RenderTarget.h"

class PlaneScene : public Scene
{
public:
    PlaneScene(const std::string& name, SceneManager* sceneMgr);
    ~PlaneScene(void);

    void Draw(D3D& d3d);

private:
    RenderTarget* m_renderTargetTest;
    Entity* m_drawRenderTargetEntity;
};

