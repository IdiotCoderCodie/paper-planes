#include "Graphics.h"

// TESTING ONLY. SHOULD JUST MAKE A NEW SCENE OR SOMETHING. 
#include "Scenes\Scene.h"
#include "Entities\Entity.h"
#include "Components\Component.h"
#include "Components\Camera\CameraComponent.h"
#include "Components\Camera\PerspectiveCamComponent.h"
#include "Components\StaticMesh.h"
#include "Components\VisualComponent.h"
#include "Components\VisualMeshComponent.h"
#include "Assets\Shaders\Shader.h"


Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen) 
    :   m_d3d(screenWidth, screenHeight, true, hwnd, fullscreen),
        m_sceneMgr(m_d3d)
{
    // TESTING. ALL THIS IN HERE IS FOR TESTING. NEED TO GET IT OUT.
	Shader* shader = new Shader();

    Scene* testScene = new Scene(std::string("testScene"), &m_sceneMgr);

    m_sceneMgr.AddScene(testScene);
    // Cube entity.
    Entity* cubeEntity = new Entity(*testScene, std::string("testEntity1"));

    testScene->AddEntity(cubeEntity);

    VisualMeshComponent* meshComp 
        = new VisualMeshComponent(m_d3d, std::string("Assets\\Models\\cube.obj"), *shader); 
    //cubeEntity->MoveForward(10.0f);
    cubeEntity->RotateLocalY(-315.0f);
    cubeEntity->RotateGlobalX(-45.0f);
    meshComp->SetParent(*cubeEntity);

    cubeEntity->SetComponent(meshComp);

    Entity* cameraEntity = new Entity(*testScene, std::string("camEntity"));
    CameraComponent* camComp = new PerspectiveCamComponent(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    cameraEntity->SetComponent(camComp);
    cameraEntity->MoveForward(-10.0f);
    camComp->SetParent(*cameraEntity);
	
    testScene->AddEntity(cameraEntity);
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
