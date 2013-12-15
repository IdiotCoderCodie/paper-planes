#include "ShaderManager.h"
#include "ShaderResources\poly_layouts.h"
#include "ShaderResources\constant_buffers.h"
#include "ShaderResources\sampler_desc.h"

ShaderManager::ShaderManager(D3D& d3d)
    : m_d3dInstance(d3d), m_shaders()
{
}


ShaderManager::~ShaderManager()
{
}


bool ShaderManager::LoadShaders(const std::string& configFilename)
{
    // For now, don't load from config file. Just hard-code.

    //----------------------------------------------------------------------------------------------
    // Create mesh render shader, using 1 light and 1 texture.
    m_shaders["Mesh_1L_1T"] = Shader();
    m_shaders["Mesh_1L_1T"].SetVertexShader(m_d3dInstance, 0, L"mesh_vs.hlsl", "vp_main", "vs_5_0",
                                            &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Mesh_1L_1T"].SetPixelShader(m_d3dInstance, 0, L"mesh_ps.hlsl", "ps_main", "ps_5_0");

    m_shaders["Mesh_1L_1T"].AddBuffer(m_d3dInstance, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::MVPBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_1L_1T"].AddBuffer(m_d3dInstance, "CameraBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::CameraPosBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_1L_1T"].AddBuffer(m_d3dInstance, "LightColorBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::LightColorBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_1L_1T"].AddBuffer(m_d3dInstance, "LightPosBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::LightPosBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Mesh_1L_1T"].SetSampleState(m_d3dInstance, SamplerDesc::DEFAULT);
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Create shader for render pass to create shadows.
    m_shaders["Mesh_1L_1T_SHADOW"] = Shader();
    m_shaders["Mesh_1L_1T_SHADOW"].SetVertexShader(m_d3dInstance, 0, L"mesh_shadow_vs.hlsl", 
                                   "vp_main", "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Mesh_1L_1T_SHADOW"].SetPixelShader(m_d3dInstance, 0, L"mesh_shadow_ps.hlsl", 
                                   "ps_main", "ps_5_0");

    m_shaders["Mesh_1L_1T_SHADOW"].AddBuffer(m_d3dInstance, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                   sizeof(ConstantBuffers::MVPBuffer), D3D11_BIND_CONSTANT_BUFFER, 
                                   D3D11_CPU_ACCESS_WRITE, 0, 0);
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Create shader for bitmap rendering.
    m_shaders["Bitmap"] = Shader();
    m_shaders["Bitmap"].SetVertexShader(m_d3dInstance, 0, L"bitmap_vs.hlsl", "vp_main", "vs_5_0",
                                        &PolyLayouts::POS3_TEX2[0], 2);
    m_shaders["Bitmap"].SetPixelShader (m_d3dInstance, 0, L"bitmap_ps.hlsl", "ps_main", "ps_5_0");

    m_shaders["Bitmap"].AddBuffer(m_d3dInstance, "MatrixBuffer", D3D11_USAGE_DYNAMIC, 
                                  sizeof(ConstantBuffers::MVPBuffer), D3D11_BIND_CONSTANT_BUFFER, 
                                  D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Bitmap"].SetSampleState(m_d3dInstance, SamplerDesc::DEFAULT);
    //----------------------------------------------------------------------------------------------

    // NOTE: should be checking all of the above works fine.
    return true;
}


Shader* ShaderManager::GetShader(const std::string& id) 
{
    if(m_shaders.find(id) != m_shaders.end())
    {
        // Shader exists.
        return &(m_shaders.find(id)->second);
    }
    
    return 0;
}
