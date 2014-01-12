#pragma once
#include "Entity.h"
#include "../RenderTarget.h"
#include "../Components/Physics/FollowPathComponent.h"
#include <string>
#include <vector>


namespace EntityFactory
{
//public:
    /** 
     * Creates an entity with a PerspectiveCamComponent and FreeRoamFpComponent attached, and adds
     * it to 'scene'
     * Returns: A pointer to the newly created Entity.
     */
    Entity* CreatePerspectiveFpCameraEntity(Scene& scene, float fov, float aspect, 
                                                   float camNear, float camFar, 
                                                   const std::string& id);

    /**
     * Creates an entity with a PerspectiveCamComponent attached.
     */
    Entity* CreatePerspectiveCameraEntity(Scene& scene, float fov, float aspect, 
                                                  float near, float far, 
                                                  glm::vec3& position, const std::string& id);

    /**
     * Creates an entity with a point light component attached, and adds it to 'scene' at position
     * 'position'.
     * Returns: A pointer to the newly created entity.
     */ 
    Entity* CreatePointlightEntity(Scene& scene, const glm::vec4& ambient, 
                                          const glm::vec4& diffuse, const glm::vec4& specular, 
                                          const glm::vec3& position,
                                          const std::string& id);
    /**
     * Creates an entity with a point light component attached, and adds it to 'scene' at position
     * (0, 0, 0).
     * Returns: A pointer to the newly created entity.
     */ 
    Entity* CreatePointlightEntity(Scene& scene, const glm::vec4& ambient, 
                                          const glm::vec4& diffuse, const glm::vec4& specular,
                                          const std::string& id);/*
    {
        return CreatePointlightEntity(scene, ambient, diffuse, specular, glm::vec3(0.0f), id);
    }*/

    Entity* CreateSpotlightEntity(Scene& scene, const glm::vec4& ambient, 
                                              const glm::vec4& diffuse, const glm::vec4& specular,
                                              const glm::vec3& position, float spotCutoff, 
                                              float spotExponent,
                                              const std::string& id);


    Entity* CreateMeshEntity(Scene& scene, D3D& d3d, const std::string& objFilename, 
                                    WCHAR* textureName, std::vector<RenderTarget*>& shadowMaps,
                                    const glm::vec3& position, const glm::vec3& scale,
                                    const std::string& id);

    Entity* CreateBumpMappedMeshEntity(Scene& scene, D3D& d3d, 
                                              const std::string& objFilename, WCHAR* textureName, 
                                              WCHAR* bmpMapTextureName, 
                                              std::vector<RenderTarget*>& shadowMaps,
                                              const glm::vec3& position, const glm::vec3& scale,
                                              const std::string& id);

    Entity* CreateBmpEntity(Scene& scene, D3D& d3d, ID3D11ShaderResourceView* srcTexture,
                                       int width, int height, int screenWidth, int screenHeight,
                                       const std::string& id);

    Entity* CreateBmpEntity(Scene& scene, D3D& d3d, ID3D11ShaderResourceView* srcTexture,
                                       int width, int height, int screenWidth, int screenHeight,
                                       int xPos, int yPos, const std::string& id);

    Entity* CreateParticleSystemEntity(Scene& scene, D3D& d3d, const std::string& id);

    Entity* CreateParticleSystemEntity(Scene& scene, D3D& d3d, const std::string& effectFile,
                                              const std::string& id);


    Entity* CreatePaperPlaneEntity(Scene& scene, D3D& d3d, glm::vec3& position,
                                          std::vector<RenderTarget*>& shadowMaps,
                                          std::vector<FollowPathComponent::Node>& pathNodes,
                                          const std::string& id);

};
