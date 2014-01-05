#include "VisualMeshComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../Camera/CameraComponent.h"
#include "../Light/LightComponent.h"

#include "../../Assets/Shaders/ShaderManager.h"
#include "../../Assets/Textures/TextureManager.h"

#include "../../glm/gtc/matrix_transform.hpp"

extern ShaderManager G_ShaderManager;
extern TextureManager G_TextureManager;

VisualMeshComponent::VisualMeshComponent(D3D& d3d, const std::string& filename, Texture& texture,
                                         std::vector<RenderTarget*>& shadowMaps)
    : VisualComponent(),
      m_mesh(filename, d3d, false), 
	  m_texture(texture),
      m_bumpTexture(texture), // Not used anyway, so set as same as m_texture for now?
      m_shadowMaps(shadowMaps),
      m_castShadows(false),
      m_recieveShadows(false)
{
    if(!G_ShaderManager.IsLoaded())
    {
        G_ShaderManager.LoadShaders(d3d, "configFile");
    }
    m_Shader = (G_ShaderManager.GetShader("Normal_Shadows_Test"));
}


VisualMeshComponent::VisualMeshComponent(D3D& d3d, const std::string& filename, Texture& texture,
                                         Texture& bmpMap, std::vector<RenderTarget*>& shadowMaps)
    : VisualComponent(),
      m_mesh(filename, d3d, true),
      m_texture(texture),
      m_bumpTexture(bmpMap),
      m_shadowMaps(shadowMaps),
      m_castShadows(false),
      m_recieveShadows(false)
{
    if(!G_ShaderManager.IsLoaded())
    {
        G_ShaderManager.LoadShaders(d3d, "configFile");
    }
    m_Shader = (G_ShaderManager.GetShader("Normal_Shadows_Test"));
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
        // Send data for the matrix buffer to the shader, getting view and proj matrix from light.

        const std::vector<Component*>& lights = GetParent().GetParent().GetLights();
        const auto& renderTargets = GetParent().GetParent().GetShadowMaps();
        // Get the first (1) lights and render their depth buffers.
        for(int i = 0; i < glm::min(2, (int)renderTargets.size()); i++)
        {
            renderTargets[i]->SetRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView());
            d3d.ClearDepthStencilView();

            ConstantBuffers::MVPBuffer matBuffer;
            matBuffer.modelMatrix = glm::transpose( GetParent().GetTransform().GetMatrix());
            // Get light and send it's data.
            LightComponent* light = static_cast<LightComponent*>(lights[i]);
            matBuffer.viewMatrix = glm::transpose( light->GetViewMatrix());
            matBuffer.projectionMatrix = glm::transpose(light->GetProjMatrix());

            shadowShader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&matBuffer,
                                               sizeof(matBuffer), 0);
            shadowShader->RenderShader(d3d, m_mesh.GetIndexCount());
        }
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
    ConstantBuffers::ShadowMatrixBuffer matBuffer;

    matBuffer.modelMatrix       = glm::transpose(
                                    GetParent().GetTransform().GetMatrix());
    matBuffer.viewMatrix        = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetViewMatrix());
    matBuffer.projectionMatrix  = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetProjMatrix());

    ConstantBuffers::LightPositionBuffer lightPosBuffer;

    // Loop through the lights (up to the max stated) and add their data to the buffer(s).
    for (int i = 0; i < ConstantBuffers::MAX_SHADOWCASTING_LIGHTS; i++)
    {
        if(i < lights.size())
        {
            // Only do this if there are enough lights to do so.
            LightComponent* light = static_cast<LightComponent*>(lights[i]);
            
            matBuffer.lightViewMatrix[i]       = glm::transpose(light->GetViewMatrix());
            matBuffer.lightProjectionMatrix[i] = glm::transpose(light->GetProjMatrix());

            lightPosBuffer.lightPosition[i]    = glm::vec4(light->GetParent().GetPos(), 0.0f);
        }
    }

    // Set the buffers using the data put into the structures above.
    m_Shader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
                                  (void*)&matBuffer, sizeof(matBuffer), 0);

    m_Shader->VSSetConstBufferData(d3d, std::string("LightPositionBuffer"), 
            (void*)&lightPosBuffer, sizeof(lightPosBuffer), 1);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Set light buffer data.
    if(lights.size() > 0)
    {
        LightComponent* light = static_cast<LightComponent*>(lights[0]);
        LightComponent* light2 = static_cast<LightComponent*>(lights[1]);

        // Create the lightBuffer to be sent to the GPU.
        ConstantBuffers::Light lightsBuff[2] =
        {
            {
                true,
                glm::vec4(light->GetParent().GetPos(), 1.0f),
                light->GetAmbient(),
                light->GetDiffuse(),
                light->GetSpecular(),
                180.0f,
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.0f,
                glm::vec3(0.0f, 0.0f, 0.0f),

            },
            {
                true,
                glm::vec4(light2->GetParent().GetPos(), 1.0f),
                light2->GetAmbient(),
                light2->GetDiffuse(),
                light2->GetSpecular(),
                180.0f,
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.0f,
                glm::vec3(0.0f, 0.0f, 0.0f)
            }
        };
            
        m_Shader->SetStructuredBufferData(d3d, std::string("LightBuffer"), (void*)&lightsBuff, 
                                          sizeof(lightsBuff));
        
        ID3D11ShaderResourceView* lightBuffer = m_Shader->GetBufferSRV(std::string("LightBuffer"));
        d3d.GetDeviceContext().PSSetShaderResources(4, 1, &lightBuffer);

        // Set camera buffer data. (pixel shader).
        glm::vec4 cameraPos = 
            glm::vec4(GetParent().GetParent().GetActiveCamera()->GetParent().GetPos(), 1.0f);

        m_Shader->PSSetConstBufferData(d3d, std::string("CameraBuffer"),
            (void*)&cameraPos, sizeof(glm::vec4), 0);
            
    }
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get textures for this model and set for shader.
	ID3D11ShaderResourceView* tex = m_texture.GetTexture();
	d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);

    ID3D11ShaderResourceView* shadowTex = m_shadowMaps[0]->GetShaderResourceView();
    d3d.GetDeviceContext().PSSetShaderResources(1, 1, &shadowTex);

    ID3D11ShaderResourceView* shadowTex2 = m_shadowMaps[1]->GetShaderResourceView();
    d3d.GetDeviceContext().PSSetShaderResources(2, 1, &shadowTex2);

    // If the static mesh contains tangents and binormals. Send the normal map to the shader
    // (just to test atm, need to add something to turn this on/off and to select which normal
    // map is wanted to be used.
    if(m_mesh.DoesContainTanBin())
    {
        ID3D11ShaderResourceView* bumpTex = m_bumpTexture.GetTexture();
        d3d.GetDeviceContext().PSSetShaderResources(3, 1, &bumpTex);
    }

    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

    // Get appropriate shader and then render with it.
    if(m_mesh.DoesContainTanBin())
    {
        m_Shader = G_ShaderManager.GetShader("Normal_Shadows_Test");
    }
    else
    {
        m_Shader = G_ShaderManager.GetShader("Mesh_1L_1T_ShadowMap");
    }

    // Render shader.
    m_Shader->RenderShader(d3d, m_mesh.GetIndexCount());
}
