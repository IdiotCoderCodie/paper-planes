#include "VisualMeshComponent.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../Camera/CameraComponent.h"
#include "../Light/LightComponent.h"

#include "../../Assets/Shaders/ShaderManager.h"

#include "../../glm/gtc/matrix_transform.hpp"

extern ShaderManager G_ShaderManager;

VisualMeshComponent::VisualMeshComponent(D3D& d3d, const std::string& filename, Texture& texture)
    :   VisualComponent(),
        m_mesh(filename, d3d), 
		m_texture(texture)
{
    if(!G_ShaderManager.IsLoaded())
    {
        G_ShaderManager.LoadShaders(d3d, "configFile");
    }
    m_Shader = (G_ShaderManager.GetShader("Mesh_1L_1T"));
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
    ConstantBuffers::MVPBuffer matBuffer;
    matBuffer.modelMatrix       = glm::transpose(
                                    GetParent().GetTransform().GetMatrix());
    matBuffer.viewMatrix        = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetViewMatrix());
    matBuffer.projectionMatrix  = glm::transpose(
                                    GetParent().GetParent().GetActiveCamera()->GetProjMatrix());
    // Set the buffer data using above matrices.
    m_Shader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), 
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
        ConstantBuffers::LightColorBuffer firstLight = 
        { 
            light->GetAmbient(),
            light->GetDiffuse(),
            light->GetSpecular(),
            40.0f,
            glm::vec3(0.0f, 0.0f, 0.0f)
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
    // Get active camera and put data in CameraBufferFormat, then send to shader.
    const ConstantBuffers::CameraPosBuffer cam = 
    { 
        GetParent().GetParent().GetActiveCamera()->GetParent().GetPos(), 
        0.0f 
    };

    m_Shader->VSSetConstBufferData(d3d, std::string("CameraBuffer"), (void*)&cam, sizeof(cam), 2);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Get texture for this model and set for shader.
	ID3D11ShaderResourceView* tex = m_texture.GetTexture();
	d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------


    // Render sahder.
    m_Shader->RenderShader(d3d, m_mesh.GetIndexCount());
}
