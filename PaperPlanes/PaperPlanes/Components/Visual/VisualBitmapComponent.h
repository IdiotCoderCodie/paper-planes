#pragma once
#include "VisualComponent.h"
#include "../../Assets/Textures/Texture.h"

class VisualBitmapComponent : public VisualComponent
{
private:
    struct VertexType
    {
        glm::vec3 position;
        glm::vec2 uv;
    };

public:
    VisualBitmapComponent(D3D& d3d, Shader& shader, Texture& texture, int screenWidth, 
                          int screenHeight);
    ~VisualBitmapComponent(void);

    virtual void ComponentID(componentId_t& out) const { out = "VisualBitmapComponent"; }

    void Update(float time);

    void Draw(D3D& d3d);

private:
    int m_screenWidth;
    int m_screenHeight;
    int m_width;
    int m_height;
    glm::vec3& m_prevPos;
    Texture& m_texture;
};

