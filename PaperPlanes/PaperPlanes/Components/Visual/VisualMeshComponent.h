#pragma once
#include "VisualComponent.h"
#include "../../Assets/Models/StaticMesh.h"
#include "../../Assets/Textures/Texture.h"
#include <DirectXMath.h>

class VisualMeshComponent : public VisualComponent
{

public:
    VisualMeshComponent(D3D& d3d, const std::string& filename, Texture& texture);
    ~VisualMeshComponent(void);

    virtual void ComponentID(componentId_t& out) const { out = "VisualMeshComponent"; }

    void SetCastShadows() { m_castShadows = true; }

    virtual void Update(float timeElapsed);

    virtual void ShadowPass() {};
    virtual void Draw(D3D& d3d);

private:
    StaticMesh m_mesh;
	Texture& m_texture;
    bool m_castShadows;
};

