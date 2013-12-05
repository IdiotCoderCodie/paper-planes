#pragma once
#include <vector>
#include <string>

#include "../D3D.h"
#include "../Entities/Entity.h"

class CameraComponent;

class SceneManager;

class Scene
{
public:
    Scene(const std::string& name, SceneManager* sceneMgr);
    ~Scene();

    const std::string& GetSceneName() { return m_Name; }

    void AddEntity(Entity* entity);

    const CameraComponent* GetActiveCamera()                  { return m_ActiveCamera; }
    void SetActiveCamera(CameraComponent* const newActiveCam) { m_ActiveCamera = newActiveCam; }

    SceneManager& GetParent() { return *m_Parent; }

    void Update(double time);

    void Draw(D3D& d3d);

private:
    std::string m_Name;
    SceneManager* m_Parent;
    std::vector<Entity*> m_Entities;

    CameraComponent* m_ActiveCamera;
};

