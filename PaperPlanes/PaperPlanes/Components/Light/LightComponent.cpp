#include "LightComponent.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/constants.hpp"
#include "../../Entities/Entity.h"

LightComponent::LightComponent(void)
    : m_active(true),
      m_ambient(0.0f, 0.0f, 0.0f, 1.0f), 
      m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
      m_specular(1.0f, 1.0f, 1.0f, 1.0f)
      
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


void LightComponent::GenerateProjectionMatrix(float near, float far)
{
    float fieldOfView  = 90.0f;
    float screenAspect = 1.0f;

    glm::mat4 ogl_to_d3d = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
    ogl_to_d3d = glm::scale(ogl_to_d3d, glm::vec3(1.0f, 1.0f, 0.5f));

    m_projectionMatrix = ogl_to_d3d * glm::perspective(fieldOfView, screenAspect, near, far);
}



void LightComponent::Update(float time)
{
    Component::Update(time);
    
    const Frame& transform = GetParent().GetTransform();
    m_viewMatrix = glm::lookAt(transform.GetPosition(),
                               transform.GetPosition() + transform.GetForward(),
                               transform.GetUp());
}