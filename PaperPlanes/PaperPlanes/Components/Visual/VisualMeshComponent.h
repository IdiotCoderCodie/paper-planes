#pragma once
#include "VisualComponent.h"
#include "../../Assets/Models/StaticMesh.h"
#include "../../Assets/Textures/Texture.h"
#include <DirectXMath.h>

class VisualMeshComponent : public VisualComponent
{

// Should probably remove these, put them in some kind of header.
struct MatrixBufferFormat
{
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};

struct CameraBufferFormat
{
    glm::vec3 cameraPos;
    float padding;
};

struct LightColorBufferFormat
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float specPow;
    glm::vec3 padding;
};


public:
    VisualMeshComponent(D3D& d3d, const std::string& filename, Shader& shader, Texture& texture);
    ~VisualMeshComponent(void);

    virtual void ComponentID(componentId_t& out) const { out = "VisualMeshComponent"; }

    virtual void Update(float timeElapsed);

    virtual void Draw(D3D& d3d);

private:
    StaticMesh m_mesh;
	Texture& m_texture;
};

