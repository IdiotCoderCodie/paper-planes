#include "VisualMeshComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../Camera/CameraComponent.h"
#include "../Light/LightComponent.h"

#include "../../glm/gtc/matrix_transform.hpp"

VisualMeshComponent::VisualMeshComponent(D3D& d3d, const std::string& filename, Shader& shader, Texture& texture)
    :   VisualComponent(shader),
        m_mesh(filename, d3d), 
		m_texture(texture)
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
	m_Shader.SetVertexShader(d3d, 0, L"Assets\\Shaders\\color_vs.hlsl", "vp_main", "vs_5_0", 
                             polygonLayout, 3);
	// Set pixel shader.
	m_Shader.SetPixelShader(d3d, 0,
                            L"Assets\\Shaders\\color_ps.hlsl", "ps_main", "ps_5_0");

	// Add matrix buffer.
	m_Shader.AddBuffer(d3d, "MatrixBuffer", 
                       D3D11_USAGE_DYNAMIC, sizeof(glm::mat4) * 3, D3D11_BIND_CONSTANT_BUFFER, 
                       D3D11_CPU_ACCESS_WRITE, 0, 0);

    // Add camera buffer.
    m_Shader.AddBuffer(d3d, "CameraBuffer", D3D11_USAGE_DYNAMIC, sizeof(CameraBufferFormat), 
                       D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    // Add light color buffer.
    m_Shader.AddBuffer(d3d, "LightColorBuffer", D3D11_USAGE_DYNAMIC, sizeof(LightColorBufferFormat),
                       D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    // Add light position buffer.
    m_Shader.AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC, sizeof(glm::vec4), 
                       D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	// Sample State.
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR; 
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias		= 0.0f;
	samplerDesc.MaxAnisotropy	= 4;
	samplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0]	= 0;
	samplerDesc.BorderColor[1]	= 0;
	samplerDesc.BorderColor[2]	= 0;
	samplerDesc.BorderColor[3]	= 0;
	samplerDesc.MinLOD			= 0;
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	if(m_Shader.SetSampleState(d3d, samplerDesc) == false)
		assert(true);
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

    //----------------------------------------------------------------------------------------------
    // Get matrices and put in buffer format.
    MatrixBufferFormat matBuffer;
    matBuffer.modelMatrix       = glm::transpose(
                                    GetParent().GetTransform().GetMatrix());
    matBuffer.viewMatrix        = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetViewMatrix());
    matBuffer.projectionMatrix  = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetProjMatrix());
    // Set the buffer data using above matrices.
    m_Shader.VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matBuffer, sizeof(matBuffer), 0);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get light from the scene.
    const std::vector<Component*>& lights = GetParent().GetParent().GetLights();
    // Get first light.
    if(lights.size() > 0)
    {
        LightComponent* light = static_cast<LightComponent*>(lights[0]);
        LightColorBufferFormat firstLight = 
        { 
            light->GetAmbient(),
            light->GetDiffuse(),
            light->GetSpecular(),
            40.0f,
            glm::vec3(0.0f, 0.0f, 0.0f)
        };

        m_Shader.PSSetConstBufferData(d3d, std::string("LightColorBuffer"), 
            (void*)&firstLight, sizeof(firstLight), 0);

        // Get pos and send to buffer.
        glm::vec4 lightPos(light->GetParent().GetPos(), 0.0f);

        m_Shader.VSSetConstBufferData(d3d, std::string("LightPositionBuffer"), 
            (void*)&lightPos, sizeof(glm::vec4), 1);
    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get active camera and put data in CameraBufferFormat, then send to shader.
    const CameraBufferFormat cam = 
    { 
        GetParent().GetParent().GetActiveCamera()->GetParent().GetPos(), 
        0.0f 
    };

    m_Shader.VSSetConstBufferData(d3d, std::string("CameraBuffer"), (void*)&cam, sizeof(cam), 2);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get texture for this model and set for shader.
	ID3D11ShaderResourceView* tex = m_texture.GetTexture();
	d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    // Render sahder.
    m_Shader.RenderShader(d3d, m_mesh.GetIndexCount());
}
