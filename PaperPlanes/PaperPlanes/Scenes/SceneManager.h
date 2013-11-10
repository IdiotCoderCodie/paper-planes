#pragma once
#include <string>
#include <map>

#include "Scene.h"
#include "../D3D.h"

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    // Sets the active scene with name matching "sceneName". 
    // Returns: Whether or not such a scene exists and was made active.
    bool SetActiveScene(const std::string& sceneName);
    // Sets the active screen to the one pointer to by scenePtr.
    // Returns whether or not it was successful
    bool SetActiveScene(Scene* const scenePtr);

    bool DoesSceneExist(const std::string& sceneName);

    bool AddScene(Scene* const sceneToAdd);

    void ClearScenes();

    void Update(double timeElapsed);

    void Draw(D3D& d3d);

private:
    typedef std::map<std::string, Scene*> SceneMap;
    std::map<std::string, Scene*> m_Scenes;
    Scene* m_ActiveScene;
};

