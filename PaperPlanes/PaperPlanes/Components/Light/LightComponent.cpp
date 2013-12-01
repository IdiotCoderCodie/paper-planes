#include "LightComponent.h"


LightComponent::LightComponent(void)
    : m_ambient(0.0f, 0.0f, 0.0f, 1.0f), 
      m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
      m_specular(1.0f, 1.0f, 1.0f, 1.0f),
      m_active(true)
{
}

LightComponent::LightComponent(const glm::vec4& ambient, const glm::vec4& diffuse, 
                               const glm::vec4& specular)
    : m_ambient(ambient), 
      m_diffuse(diffuse),
      m_specular(specular),
      m_active(true)
{
}



LightComponent::~LightComponent(void)
{
}
