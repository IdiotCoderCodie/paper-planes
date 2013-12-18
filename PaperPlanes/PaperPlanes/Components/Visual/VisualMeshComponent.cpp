#include "VisualMeshComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../Camera/CameraComponent.h"
#include "../Light/LightComponent.h"

#include "../../Assets/Shaders/ShaderManager.h"

#include "../../glm/gtc/matrix_transform.hpp"

extern ShaderManager G_ShaderManager;

VisualMeshComponent::VisualMeshComponent(D3D& d3d, const std::string& filename, Texture& texture,
                                         std::vector<RenderTarget*>& shadowMaps)
    :   VisualComponent(),
        m_mesh(filename, d3d), 
		m_texture(texture),
        m_shadowMaps(shadowMaps),
        m_castShadows(false),
        m_recieveShadows(false)
{
    if(!G_ShaderManager.IsLoaded())
    {
        G_ShaderManager.LoadShaders(d3d, "configFile");
    }
    m_Shader = (G_ShaderManager.GetShader("Mesh_1L_1T_ShadowMap"));
}


VisualMeshComponent::~VisualMeshComponent(void)
{
}


void VisualMeshComponent::Update(float timeElapsed)
{
}


void VisualMeshComponent::ShadowPass(D3D& d3d)
{
    if(m_castShadows)
    {
        m_mesh.Render(d3d);

        Shader* shadowShader = G_ShaderManager.GetShader("Depth");
        // Send data for the matrix buffer to the shader, getting view and projection matrix from light.

        const std::vector<Component*>& lights = GetParent().GetParent().GetLights();
        const auto& renderTargets = GetParent().GetParent().GetShadowMaps();
        // Get the first (1) lights and render their depth buffers.
        for(int i = 0; i < glm::min(2, (int)renderTargets.size()); i++)
        {
            renderTargets[i]->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());
            d3d.ClearDepthStencilView();

            ConstantBuffers::MVPBuffer matBuffer;
            matBuffer.modelMatrix = glm::transpose( GetParent().GetTransform().GetMatrix());
            LightComponent* light = static_cast<LightComponent*>(lights[i]);
            matBuffer.viewMatrix = glm::transpose( light->GetViewMatrix());
            matBuffer.projectionMatrix = glm::transpose(light->GetProjMatrix());

            shadowShader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&matBuffer,
                                               sizeof(matBuffer), 0);
            shadowShader->RenderShader(d3d, m_mesh.GetIndexCount());
        }
        



//        ConstantBuffers::MVPBuffer matBuffer;
//        matBuffer.modelMatrix       = glm::transpose(
//                                        GetParent().GetTransform().GetMatrix());
//        if(lights.size() > 0)
//        {
//            LightComponent* light = static_cast<LightComponent*>(lights[0]);
//
//            matBuffer.viewMatrix   = glm::transpose(
//                                            light->GetViewMatrix());
//            matBuffer.projectionMatrix = glm::transpose(
//                                            light->GetProjMatrix());
//        }
//        else
//        {
//            assert(false);
//        }
//!
//        // Set the buffer data using above matrices.
//        shadowShader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
//                                      (void*)&matBuffer, sizeof(matBuffer), 0);
//
//        shadowShader->RenderShader(d3d, m_mesh.GetIndexCount());
    }
}


void VisualMeshComponent::Draw(D3D& d3d)
{
    m_mesh.Render(d3d);

    if(m_recieveShadows)
    {
        // Draw using the shadow map to cast any shadows on the mesh.
        DrawWithShadows(d3d);
    }
    else
    {
        // Draw using standard shader, no shadows.
        DrawNoShadows(d3d);
    }   
}


