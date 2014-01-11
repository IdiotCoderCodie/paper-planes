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
#include "../Components/Visual/ParticleSystemComponent.h"

#include <sstream>

#include "SceneManager.h"
extern TextureManager G_TextureManager;
extern ShaderManager G_ShaderManager;

PlaneScene::PlaneScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr),
    m_camComponent(0),
    m_airfieldCam(0),
    m_planeFollowCam(0),
    m_planeToFollow(0),
    m_prevF4Pressed(false)
{
    D3D& d3d = GetParent().GetD3DInstance();
    // *************** TESTING. ALL THIS IN HERE IS FOR TESTING. *************** 




    //----------------------------------------------------------------------------------------------
    // Main inversed cube entity.
    EntityFactory::CreateBumpMappedMeshEntity(*this, d3d, "Assets\\Models\\cubeInv.obj", 
                                              L"airfields.dds", L"airfieldNormal2.dds",
                                              GetShadowMaps(), glm::vec3(0.0f), glm::vec3(50.0f), 
                                              "mainCube");

    //EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cubeInvT.obj", 
    //                                          L"airfields.dds", /*;L"buttonFabric.dds",*/
    //                                          GetShadowMaps(), glm::vec3(0.0f), glm::vec3(100.0f), 
    //                                          "mainCube");
    //----------------------------------------------------------------------------------------------
    
   

   // //----------------------------------------------------------------------------------------------
   // // Test Occluding Sphere.
    Entity* sphere = 
    EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\sphere.obj", 
                                              L"crumpledPaper1024.dds", 
                                              GetShadowMaps(), glm::vec3(0.0f, 0.0f, -3.0f), 
                                              glm::vec3(2.5f), "sphere");
   sphere->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), 
                                                  glm::vec3(5.0f, 10.0f, 0.0f)));    
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
    // Cameras.
    float aspect = GetParent().GetD3DInstance().GetScreenWidth() 
                        / (float)GetParent().GetD3DInstance().GetScreenHeight();

    m_camComponent = 
    EntityFactory::CreatePerspectiveFpCameraEntity(*this, 60.0f, aspect, 0.1f, 1000.0f, "camera1"); 

    m_airfieldCam =
        EntityFactory::CreatePerspectiveCameraEntity(*this, 60.0f, aspect, 0.1f, 1000.0f,
            glm::vec3(-3.0f, 22.0f, 49.0f), "airfieldCam");

    m_airfieldCam->RotateGlobalY(180.0f);
    m_airfieldCam->RotateLocalX(85.0f);

    m_planeFollowCam = 
        EntityFactory::CreatePerspectiveCameraEntity(*this, 60.0f, aspect, 0.1f, 1000.0f,
            glm::vec3(0.0f), "followCam");
    //----------------------------------------------------------------------------------------------

    



    //----------------------------------------------------------------------------------------------
    // Light entity
    Entity* redLight =
    EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.02f, 0.0f, 0.0f, 1.0f),   // ambient.
                                                glm::vec4(1.0f, 0.0f, 0.0f, 0.2f),     // diffuse.
                                                glm::vec4(0.9f, 0.7f, 0.7f, 0.5f),     // specular.
                                                glm::vec3(0.0f, 0.0f, -0.0f),        // position.
                                                30.0f,
                                                16.0f,
                                                "redLight");
    redLight->RotateLocalY(-45.0f);
    redLight->RotateLocalX(-45.0f);
    redLight->MoveGlobalX(+48.0f);
    redLight->MoveGlobalY(-48.0f);
    redLight->MoveGlobalZ(-48.0f);
    //redLight->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 90.0f, 0.0f)));
	//----------------------------------------------------------------------------------------------
    
    //----------------------------------------------------------------------------------------------
    // Light entity no.2
    Entity* greenLight = 
    EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.0f, 0.02f, 0.0f, 1.0f),   // ambient.
                                            glm::vec4(0.0f, 0.35f, 0.0f, 0.2f),         // diffuse.
                                            glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),         // specular.
                                            glm::vec3(0.0f, 0.0f, 0.0f),            // position.
                                            30.0f,
                                            16.0f,
                                            "greenLight");
    greenLight->RotateLocalY(135.0f);
    greenLight->RotateLocalX(+45.0f);
    greenLight->MoveGlobalX(-48.0f);
    greenLight->MoveGlobalY(+48.0f);
    greenLight->MoveGlobalZ(+48.0f);
    //greenLight->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(20.0f, -80.0f, 0.0f)));
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Light entity no.3
    Entity* blueLight = 
    EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.0f, 0.0f, 0.02f, 1.0f), // ambient.
                                            glm::vec4(0.0f, 0.0f, 1.0f, 0.2f),        // diffuse.
                                            glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),         // specular.
                                            glm::vec3(0.0f, 0.0f, 0.0f),            // position.
                                            30.0f,
                                            16.0f,
                                            "blueLight");
    blueLight->RotateLocalY(-135.0f);
    blueLight->RotateLocalX(+45.0f);
    blueLight->MoveGlobalX(+48.0f);
    blueLight->MoveGlobalY(+48.0f);
    blueLight->MoveGlobalZ(+48.0f);
   // blueLight->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(80.0f, 0.0f, 50.0f)));
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Light entity no.4
    Entity* whiteLight = 
    EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.01f, 0.01f, 0.01f, 1.0f), // ambient.
                                            glm::vec4(0.3f, 0.3f, 0.3f, 0.2f),        // diffuse.
                                            glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),        // specular.
                                            glm::vec3(0.0f, 0.0f, 0.0f),           // position.
                                            30.0f,
                                            16.0f,
                                            "whiteLight");
    whiteLight->RotateLocalY(45.0f);
    whiteLight->RotateLocalX(-45.0f);
    whiteLight->MoveGlobalX(-48.0f);
    whiteLight->MoveGlobalY(-48.0f);
    whiteLight->MoveGlobalZ(-48.0f);
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Airport Control Towers
    Entity* tower = 
        EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"cement.dds",
        GetShadowMaps(), glm::vec3(-7.5f, -45.0f, -6.2f), glm::vec3(2.0f, 5.0f, 2.0f), "tower");
    Entity* tower2 = 
        EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"cement.dds",
        GetShadowMaps(), glm::vec3(-7.5f, 45.0f, -6.2f), glm::vec3(2.0f, 5.0f, 2.0f), "tower1");
    Entity* tower3 = 
        EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"cement.dds",
        GetShadowMaps(), glm::vec3(-7.5f, -6.2f, 45.0f), glm::vec3(2.0f, 2.0f, 5.0f), "tower2");
    Entity* tower4 = 
        EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"cement.dds",
        GetShadowMaps(), glm::vec3(-7.5f, -6.2f, -45.0f), glm::vec3(2.0f, 2.0f, 5.0f), "tower3");
    Entity* tower5 = 
        EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"cement.dds",
        GetShadowMaps(), glm::vec3(-45.0f, -7.3f, -6.2f), glm::vec3(5.0f, 2.0f, 2.0f), "tower4");
    Entity* tower6 = 
        EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"cement.dds",
        GetShadowMaps(), glm::vec3(45.0f, -7.3f, -6.2f), glm::vec3(5.0f, 2.0f, 2.0f), "tower5");
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

    LoadPlanes(d3d);
}


