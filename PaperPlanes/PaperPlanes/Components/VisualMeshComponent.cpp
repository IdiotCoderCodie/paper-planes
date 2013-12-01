#include "VisualMeshComponent.h"
#include "../Entities/Entity.h"
#include "../Scenes/Scene.h"
#include "../Scenes/SceneManager.h"

VisualMeshComponent::VisualMeshComponent(const std::string& filename, Shader shader)
    :   m_mesh(filename, GetParent().GetParent().GetParent().GetD3DInstance()), // Eurgh.
        VisualComponent(shader)
{
}


VisualMeshComponent::~VisualMeshComponent(void)
{
}


void VisualMeshComponent::Update(float timeElapsed)
{
}


void VisualMeshComponent::Draw(D3D& d3d)
{
    m_mesh.Render(d3d);
    
    // Set shader parameters before rendering.
    //m_Shader.SetParam();
    m_Shader.RenderShader(d3d, m_mesh.GetIndexCount());
    //m_Shader.RenderShader(d3d, m_mesh.GetIndexCount());
}