void VisualMeshComponent::DrawNoShadows(D3D& d3d)
{
    Shader* noShadowShader = G_ShaderManager.GetShader("Mesh_2L_1T");
    //----------------------------------------------------------------------------------------------
    // Get matrices and put in buffer format.
    ConstantBuffers::MVPBuffer matBuffer;
    matBuffer.modelMatrix       = glm::transpose(
                                    GetParent().GetTransform().GetMatrix());
    matBuffer.viewMatrix        = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetViewMatrix());
    matBuffer.projectionMatrix  = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetProjMatrix());
    // Set the buffer data using above matrices.
    noShadowShader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matBuffer, sizeof(matBuffer), 0);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get light from the scene.
    const std::vector<Component*>& lights = GetParent().GetParent().GetLights();
    // Get first light.
    if(lights.size() > 0)
    {
        LightComponent* light1 = static_cast<LightComponent*>(lights[0]);
        LightComponent* light2 = static_cast<LightComponent*>(lights[1]);
        ConstantBuffers::LightColorBuffer2 firstLight = 
        { 
            light1->GetAmbient(),
            light1->GetDiffuse(),
            light1->GetSpecular(),
            40.0f,
            glm::vec3(0.0f, 0.0f, 0.0f),

            light2->GetAmbient(),
            light2->GetDiffuse(),
            light2->GetSpecular(),
            40.0f,
            glm::vec3(0.0f, 0.0f, 0.0f)
        };

        noShadowShader->PSSetConstBufferData(d3d, std::string("LightColorBuffer"), 
                                             (void*)&firstLight, sizeof(firstLight), 0);

        // Get light positions and send to buffer.
        ConstantBuffers::LightPosBuffer2 posBuffer =
        {
            glm::vec4(light1->GetParent().GetPos(), 0.0f),
            glm::vec4(light2->GetParent().GetPos(), 0.0f)
        };

        noShadowShader->VSSetConstBufferData(d3d, std::string("LightPositionBuffer"), 
                                             (void*)&posBuffer, sizeof(posBuffer), 1);

    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get active camera and put data in CameraBufferFormat, then send to shader.
    const ConstantBuffers::CameraPosBuffer cam = 
    { 
        GetParent().GetParent().GetActiveCamera()->GetParent().GetPos(), 
        0.0f 
    };

    noShadowShader->VSSetConstBufferData(d3d, std::string("CameraBuffer"), (void*)&cam, sizeof(cam), 
                                         2);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get texture for this model and set for shader.
	ID3D11ShaderResourceView* tex = m_texture.GetTexture();
	d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    // Render sahder.
    noShadowShader->RenderShader(d3d, m_mesh.GetIndexCount());
}


void VisualMeshComponent::DrawWithShadows(D3D& d3d)
{
    const std::vector<Component*>& lights = GetParent().GetParent().GetLights();

    //----------------------------------------------------------------------------------------------
    // Get matrices and put in buffer format.
    ConstantBuffers::MVPShadowBuffer matBuffer;
    matBuffer.modelMatrix       = glm::transpose(
                                    GetParent().GetTransform().GetMatrix());
    matBuffer.viewMatrix        = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetViewMatrix());
    matBuffer.projectionMatrix  = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetProjMatrix());

    if(lights.size() > 0)
    {
        LightComponent* light = static_cast<LightComponent*>(lights[0]);

        matBuffer.lightViewMatrix   = glm::transpose(
                                        light->GetViewMatrix());
        matBuffer.lightProjectionMatrix = glm::transpose(
                                        light->GetProjMatrix());
    }
    else
    {
        assert(false);
    }

    // Set the buffer data using above matrices.
    m_Shader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matBuffer, sizeof(matBuffer), 0);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Set light buffer data.
    
    // Get first light.
    if(lights.size() > 0)
    {
        LightComponent* light = static_cast<LightComponent*>(lights[0]);
        ConstantBuffers::LightAmbientDiffuseColorBuffer firstLight = 
        { 
            light->GetAmbient(),
            light->GetDiffuse(),
        };

        m_Shader->PSSetConstBufferData(d3d, std::string("LightColorBuffer"), 
            (void*)&firstLight, sizeof(firstLight), 0);

        // Get pos and send to buffer.
        glm::vec4 lightPos(light->GetParent().GetPos(), 0.0f);

        m_Shader->VSSetConstBufferData(d3d, std::string("LightPositionBuffer"), 
            (void*)&lightPos, sizeof(glm::vec4), 1);
    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get texture for this model and set for shader.
	ID3D11ShaderResourceView* tex = m_texture.GetTexture();
	d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);

    ID3D11ShaderResourceView* shadowTex = m_shadowMaps[0]->GetShaderResourceView();
    d3d.GetDeviceContext().PSSetShaderResources(1, 1, &shadowTex);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    // Render shader.
    m_Shader->RenderShader(d3d, m_mesh.GetIndexCount());
}
