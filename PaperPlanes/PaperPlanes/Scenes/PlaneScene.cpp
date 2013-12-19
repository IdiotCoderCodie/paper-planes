#include "PlaneScene.h"
#include "../Assets/Shaders/Shader.h"
#include "../Assets/Textures/Texture.h"
#include "../Components/Visual/VisualMeshComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Camera/PerspectiveCamComponent.h"
#include "../Components/Light/LightComponent.h"
#include "../Components/Visual/VisualBitmapComponent.h"
#include "../Components/Physics/FreeRoamFpComponent.h"
#include "../Assets/Textures/TextureManager.h"

#include "SceneManager.h"
extern TextureManager G_TextureManager;

PlaneScene::PlaneScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr),
    m_light1shadowMap(0)
{
    D3D& d3d = GetParent().GetD3DInstance();
    // *************** TESTING. ALL THIS IN HERE IS FOR TESTING. *************** 
    
    m_light1shadowMap = new RenderTarget(&d3d.GetDevice(), 800, 600);


	Shader* shader = new Shader();

    //----------------------------------------------------------------------------------------------
    // Main inversed cube entity.
    Entity* cubeEntity = new Entity(*this, std::string("testEntity1"));

    AddEntity(cubeEntity);

    Texture* texture = G_TextureManager.LoadTexture(d3d, L"grasstex.dds", 
                                                          "GrassTexture");
	//Texture* texture = new Texture(d3d, L"Assets\\Textures\\texture_error.dds");
    VisualMeshComponent* meshComp = new VisualMeshComponent(d3d, 
                                            std::string("Assets\\Models\\cubeInv.obj"), *texture, 
                                            GetShadowMaps()); 
    
    meshComp->SetParent(*cubeEntity);
    meshComp->EnableRecieveShadows();

    cubeEntity->SetComponent(meshComp);
    cubeEntity->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f), 
                               glm::vec3(0.0f, 0.0f, 0.0f)));

    cubeEntity->SetScaleX(2.0f);
    cubeEntity->SetScaleY(2.0f);
    cubeEntity->SetScaleZ(2.0f);
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Small occluding cube.
    Entity* occluderCube = new Entity(*this, std::string("testOccluderCube"));
    AddEntity(occluderCube);

    texture = G_TextureManager.LoadTexture(d3d, L"tim.dds", "Tim");

    VisualMeshComponent* occluderCubeMesh = new VisualMeshComponent(d3d, 
                                            std::string("Assets\\Models\\sphere.obj"), *texture, 
                                            GetShadowMaps());
    occluderCubeMesh->SetParent(*occluderCube);
    occluderCubeMesh->EnableCastShadows();
    occluderCube->SetComponent(occluderCubeMesh);
    occluderCube->MoveUp(-0.0f);
    occluderCube->MoveForward(-3.0f);
    occluderCube->RotateGlobalX(180.0f);
    occluderCube->SetScaleX(0.1f);
    occluderCube->SetScaleY(0.1f);
    occluderCube->SetScaleZ(0.1f);
   /* occluderCube->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), 
                               glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f)));*/
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
                                                  glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                                                  glm::vec4(1.0f, 0.8f, 0.8f, 1.0f));
    lightComp->GenerateProjectionMatrix(1.0f, 100.0f);
    lightEntity->SetComponent(lightComp);
    //lightEntity->SetComponent(new FreeRoamFpComponent(10.0f, 50.0f, 50.0f));
    lightEntity->MoveForward(-10.0f);
    lightEntity->MoveRight(1.0f);
    AddEntity(lightEntity);
	//----------------------------------------------------------------------------------------------
    
    //----------------------------------------------------------------------------------------------
    // Light entity no.2
    lightEntity = new Entity(*this, std::string("lightEntity2"));

    lightComp = new LightComponent(glm::vec4(0.04f, 0.04f, 0.04f, 1.0f),
                                                  glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                                                  glm::vec4(0.8f, 1.0f, 0.8f, 1.0f));
    lightComp->GenerateProjectionMatrix(1.0f, 100.0f);
    lightEntity->SetComponent(lightComp);
    //lightEntity->SetComponent(new FreeRoamFpComponent(10.0f, 50.0f, 50.0f));
    lightEntity->MoveForward(-10.0f);
    lightEntity->MoveRight(-1.0f);
    AddEntity(lightEntity);
    //----------------------------------------------------------------------------------------------
    
    //----------------------------------------------------------------------------------------------
    // BITMAP for drawing what is rendered to first light's render target.
    Entity* bmpEntity = new Entity(*this, std::string("bitmapLight1ShadowMap"));
    Shader* newShader = new Shader();
    // Give bitmap the shader resource view from render target.
    Bitmap* bmp = new Bitmap(d3d, GetShadowMaps()[0]->GetShaderResourceView(), 100, 100, 
                             GetParent().GetD3DInstance().GetScreenWidth(), 
                             GetParent().GetD3DInstance().GetScreenHeight());

    bmpEntity->SetComponent(new VisualBitmapComponent(d3d, *newShader, *bmp));
    AddEntity(bmpEntity);
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Bitmap for drawing what is rendered to the second light's render target.
    bmpEntity = new Entity(*this, std::string("bitmapLight1ShadowMap"));
    //Shader* newShader = new Shader();
    // Give bitmap the shader resource view from render target.
    bmp = new Bitmap(d3d, GetShadowMaps()[1]->GetShaderResourceView(), 100, 100, 
                             GetParent().GetD3DInstance().GetScreenWidth(), 
                             GetParent().GetD3DInstance().GetScreenHeight());

    bmpEntity->SetComponent(new VisualBitmapComponent(d3d, *newShader, *bmp));
    bmpEntity->MoveRight(-101.0f);
    AddEntity(bmpEntity);
    //----------------------------------------------------------------------------------------------

    m_drawRenderTargetEntity = bmpEntity;
    
}


PlaneScene::~PlaneScene(void)
{

}

void PlaneScene::Draw(D3D& d3d)
{
    //

    //GetShadowMaps()[0]->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());

    GetShadowMaps()[0]->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(),
                                          1.0f, 1.0f, 1.0f, 1.0f);

    GetShadowMaps()[1]->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(),
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