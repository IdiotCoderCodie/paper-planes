#pragma once
#include "VisualComponent.h"
#include "StaticMesh.h"

class VisualMeshComponent : public VisualComponent
{
public:
    VisualMeshComponent(const std::string& filename, Shader shader);
    ~VisualMeshComponent(void);

    virtual void ComponentID(componentId_t& out) const { out = "VisualMeshComponent"; }

    virtual void Update(float timeElapsed);

    virtual void Draw(D3D& d3d);

private:
    StaticMesh m_mesh;
};

