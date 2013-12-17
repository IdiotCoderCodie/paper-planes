#pragma once
#include "../Component.h"

#include "../../glm/glm.hpp"

struct LightBufferStruct
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float specularPow;

};

class LightComponent : public Component
{
public:
    LightComponent(void);
    LightComponent(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular);
    virtual ~LightComponent(void);

    void FamilyID(componentId_t& out) const     { out = "LightComponent"; }
    void ComponentID(componentId_t& out) const  { out = "SpotLightComponent"; } //Temporary.?

    const glm::vec4& GetAmbient() const                     { return m_ambient; }
    void SetAmbient(const glm::vec4 ambient)                { m_ambient = ambient; }
    void SetAmbient(float r, float g, float b, float a)     { m_ambient = glm::vec4(r, g, b, a); }

    const glm::vec4& GetDiffuse() const                     { return m_diffuse; }
    void SetDiffuse(const glm::vec4& diff)                  { m_diffuse = diff; }
    void SetDiffuse(float r, float g, float b, float a)     { m_diffuse = glm::vec4(r, g, b, a); }

    const glm::vec4& GetSpecular() const                    { return m_specular; }
    void SetSpecular(const glm::vec4& spec)                 { m_specular = spec; }
    void SetSpecular(float r, float g, float b, float a)    { m_specular = glm::vec4(r, g, b, a); }

    bool IsActive() const   { return m_active; }
    void Enable()           { m_active = true; }
    bool Disable()          { m_active = false; }

    // Functions for shadow mapping.

    void GenerateProjectionMatrix(float near, float far);

    const glm::mat4& GetViewMatrix() { return m_viewMatrix; }
    const glm::mat4& GetProjMatrix() { return m_projectionMatrix; }

    virtual void Update(float time);
    

private:
    bool m_active;
    glm::vec4 m_ambient;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    
    // Used for shadow mapping.
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;


};

