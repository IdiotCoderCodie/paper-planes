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
    
    m_renderTargetTest = new RenderTarget(&d3d.GetDevice(), 800, 600);

	Shader* shader = new Shader();

    //----------------------------------------------------------------------------------------------
    // Main inversed cube entity.
    Entity* cubeEntity = new Entity(*this, std::string("testEntity1"));

    AddEntity(cubeEntity);

	Texture* texture = new Texture(d3d, L"Assets\\Textures\\tim.dds");
    VisualMeshComponent* meshComp = new VisualMeshComponent(d3d, 
                                            std::string("Assets\\Models\\cubeInv.obj"), *texture, 
                                            m_renderTargetTest->GetShaderResourceView()); 
    
    meshComp->SetParent(*cubeEntity);

    cubeEntity->SetComponent(meshComp);
    cubeEntity->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), 
                               glm::vec3(0.0f, 20.0f, 0.0f)));
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Small occluding cube.
    Entity* occluderCube = new Entity(*this, std::string("testOccluderCube"));
    AddEntity(occluderCube);

    VisualMeshComponent* occluderCubeMesh = new VisualMeshComponent(d3d, 
                                            std::string("Assets\\Models\\cube.obj"), *texture, 
                                            m_renderTargetTest->GetShaderResourceView());
    occluderCubeMesh->SetParent(*occluderCube);
    occluderCube->SetComponent(occluderCubeMesh);
    occluderCube->MoveUp(-1.5f);
    occluderCube->MoveForward(-5.0f);
    occluderCube->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), 
                               glm::vec3(15.0f, 20.0f, 0.0f)));
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Camera entity.
    Entity* cameraEntity = new Entity(*this, std::string("camEntity"));

    float aspect = GetParent().GetD3DInstance().GetScreenWidth() 
                        / (float)GetParent().GetD3DInstance().GetScreenHeight();
    CameraComponent* camComp = new PerspectiveCamComponent(60.0f, aspect, 0.1f, 100.0f);
    camComp->SetParent(*cameraEntity);
    cameraEntity->SetComponent(camComp);
    
    cameraEntity->SetComponent(new FreeRoamFpComponent(10.0f, 50.0f, 50.0f));
    
    cameraEntity->MoveForward(-10.0f);
    AddEntity(cameraEntity);
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Light entity
    Entity* lightEntity = new Entity(*this, std::string("lightEntity"));

    LightComponent* lightComp = new LightComponent(glm::vec4(0.04f, 0.04f, 0.04f, 1.0f),
                                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    lightComp->GenerateProjectionMatrix(1.0f, 100.0f);
    lightEntity->SetComponent(lightComp);
    //lightEntity->SetComponent(new FreeRoamFpComponent(10.0f, 50.0f, 50.0f));
    lightEntity->MoveForward(-10.0f);
    AddEntity(lightEntity);
	//----------------------------------------------------------------------------------------------
    


    
    // BITMAP for drawing what is rendered to render target.
    Entity* bmpEntity = new Entity(*this, std::string("bmpEntity"));
    Shader* newShader = new Shader();
    // Give bitmap the shader resource view from render target.
    Bitmap* bmp = new Bitmap(d3d, m_renderTargetTest->GetShaderResourceView(), 100, 100, 
                             GetParent().GetD3DInstance().GetScreenWidth(), 
                             GetParent().GetD3DInstance().GetScreenHeight());

    bmpEntity->SetComponent(new VisualBitmapComponent(d3d, *newShader, *bmp));
    AddEntity(bmpEntity);

    m_drawRenderTargetEntity = bmpEntity;
    
}


PlaneScene::~PlaneScene(void)
{

}

void PlaneScene::Draw(D3D& d3d)
{
    m_renderTargetTest->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());

    m_renderTargetTest->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(),
                                          1.0f, 1.0f, 1.0f, 1.0f);

    // Render all things, but the renderTarget entity.
    for(auto ent : GetEntities())
    {
        if(ent != m_drawRenderTargetEntity)
        {
            // LightComponent* light = static_cast<LightComponent*>(lights[0]);
            Component* vmcCheck = ent->GetComponent("VisualComponent", "VisualMeshComponent");
            if(vmcCheck)
            {
                // entity has a VMC.
                VisualMeshComponent* vmc = static_cast<VisualMeshComponent*>(vmcCheck);
                vmc->ShadowPass(d3d);
            }
             
            //ent->Draw(d3d);
        }
    }

    // Clear buffers to draw 3D scene.
    d3d.BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

    // Reset to drawing to back buffer.
    d3d.SetBackBufferRenderTarget();    

    // Now render 3D scene.
    Scene::Draw(d3d);
}