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

    m_shaders["Mesh_1L_1T"].AddSamplerState(d3d, "ModelTextureSampler", SamplerDesc::DEFAULT_WRAP);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create mesh render shader, using 1 light and 1 texture.
    m_shaders["Mesh_2L_1T"] = Shader();
    m_shaders["Mesh_2L_1T"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\mesh_2l_vs.hlsl", "vp_main", 
                                            "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Mesh_2L_1T"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\mesh_2l_ps.hlsl", "ps_main", 
                                           "ps_5_0");

    m_shaders["Mesh_2L_1T"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::MVPBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_2L_1T"].AddBuffer(d3d, "CameraBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::CameraPosBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_2L_1T"].AddBuffer(d3d, "LightColorBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::LightColorBuffer2), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_2L_1T"].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::LightPosBuffer2), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Mesh_2L_1T"].AddSamplerState(d3d, "ModelTextureSampler", SamplerDesc::DEFAULT_WRAP);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create mesh render shader using shadow, using 1 light and 1 texture.
    m_shaders["Mesh_1L_1T_ShadowMap"] = Shader();
    m_shaders["Mesh_1L_1T_ShadowMap"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\shadow2_vs.hlsl", 
                                        "vp_main", "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Mesh_1L_1T_ShadowMap"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\shadow2_ps.hlsl", 
                                        "ps_main", "ps_5_0");

    // Add all the buffers.
    m_shaders["Mesh_1L_1T_ShadowMap"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::MVPShadowBuffer2), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);    
    m_shaders["Mesh_1L_1T_ShadowMap"].AddBuffer(d3d, "LightColorBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::LightAmbientDiffuse2ColorBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Mesh_1L_1T_ShadowMap"].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::LightPosBuffer2), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    // Add the sampler states.
    m_shaders["Mesh_1L_1T_ShadowMap"].AddSamplerState(d3d, "ModelTextureSampler", 
                                                      SamplerDesc::DEFAULT_WRAP);
    m_shaders["Mesh_1L_1T_ShadowMap"].AddSamplerState(d3d, "ShadowMapSampler", 
                                                      SamplerDesc::SAMPLE_CLAMP);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create shader for render pass to create shadows.
    m_shaders["Depth"] = Shader();
    m_shaders["Depth"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\depth_vs.hlsl", 
                                   "vp_main", "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Depth"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\depth_ps.hlsl", 
                                   "ps_main", "ps_5_0");

    m_shaders["Depth"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                   sizeof(ConstantBuffers::MVPBuffer), D3D11_BIND_CONSTANT_BUFFER, 
                                   D3D11_CPU_ACCESS_WRITE, 0, 0);
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
    m_shaders["Bitmap"].AddSamplerState(d3d, "BitmapTextureSampler", SamplerDesc::DEFAULT_WRAP);
    //----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Load in the test normal map shader, with 2 lights and shadows.
	m_shaders["Normal_Shadows"] = Shader();
	m_shaders["Normal_Shadows"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\shadow2_bump_vs.hlsl", 
                                                "vp_main", "vs_5_0", 
                                                &PolyLayouts::POS3_TEX2_NORM3_TAN3_BIN3[0], 5);
	m_shaders["Normal_Shadows"].SetPixelShader (d3d, 0, L"Assets\\Shaders\\shadow2_bump_ps.hlsl", 
												"ps_main", "ps_5_0");
	m_shaders["Normal_Shadows"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC, 
										  sizeof(ConstantBuffers::MVPShadowBuffer2), 
                                          D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
	m_shaders["Normal_Shadows"].AddBuffer(d3d, "LightColorBuffer", D3D11_USAGE_DYNAMIC, 
										  sizeof(ConstantBuffers::LightAmbientDiffuse2ColorBuffer), 
										  D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
	m_shaders["Normal_Shadows"].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
										  sizeof(ConstantBuffers::LightPosBuffer2), 
    								      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	// Add the sampler states.  
	m_shaders["Normal_Shadows"].AddSamplerState(d3d, "ModelTextureSampler", 
                                                      SamplerDesc::DEFAULT_WRAP);
	m_shaders["Normal_Shadows"].AddSamplerState(d3d, "ShadowMapSampler", 
                                                      SamplerDesc::SAMPLE_CLAMP);
    /*m_shaders["Normal_Shadows"].AddSamplerState(d3d, "NormalTexture", 
                                                 SamplerDesc::DEFAULT_WRAP);*/
	//----------------------------------------------------------------------------------------------
    

    //----------------------------------------------------------------------------------------------
	// Load in the test normal map shader, with 2 lights and shadows.
	m_shaders["Normal_Shadows_Test"] = Shader();
	m_shaders["Normal_Shadows_Test"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\shadowStruct_bump_vs.hlsl", 
                                                "vp_main", "vs_5_0", 
                                                &PolyLayouts::POS3_TEX2_NORM3_TAN3_BIN3[0], 5);
	m_shaders["Normal_Shadows_Test"].SetPixelShader (d3d, 0, L"Assets\\Shaders\\shadowStruct_bump_ps.hlsl", 
												"ps_main", "ps_5_0");
	m_shaders["Normal_Shadows_Test"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC, 
										  sizeof(ConstantBuffers::MVPShadowBuffer2), 
                                          D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
	/*m_shaders["Normal_Shadows_Test"].AddBuffer(d3d, "LightColorBuffer", D3D11_USAGE_DYNAMIC, 
										  sizeof(ConstantBuffers::LightAmbientDiffuse2ColorBuffer), 
										  D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);*/
	m_shaders["Normal_Shadows_Test"].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
										  sizeof(ConstantBuffers::LightPosBuffer2), 
    								      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Normal_Shadows_Test"].AddBuffer(d3d, "CameraBuffer", D3D11_USAGE_DYNAMIC,
                                          sizeof(ConstantBuffers::CameraPosBuffer),
                                          D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Normal_Shadows_Test"].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	// Add the sampler states.  
	m_shaders["Normal_Shadows_Test"].AddSamplerState(d3d, "ModelTextureSampler", 
                                                      SamplerDesc::DEFAULT_WRAP);
	m_shaders["Normal_Shadows_Test"].AddSamplerState(d3d, "ShadowMapSampler", 
                                                      SamplerDesc::SAMPLE_CLAMP);
    
	//----------------------------------------------------------------------------------------------

    m_loaded = true;
    // NOTE: should be checking all of the above worked fine.
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
