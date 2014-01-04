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
#include "../Entities/EntityFactory.h"

#include "SceneManager.h"
extern TextureManager G_TextureManager;

PlaneScene::PlaneScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();
    // *************** TESTING. ALL THIS IN HERE IS FOR TESTING. *************** 

    //----------------------------------------------------------------------------------------------
    // Main inversed cube entity.
    EntityFactory::CreateBumpMappedMeshEntity(*this, d3d, "Assets\\Models\\cubeInv.obj", 
                                              L"grasstex.dds", L"rockwall_normal.dds",
                                              GetShadowMaps(), glm::vec3(0.0f), glm::vec3(4.0f), 
                                              "mainCube");
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Test Occluding Sphere.
    Entity* sphere = 
    EntityFactory::CreateBumpMappedMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"grasstex.dds", L"rockwall_normal.dds",
        GetShadowMaps(), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(1.0f), "occluderSphere");
    sphere->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), 
                                                  glm::vec3(0.0f, 10.0f, 0.0f)));
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Camera entity.
    float aspect = GetParent().GetD3DInstance().GetScreenWidth() 
                        / (float)GetParent().GetD3DInstance().GetScreenHeight();
    EntityFactory::CreatePerspectiveFpCameraEntity(*this, 60.0f, aspect, 0.1f, 100.0f, "camera1"); 
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Light entity
    Entity* redLight =
    EntityFactory::CreatePointlightEntity(*this, glm::vec4(0.02f, 0.02f, 0.02f, 1.0f), 
                                                glm::vec4(1.0f, 0.3f, 0.3f, 1.0f), 
                                                glm::vec4(0.9f, 0.7f, 0.7f, 0.5f), 
                                                glm::vec3(0.0f, 0.0f, -10.0f),
                                                "redLight");
	//----------------------------------------------------------------------------------------------
    
    //----------------------------------------------------------------------------------------------
    // Light entity no.2
    Entity* greenLight = 
    EntityFactory::CreatePointlightEntity(*this, glm::vec4(0.02f, 0.02f, 0.02f, 1.0f), 
                                            glm::vec4(0.2f, 0.7f, 0.2f, 0.5f), 
                                            glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),
                                            glm::vec3(-1.0f, 0.0f, -10.0f),
                                            "greenLight");
    //----------------------------------------------------------------------------------------------
    
    //----------------------------------------------------------------------------------------------
    // BITMAP for drawing what is rendered to first light's render target.
    EntityFactory::CreateBmpEntity(*this, d3d, GetShadowMaps()[0]->GetShaderResourceView(), 
                                   100, 100, GetParent().GetD3DInstance().GetScreenWidth(), 
                                   GetParent().GetD3DInstance().GetScreenHeight(),
                                   "light1ShadowMapBmp");
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Bitmap for drawing what is rendered to the second light's render target.
    EntityFactory::CreateBmpEntity(*this, d3d, GetShadowMaps()[1]->GetShaderResourceView(),
                                   100, 100, GetParent().GetD3DInstance().GetScreenWidth(),
                                   GetParent().GetD3DInstance().GetScreenHeight(), 101, 0,
                                   "light2ShadowMapBmp");
    //----------------------------------------------------------------------------------------------  
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
