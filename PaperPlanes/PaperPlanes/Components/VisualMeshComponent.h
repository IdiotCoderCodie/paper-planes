#pragma once
#include "VisualComponent.h"
#include "StaticMesh.h"
#include <DirectXMath.h>

class VisualMeshComponent : public VisualComponent
{
struct MatrixBufferFormat
{
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};


public:
    VisualMeshComponent(D3D& d3d, const std::string& filename, Shader& shader);
    ~VisualMeshComponent(void);

    virtual void ComponentID(componentId_t& out) const { out = "VisualMeshComponent"; }

    virtual void Update(float timeElapsed);

    virtual void Draw(D3D& d3d);

private:
    StaticMesh m_mesh;
};

