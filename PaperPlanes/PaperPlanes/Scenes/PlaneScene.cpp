#include "PlaneScene.h"
#include "../Assets/Shaders/Shader.h"
#include "../Assets/Textures/Texture.h"
#include "../Components/VisualMeshComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/Camera/PerspectiveCamComponent.h"
#include "../Components/Light/LightComponent.h"

#include "SceneManager.h"

PlaneScene::PlaneScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();
    // TESTING. ALL THIS IN HERE IS FOR TESTING. NEED TO GET IT OUT.
	Shader* shader = new Shader();

    Scene* testScene = new Scene(std::string("testScene"), sceneMgr);

    sceneMgr->AddScene(testScene);
    // Cube entity.
    Entity* cubeEntity = new Entity(*testScene, std::string("testEntity1"));

    testScene->AddEntity(cubeEntity);

	Texture* texture = new Texture(d3d, L"Assets\\Textures\\tim.dds");
    VisualMeshComponent* meshComp 
        = new VisualMeshComponent(d3d, std::string("Assets\\Models\\cube.obj"), *shader, *texture); 
    
    cubeEntity->MoveForward(15.0f);
    meshComp->SetParent(*cubeEntity);

    cubeEntity->SetComponent(meshComp);
    cubeEntity->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f),
                                                  glm::vec3(0.0f, 0.20, 0.0f), 
                                                  glm::vec3(0.0f, 0.0f, 0.000f)));


    Entity* cameraEntity = new Entity(*testScene, std::string("camEntity"));
    CameraComponent* camComp = new PerspectiveCamComponent(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    cameraEntity->SetComponent(camComp);
    cameraEntity->MoveForward(-10.0f);

    cameraEntity->SetComponent(new LightComponent(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
                                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    camComp->SetParent(*cameraEntity);
	
    testScene->AddEntity(cameraEntity);
}


PlaneScene::~PlaneScene(void)
{

}
