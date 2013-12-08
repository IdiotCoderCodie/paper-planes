#pragma once
#include "Scene.h"

class PlaneScene : public Scene
{
public:
    PlaneScene(const std::string& name, SceneManager* sceneMgr);
    ~PlaneScene(void);
};

