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
    : Scene(name, sceneMgr),
    m_renderTargetTest(0)
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


    m_renderTargetTest = new RenderTarget(&d3d.GetDevice(), 800, 600);
    
    // BITMAP for drawing what is rendered to render target.
    Entity* bmpEntity = new Entity(*this, std::string("bmpEntity"));
    Shader* newShader = new Shader();
    // Give bitmap the shader resource view from render target.
    Bitmap* bmp = new Bitmap(d3d, m_renderTargetTest->GetShaderResourceView(), 256, 256, 
                             GetParent().GetD3DInstance().GetScreenWidth(), 
                             GetParent().GetD3DInstance().GetScreenHeight());

    bmpEntity->SetComponent(new VisualBitmapComponent(d3d, *newShader, *bmp));
    AddEntity(bmpEntity);
    
}


PlaneScene::~PlaneScene(void)
{

}

void PlaneScene::Draw(D3D& d3d)
{
    m_renderTargetTest->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());

    m_renderTargetTest->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(),
                                          0.0f, 0.0f, 1.0f, 1.0f);

    // Now render once, rendering to texture.
    // This is going to try and render the bitmap which is drawing the renderTarget... hmm...
    Scene::Draw(d3d);

    //for(auto it = GetEntities().begin(); it != GetEntities().end(); ++it)
    //{
    //    if((*it)->GetComponent("VisualComponent"))
    //    {
    //        std::string compID;
    //        (*it)->GetComponent("VisualComponent")->ComponentID(compID);
    //        if(compID.compare("VisualBitmapComponent"))
    //        {
    //            // Didn't compare, so draw.
    //            (*it)->Draw(d3d);
    //        }
    //    }
    //}

    // Clear buffers to draw 3D scene.
    d3d.BeginScene(0.1f, 0.1f, 0.1f, 1.0f);
    // Reset to drawing to back buffer.
    d3d.SetBackBufferRenderTarget();

    // Now render 3D scene.
    Scene::Draw(d3d);
}