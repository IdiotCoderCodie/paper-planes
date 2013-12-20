#include "EntityFactory.h"
#include "../Assets/Textures/Texture.h"
#include "../Assets/Textures/TextureManager.h"
#include "../Scenes/Scene.h"
#include "../Components/Camera/PerspectiveCamComponent.h"
#include "../Components/Physics/FreeRoamFpComponent.h"
#include "../Components/Light/LightComponent.h"
#include "../Components/Visual/VisualMeshComponent.h"


extern TextureManager G_TextureManager;
Entity* EntityFactory::CreatePerspectiveFpCameraEntity(Scene& scene, float fov, float aspect, 
                                                     float pNear, float pFar, const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    newEntity->SetComponent(new PerspectiveCamComponent(fov, aspect, pNear, pFar));
    newEntity->SetComponent(new FreeRoamFpComponent(10.0f, 50.0f, 50.0f));
    scene.AddEntity(newEntity);
    newEntity->MoveForward(-10.0f);
    return newEntity;
}


Entity* EntityFactory::CreatePointlightEntity(Scene& scene, const glm::vec4& ambient, 
                                              const glm::vec4& diffuse, const glm::vec4& specular,
                                              const glm::vec3& position,
                                              const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    LightComponent* lightComp = new LightComponent(ambient, diffuse, specular);
    lightComp->GenerateProjectionMatrix(1.0f, 100.0f);
    newEntity->SetComponent(lightComp);
    scene.AddEntity(newEntity);
    newEntity->MoveForward(position.z);
    newEntity->MoveRight(position.x);
    newEntity->MoveUp(position.y);

    return newEntity;
}


Entity* EntityFactory::CreateMeshEntity(Scene& scene, D3D& d3d, const std::string& objFilename, 
                                        WCHAR* textureName, std::vector<RenderTarget*>& shadowMaps,
                                        const glm::vec3& position, const glm::vec3& scale,
                                        const std::string& id)
{
    // Create the entity.
    Entity* newEntity = new Entity(scene, id);

    // Get the standard string from WCHAR*.
    std::wstring ws(textureName);
    std::string  ssTexName(ws.begin(), ws.end());

    // Check if texture is already loaded...
    Texture* tex = G_TextureManager.GetTexture(ssTexName);
    if(!tex)
    {
        // It's not, so load it.
        tex = G_TextureManager.LoadTexture(d3d, textureName, ssTexName);
    }

    // Create the mesh component, enable shadows (both cast and recieve).
    VisualMeshComponent* mesh = new VisualMeshComponent(d3d, objFilename, *tex, shadowMaps);
    mesh->EnableCastShadows();
    mesh->EnableRecieveShadows();
    newEntity->SetComponent(mesh);

    // Move to requested position.
    newEntity->MoveForward(position.z);
    newEntity->MoveRight(position.x);
    newEntity->MoveUp(position.y);

    // Scale to requested scale.
    newEntity->SetScaleX(scale.x);
    newEntity->SetScaleY(scale.y);
    newEntity->SetScaleZ(scale.z);

    // Add to the scene.
    scene.AddEntity(newEntity);

    return newEntity;
}