PlaneScene::~PlaneScene(void)
{
    
}


void PlaneScene::Update(double time)
{
    Scene::Update(time);

    if(G_InputManager.IsKeyPressed(DIK_F1))
    {
        SetActiveCamera((CameraComponent*)m_camComponent->GetComponent("CameraComponent"));
    }
    if(G_InputManager.IsKeyPressed(DIK_F2))
    {
        SetActiveCamera((CameraComponent*)m_airfieldCam->GetComponent("CameraComponent"));
    } 
    if(G_InputManager.IsKeyPressed(DIK_F3))
    {
        SetActiveCamera((CameraComponent*)m_planeFollowCam->GetComponent("CameraComponent"));
    }

    m_planeFollowCam->SetTransform(m_planes[m_planeToFollow]->GetTransform());
   /* m_planeFollowCam->SetPos(m_planes[m_planeToFollow]->GetPos());*/
    //m_planeFollowCam->MoveGlobalY(1.0f);
    m_planeFollowCam->MoveForward(-2.0f);
    m_planeFollowCam->MoveUp(0.5f);

    bool isF4Pressed = G_InputManager.IsKeyPressed(DIK_F4);
    if(isF4Pressed && !m_prevF4Pressed)
    {
        m_planeToFollow++;
        if(m_planeToFollow >= m_planes.size())
        {
            m_planeToFollow = 0;
        }
    }
    m_prevF4Pressed = isF4Pressed;
}

void PlaneScene::Draw(D3D& d3d)
{
    
    for(int i = 0; i < GetShadowMaps().size(); i++)
    {
        GetShadowMaps()[i]->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(),
                                          1.0f, 1.0f, 1.0f, 1.0f);
    }

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
    d3d.BeginScene(0.6f, 0.6f, 0.6f, 1.0f);

    // Reset to drawing to back buffer.
    d3d.SetBackBufferRenderTarget();    

    // Now render 3D scene.
    Scene::Draw(d3d);
}


void PlaneScene::LoadPlanes(D3D& d3d)
{
    const std::string filename = "Assets\\Data\\planeData.txt";
    std::ifstream input(filename);
    if(!input.is_open())
    {
        return;
    }

    std::vector<FollowPathComponent::Node> currPlaneNodes;
    std::string token;
    std::string value;
    std::istringstream value_iss;
    //bool readingNodes = false;
    glm::vec3 currPlanePos;
    int planeNum = 0;
    while(input.good())
    {
        std::getline(input, token, '=');
        std::getline(input, value);
        value_iss = std::istringstream(value);

        if(!strcmp(token.c_str(), "Plane"))
        {
            value_iss >> currPlanePos;
        }
        else if(!strcmp(token.c_str(), "Node"))
        {
            FollowPathComponent::Node newNode;
            char ch;
            value_iss >> newNode.position >> ch/*,*/ >> newNode.timeToReach >> ch >> newNode.delay;
            currPlaneNodes.push_back(newNode);
        }
        else if(!strcmp(token.c_str(), "EndPlane"))
        {
            Entity* plane = EntityFactory::CreatePaperPlaneEntity(*this, d3d, currPlanePos, GetShadowMaps(),
                                                      currPlaneNodes, 
                                                      "Plane" + std::to_string(planeNum));
            planeNum++;
            currPlaneNodes.clear();
            m_planes.push_back(plane);
               // readingNodes = false;
        }
    }
}
