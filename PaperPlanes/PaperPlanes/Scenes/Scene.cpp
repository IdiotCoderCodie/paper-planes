#include "Scene.h"
#include "../Components/Camera/CameraComponent.h"

Scene::Scene(const std::string& name, SceneManager* sceneMgr)
    :   m_Name(name),
        m_Parent(sceneMgr),
        m_Entities(),
        m_ActiveCamera(0)
{
}


Scene::~Scene()
{
    for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
    {
        delete *it;
        *it = 0;
    }

    m_Parent = 0;
}


void Scene::AddEntity(Entity* ent)
{
    m_Entities.push_back(ent);

    // LOGIC FOR LIGHTS AND CAMERAS.
    // Component* light = objectToAdd->GetComponent("LightComponent");
	//if(light)
	//{ // This object has a light attached... add it to the lights list.
	//	// Cast the component to a LightComponent and push it into the lights list.
	//	LightComponent* test = static_cast<LightComponent*>(light);
	//	m_Lights.push_back(static_cast<LightComponent*>(light));
	//}

	// If there is not already an active camera component then check if the 
	// added entity has a camera component attached to it and add this as the
	// active camera.
    if(!m_ActiveCamera)
	{
		Component* camCheck = ent->GetComponent("CameraComponent");
		if(camCheck)
		{
			m_ActiveCamera = static_cast<CameraComponent*>(camCheck);
		}
	}
}


void Scene::Update(double time)
{
    for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
    {
        (*it)->Update(time);
    }
}


void Scene::Draw(D3D& d3d)
{
    for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
    {
        (*it)->Draw(d3d);
    }
}
