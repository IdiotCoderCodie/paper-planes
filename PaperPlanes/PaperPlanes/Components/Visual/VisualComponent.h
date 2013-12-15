#pragma once
#include "../Component.h"
#include "../../Assets/Shaders/Shader.h"
#include "../../Assets/Shaders/ShaderManager.h"

extern ShaderManager G_ShaderManager;

class VisualComponent : public Component
{
public:
    VisualComponent();

    virtual ~VisualComponent();

    virtual void FamilyID(componentId_t& out) const { out = "VisualComponent"; }

    //virtual void Draw(D3D& d3d);

    void SetShader(Shader& shader) { m_Shader = &shader; }
    const Shader& GetShader() { return *m_Shader; }

protected:
    Shader* m_Shader;
};
