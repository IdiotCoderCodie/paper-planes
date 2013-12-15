#include "ShaderManager.h"

ShaderManager G_ShaderManager;

ShaderManager::ShaderManager()
    :  m_shaders()
{
}


ShaderManager::~ShaderManager()
{
}


bool ShaderManager::LoadShaders(D3D& d3d, const std::string& configFilename)
{
    // For now, don't load from config file. Just hard-code.

    //----------------------------------------------------------------------------------------------
    // Create mesh render shader, using 1 light and 1 texture.
    m_shaders["Mesh_1L_1T"] = Shader();
    m_shaders["Mesh_1L_1T"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\color_vs.hlsl", "vp_main", 
                                            "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Mesh_1L_1T"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\color_ps.hlsl", "ps_main", 
                                           "ps_5_0");

    m_shaders["Mesh_1L_1T"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::MVPBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_1L_1T"].AddBuffer(d3d, "CameraBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::CameraPosBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_1L_1T"].AddBuffer(d3d, "LightColorBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::LightColorBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_1L_1T"].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::LightPosBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Mesh_1L_1T"].SetSampleState(d3d, SamplerDesc::DEFAULT);
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Create shader for render pass to create shadows.
    /*m_shaders["Mesh_1L_1T_SHADOW"] = Shader();
    m_shaders["Mesh_1L_1T_SHADOW"].SetVertexShader(d3d, 0, L"mesh_shadow_vs.hlsl", 
                                   "vp_main", "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Mesh_1L_1T_SHADOW"].SetPixelShader(d3d, 0, L"mesh_shadow_ps.hlsl", 
                                   "ps_main", "ps_5_0");

    m_shaders["Mesh_1L_1T_SHADOW"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                   sizeof(ConstantBuffers::MVPBuffer), D3D11_BIND_CONSTANT_BUFFER, 
                                   D3D11_CPU_ACCESS_WRITE, 0, 0);*/
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Create shader for bitmap rendering.
    m_shaders["Bitmap"] = Shader();
    m_shaders["Bitmap"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\bitmap_vs.hlsl", "vp_main", 
                                        "vs_5_0", &PolyLayouts::POS3_TEX2[0], 2);
    m_shaders["Bitmap"].SetPixelShader (d3d, 0, L"Assets\\Shaders\\bitmap_ps.hlsl", "ps_main", 
                                        "ps_5_0");

    m_shaders["Bitmap"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC, 
                                  sizeof(ConstantBuffers::MVPBuffer), D3D11_BIND_CONSTANT_BUFFER, 
                                  D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Bitmap"].SetSampleState(d3d, SamplerDesc::DEFAULT);
    //----------------------------------------------------------------------------------------------

    m_loaded = true;
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
