#include "VisualBitmapComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../../glm/gtc/matrix_transform.hpp"

VisualBitmapComponent::VisualBitmapComponent(D3D& d3d, Shader& shader, Bitmap& bitmap)
    :	VisualComponent(shader),
    m_bitmap(bitmap)
{

    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
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

    m_Shader.SetVertexShader(d3d, 0, L"Assets\\Shaders\\bitmap_vs.hlsl", "vp_main", "vs_5_0",
                             polygonLayout, 2);

    // Set pixel shader.
    m_Shader.SetPixelShader(d3d, 0, L"Assets\\Shaders\\bitmap_ps.hlsl", "ps_main", "ps_5_0");

    m_Shader.AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC, sizeof(glm::mat4) * 3, 
                       D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

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
    {
        assert(true);
    }
}


VisualBitmapComponent::~VisualBitmapComponent(void)
{

}


void VisualBitmapComponent::Update(float time)
{
    // Check if updating position. If so need to update bitmap buffers.
    m_bitmap.UpdateBuffers(GetParent().GetParent().GetParent().GetD3DInstance(), 
                           (int)GetParent().GetPos().x, (int)GetParent().GetPos().y);
}

void VisualBitmapComponent::Draw(D3D& d3d)
{
    d3d.TurnZBufferOff();
    // Bind bitmap stuff to the pipeline.
    m_bitmap.Draw(d3d);

    // NOTE: need to get the actual screen width and height.
    bitmap::MatrixBufferStruct matBuffer = 
    { 
        glm::mat4(2.0f / 800.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 2.0f / 600.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f/(100.0f), 0.0f,
                  0.0f, 0.0f, -0.0f/(100.0f - 0.0f), 1.0f)
        //glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, 0.0f, 100.0f) 
    };

    m_Shader.VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matBuffer, sizeof(matBuffer), 0);

    ID3D11ShaderResourceView* tex = m_bitmap.GetTexture().GetTexture();
    d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);

    m_Shader.RenderShader(d3d, 6);
    d3d.TurnZBufferOn();
}