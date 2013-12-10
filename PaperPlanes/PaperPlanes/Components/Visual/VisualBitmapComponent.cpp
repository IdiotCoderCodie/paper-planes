#include "VisualBitmapComponent.h"


VisualBitmapComponent::VisualBitmapComponent(D3D& d3d, Shader& shader, Texture& texture, 
    int screenWidth, int screenHeight)
    :	VisualComponent(shader),
    m_screenWidth(screenWidth),
    m_screenHeight(screenHeight),
    m_prevPos(glm::vec3(0.2342, -123433, 3434)),
    m_texture(texture)	
{
    

}


VisualBitmapComponent::~VisualBitmapComponent(void)
{
}
