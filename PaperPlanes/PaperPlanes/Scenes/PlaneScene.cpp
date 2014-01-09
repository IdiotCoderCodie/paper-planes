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
#include "../Components/Physics/FollowPathComponent.h"
#include "../Components/Collision/CollisionComponent.h"


#include "SceneManager.h"
extern TextureManager G_TextureManager;
extern ShaderManager G_ShaderManager;

PlaneScene::PlaneScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();
    // *************** TESTING. ALL THIS IN HERE IS FOR TESTING. *************** 




    //----------------------------------------------------------------------------------------------
    // Main inversed cube entity.
    EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cubeInv.obj", 
                                              L"grassTex.dds",/* L"buttonFabric.dds",*/
                                              GetShadowMaps(), glm::vec3(0.0f), glm::vec3(50.0f), 
                                              "mainCube");
    //----------------------------------------------------------------------------------------------
    
    ////----------------------------------------------------------------------------------------------
    //// Testing FollowPathComponent
    //Entity* follower =
    //EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"crumpledPaper1024.dds",
    //    GetShadowMaps(), glm::vec3(0.0f), glm::vec3(1.0f), "cubeFollowingPath");
    //FollowPathComponent* pathComp = new FollowPathComponent();
    //follower->SetComponent(pathComp);
    //
    //int i = 0;
    //Entity* newNode = EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"tim.dds",
    //                                    GetShadowMaps(), glm::vec3(0.0f), 
    //                                    glm::vec3(0.1f),
    //                                    "Node" + std::to_string(i));
    //i++;
    //pathComp->AddNode(newNode->GetPos(), 5.0f, 3.0f);
    //newNode =  EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"tim.dds",
    //                                    GetShadowMaps(), glm::vec3(5.0f, 0.0f, 0.0f), 
    //                                    glm::vec3(0.1f),
    //                                    "Node" + std::to_string(i));
    //i++;
    //pathComp->AddNode(newNode);
    //newNode =  EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"tim.dds",
    //                                    GetShadowMaps(), glm::vec3(5.0f, 5.0f, 0.0f), 
    //                                    glm::vec3(0.1f),
    //                                    "Node" + std::to_string(i));
    //i++;
    //pathComp->AddNode(newNode);
    //newNode =  EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"tim.dds",
    //                                    GetShadowMaps(), glm::vec3(0.0f, 5.0f, 0.0f), 
    //                                    glm::vec3(0.1f),
    //                                    "Node" + std::to_string(i));
    //i++;
    //pathComp->AddNode(newNode->GetPos(), 5.0f, 3.0f);
    //newNode =  EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"tim.dds",
    //                                    GetShadowMaps(), glm::vec3(0.0f, 10.0f, 5.0f), 
    //                                    glm::vec3(0.1f),
    //                                    "Node" + std::to_string(i));
    //i++;
    //pathComp->AddNode(newNode);
    //newNode =  EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"tim.dds",
    //                                    GetShadowMaps(), glm::vec3(5.0f, 10.0f, 5.0f), 
    //                                    glm::vec3(0.1f),
    //                                    "Node" + std::to_string(i));
    //i++;
    //pathComp->AddNode(newNode);
    //newNode =  EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"tim.dds",
    //                                    GetShadowMaps(), glm::vec3(5.0f, 10.0f, 10.0f), 
    //                                    glm::vec3(0.1f),
    //                                    "Node" + std::to_string(i));
    //i++;
    //pathComp->AddNode(newNode->GetPos(), 5.0f, 2.0f);

    ////----------------------------------------------------------------------------------------------


   // //----------------------------------------------------------------------------------------------
   // // Test Occluding Sphere.
   // Entity* sphere = 
   // EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\sphere.obj", 
   //                                           L"crumpledPaper1024.dds",
   //                                           GetShadowMaps(), glm::vec3(0.0f, 0.0f, -3.0f), 
   //                                           glm::vec3(2.5f), "sphere");
   ///* sphere->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), 
   //                                               glm::vec3(5.0f, 10.0f, 0.0f)));    */
   // CollisionComponentDesc desc =
   // {
   //     CollisionType::BoundingSphere,
   //     2.5f,
   // };
   // sphere->SetComponent(new CollisionComponent(desc));

   // Entity* sphere2 = 
   // EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\sphere.obj", 
   //                                           L"crumpledPaper1024.dds",
   //                                           GetShadowMaps(), glm::vec3(7.0f, 0.0f, -3.0f), 
   //                                           glm::vec3(1.0f), "sphere2");
   // 
   // desc.radius = 1.0f;

   // sphere2->SetComponent(new CollisionComponent(desc));
   // //----------------------------------------------------------------------------------------------      


    


    //----------------------------------------------------------------------------------------------
    // Testing PaperPlaneEntity
    EntityFactory::CreatePaperPlaneEntity(*this, d3d, glm::vec3(0.0f, 0.0f, 0.0f), GetShadowMaps(),
                                          "testPlane1");
    EntityFactory::CreatePaperPlaneEntity(*this, d3d, glm::vec3(5.0f, 0.0f, 0.0f), GetShadowMaps(),
                                          "testPlane2");
    //----------------------------------------------------------------------------------------------

    ////----------------------------------------------------------------------------------------------
    //// Test Particle System
    //EntityFactory::CreateParticleSystemEntity(*this, d3d, "flameParticleEffect.txt", 
    //                                          "particleSystem");
    ////----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Camera entity.
    float aspect = GetParent().GetD3DInstance().GetScreenWidth() 
                        / (float)GetParent().GetD3DInstance().GetScreenHeight();
    EntityFactory::CreatePerspectiveFpCameraEntity(*this, 60.0f, aspect, 0.1f, 1000.0f, "camera1"); 
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Light entity
    Entity* redLight =
    EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.02f, 0.0f, 0.0f, 1.0f),   // ambient.
                                                glm::vec4(0.35f, 0.0f, 0.0f, 0.2f),     // diffuse.
                                                glm::vec4(0.9f, 0.7f, 0.7f, 0.5f),     // specular.
                                                glm::vec3(5.0f, 0.0f, -25.0f),        // position.
                                                30.0f,
                                                16.0f,
                                                "redLight");

    //redLight->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 90.0f, 0.0f)));
	//----------------------------------------------------------------------------------------------
    
    //----------------------------------------------------------------------------------------------
    // Light entity no.2
    Entity* greenLight = 
    EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.0f, 0.02f, 0.0f, 1.0f),   // ambient.
                                            glm::vec4(0.0f, 0.35f, 0.0f, 0.2f),         // diffuse.
                                            glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),         // specular.
                                            glm::vec3(-5.0f, 0.0f, -25.0f),            // position.
                                            30.0f,
                                            16.0f,
                                            "greenLight");
    //greenLight->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(20.0f, -80.0f, 0.0f)));
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Light entity no.3
    Entity* blueLight = 
    EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.0f, 0.0f, 0.02f, 1.0f), // ambient.
                                            glm::vec4(0.0f, 0.0f, 0.35f, 0.2f),        // diffuse.
                                            glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),         // specular.
                                            glm::vec3(0.0f, +5.0f, -25.0f),            // position.
                                            30.0f,
                                            16.0f,
                                            "blueLight");
   // blueLight->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(80.0f, 0.0f, 50.0f)));
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Light entity no.4
    Entity* whiteLight = 
    EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.01f, 0.01f, 0.01f, 1.0f), // ambient.
                                            glm::vec4(0.1f, 0.1f, 0.1f, 0.2f),        // diffuse.
                                            glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),         // specular.
                                            glm::vec3(0.0f, -5.0f, -25.0f),            // position.
                                            30.0f,
                                            16.0f,
                                            "whiteLight");
   // whiteLight->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(-80.0f, 40.0f, -20.0f)));
    //----------------------------------------------------------------------------------------------
    
    ////----------------------------------------------------------------------------------------------
    //// BITMAP for drawing what is rendered to first light's render target.
    //EntityFactory::CreateBmpEntity(*this, d3d, GetShadowMaps()[0]->GetShaderResourceView(), 
    //                               100, 100, GetParent().GetD3DInstance().GetScreenWidth(), 
    //                               GetParent().GetD3DInstance().GetScreenHeight(),
    //                               "light1ShadowMapBmp");
    ////----------------------------------------------------------------------------------------------

    ////----------------------------------------------------------------------------------------------
    //// Bitmap for drawing what is rendered to the second light's render target.
    //EntityFactory::CreateBmpEntity(*this, d3d, GetShadowMaps()[1]->GetShaderResourceView(),
    //                               100, 100, GetParent().GetD3DInstance().GetScreenWidth(),
    //                               GetParent().GetD3DInstance().GetScreenHeight(), 101, 0,
    //                               "light2ShadowMapBmp");
    ////----------------------------------------------------------------------------------------------  
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
