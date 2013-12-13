#include "PlaneScene.h"
#include "../Assets/Shaders/Shader.h"
#include "../Assets/Textures/Texture.h"
#include "../Components/Visual/VisualMeshComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Camera/PerspectiveCamComponent.h"
#include "../Components/Light/LightComponent.h"
#include "../Components/Visual/VisualBitmapComponent.h"
#include "../Components/Physics/FreeRoamFpComponent.h"

#include "SceneManager.h"

PlaneScene::PlaneScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();
    // *************** TESTING. ALL THIS IN HERE IS FOR TESTING. *************** 
	Shader* shader = new Shader();

    // Cube entity.
    Entity* cubeEntity = new Entity(*this, std::string("testEntity1"));

    AddEntity(cubeEntity);

	Texture* texture = new Texture(d3d, L"Assets\\Textures\\tim.dds");
    VisualMeshComponent* meshComp 
        = new VisualMeshComponent(d3d, std::string("Assets\\Models\\cubeInv.obj"), *shader, *texture); 
    
    cubeEntity->MoveForward(0.0f);
    cubeEntity->MoveRight(1.0f);
    meshComp->SetParent(*cubeEntity);

    cubeEntity->SetComponent(meshComp);


    Entity* cameraEntity = new Entity(*this, std::string("camEntity"));
    float aspect = GetParent().GetD3DInstance().GetScreenWidth() 
                        / (float)GetParent().GetD3DInstance().GetScreenHeight();
    CameraComponent* camComp = new PerspectiveCamComponent(60.0f, aspect, 0.1f, 1000.0f);
    cameraEntity->SetComponent(camComp);
    cameraEntity->MoveForward(-10.0f);
    cameraEntity->MoveRight(1.0f);

    cameraEntity->SetComponent(new LightComponent(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
                                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    camComp->SetParent(*cameraEntity);
    cameraEntity->SetComponent(new FreeRoamFpComponent(10.0f, 50.0f, 50.0f));
	
    AddEntity(cameraEntity);


    // TESTING BITMAP
    Entity* bmpEntity = new Entity(*this, std::string("bmpEntity"));
    Shader* newShader = new Shader();
    Bitmap* bmp = new Bitmap(d3d, *texture, 256, 256, 
                             GetParent().GetD3DInstance().GetScreenWidth(), 
                             GetParent().GetD3DInstance().GetScreenHeight());

    bmpEntity->SetComponent(new VisualBitmapComponent(d3d, *newShader, *bmp));
    AddEntity(bmpEntity);
    // Some lolz.
    /*bmpEntity->SetComponent(new PhysicsComponent(1.0f, glm::vec3(-10.0f, 0.0f, 0.0f), 
                            glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));*/
}


PlaneScene::~PlaneScene(void)
{

}
