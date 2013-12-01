#include "PerspectiveCamComponent.h"


PerspectiveCamComponent::PerspectiveCamComponent(void)
    : m_fov(45.0f), m_aspect(600.0f / 400.0f), m_near(0.01f), m_far(1000.0f)
{
    CameraComponent::SetProjMatrix(glm::perspective(m_fov, m_aspect, m_near, m_far));
}


PerspectiveCamComponent::PerspectiveCamComponent(float fov, float aspect, float near, float far)
    : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
{
    CameraComponent::SetProjMatrix(glm::perspective(m_fov, m_aspect, m_near, m_far));
}


PerspectiveCamComponent::~PerspectiveCamComponent(void)
{
}

