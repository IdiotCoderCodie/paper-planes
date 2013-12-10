#include "VisualComponent.h"


VisualComponent::VisualComponent()
    : m_Shader()
{
}


VisualComponent::VisualComponent(Shader& shader) 
    : m_Shader(shader)
{
}


VisualComponent::~VisualComponent()
{
}
