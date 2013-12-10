#pragma once
#include "VisualComponent.h"
#include "../../Assets/Textures/Texture.h"
#include "../../Assets/Textures/Bitmap.h"

class VisualBitmapComponent : public VisualComponent
{
private:
    struct VertexType
    {
        glm::vec3 position;
        glm::vec2 uv;
    };

public:
    VisualBitmapComponent(D3D& d3d, Shader& shader, Bitmap& bitmap);
    ~VisualBitmapComponent(void);

    virtual void ComponentID(componentId_t& out) const { out = "VisualBitmapComponent"; }

    virtual void Update(float time);

    virtual void Draw(D3D& d3d);

private:
    Bitmap& m_bitmap;
};

