#include "VisualMeshComponent.h"
#include "../Entities/Entity.h"
#include "../Scenes/Scene.h"
#include "../Scenes/SceneManager.h"
#include "Camera\CameraComponent.h"

#include "../glm/gtc/matrix_transform.hpp"

VisualMeshComponent::VisualMeshComponent(D3D& d3d, const std::string& filename, Shader& shader)
    :   VisualComponent(shader),
        m_mesh(filename, d3d) // Eurgh.
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	polygonLayout[0].SemanticName           = "POSITION";
	polygonLayout[0].SemanticIndex          = 0;
	polygonLayout[0].Format                 = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot              = 0;
	polygonLayout[0].AlignedByteOffset      = 0;
	polygonLayout[0].InputSlotClass         = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate   = 0;
    
    polygonLayout[1].SemanticName           = "TEXCOORD";
	polygonLayout[1].SemanticIndex          = 0;
	polygonLayout[1].Format                 = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot              = 0;
	polygonLayout[1].AlignedByteOffset      = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass         = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate   = 0;

	polygonLayout[2].SemanticName           = "NORMAL";
	polygonLayout[2].SemanticIndex          = 0;
	polygonLayout[2].Format                 = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot              = 0;
	polygonLayout[2].AlignedByteOffset      = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass         = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate   = 0;

	// Set vertex shader and input layout.
	m_Shader.SetVertexShader(d3d, 0,
                             L"Assets\\Shaders\\color_vs.hlsl", "vp_main", "vs_5_0", polygonLayout, 3);
	// Set pixel shader.
	m_Shader.SetPixelShader(d3d, 0,
                            L"Assets\\Shaders\\color_ps.hlsl", "ps_main", "ps_5_0");

	// Add matrix buffer.
	m_Shader.AddBuffer(d3d, "MatrixBuffer", 
                       D3D11_USAGE_DYNAMIC, sizeof(glm::mat4) * 3, D3D11_BIND_CONSTANT_BUFFER, 
                       D3D11_CPU_ACCESS_WRITE, 0, 0);
	
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

    MatrixBufferFormat matBuffer;
    matBuffer.modelMatrix       = glm::transpose(
                                    GetParent().GetTransform().GetMatrix());
    matBuffer.viewMatrix        = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetViewMatrix());
    matBuffer.projectionMatrix  = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetProjMatrix());

    m_Shader.VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matBuffer, sizeof(matBuffer), 0);

    int test = m_mesh.GetIndexCount();
    m_Shader.RenderShader(d3d, m_mesh.GetIndexCount());
}
