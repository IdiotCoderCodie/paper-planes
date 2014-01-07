#include "ParticleSystemComponent.h"
#include "../Component.h"
#include "../../Entities/Entity.h"
#include "../../Scenes/Scene.h"
#include "../../Scenes/SceneManager.h"
#include "../../Assets/Textures/TextureManager.h"
#include "../../Components/Camera/CameraComponent.h"
#include "../../d3d_safe_release.h"

#include <algorithm>
#include <time.h>

extern TextureManager G_TextureManager; 

ParticleSystemComponent::ParticleSystemComponent(D3D& d3d)
    : m_particlePool(),
      m_engagedParticles(),
     // m_eligibleParticles(),
      m_maxParticleCount(100),
      m_currentParticleCount(0),
      m_emissionFreq(1.0f),
      m_timeBetweenEmissions(1.0f / m_emissionFreq),
      m_timeSinceLastEmission(0.0f),
      m_systemLifetime(-1.0f),
      m_startPosition(0.0f, 0.0f, 0.0f),
      m_startPositionDeviation(0.0f),
      m_startVelocity(0.0f, 1.0f, 0.0f),
      m_startVelocityDeviation(1.0f),
      m_startColor(0.5f, 0.0f, 0.0f, 1.0f),
      m_startColorDeviation(0.5f, 0.0f, 0.0f, 1.0f),
      m_startSize(1.0f),
      m_startSizeDeviation(0.0f),
      m_texture(0)
{
    m_texture = G_TextureManager.LoadTexture(d3d, L"errorTexture.dds", "errorTexture.dds");
    //m_texture = G_TextureManager.GetTexture("particle.dds");

    // Fill the particle pool with the maximum number of particles, also add them all to elibible.
    for (int i = 0; i < m_maxParticleCount; i++)
    {
        m_particlePool.push_back(Particle());
        m_particlePool[i].active = false;
        
    }
    srand(time(NULL));
    InitBuffers(d3d);

    m_Shader = G_ShaderManager.GetShader("Particle");
}


ParticleSystemComponent::~ParticleSystemComponent(void)
{
    d3d_safe_release(m_vertexBuffer);
    d3d_safe_release(m_indexBuffer);
}


bool ParticleSystemComponent::InitBuffers(D3D& d3d)
{
    m_vertexCount = m_maxParticleCount * 6;
    m_indexCount = m_vertexCount;

    // Create vertices array.
    m_vertices = new VertexType[m_vertexCount];
    if(!m_vertices)
    {
        return false;
    }

    // Create indices array.
    unsigned long* indices = new unsigned long[m_indexCount];
    if(!indices)
    {
        return false;
    }

    // Init vertex data to 0's.
    memset(m_vertices, 0, sizeof(VertexType) * m_vertexCount);

    // Init index array.
    for (int i = 0; i < m_indexCount; i++)
    {
        indices[i] = i;
    }

    //----------------------------------------------------------------------------------------------
    // Setup vertex buffer and then create.
    D3D11_BUFFER_DESC vertexBufferDesc = 
    {
        sizeof(VertexType) * m_vertexCount,     // ByteWidth
        D3D11_USAGE_DYNAMIC,                    // Usage
        D3D11_BIND_VERTEX_BUFFER,               // Bind Flags
        D3D11_CPU_ACCESS_WRITE,                 // CPUAccessFlags
        0,                                      // MiscFlags
        0                                       // StructureByteStride
    };
    D3D11_SUBRESOURCE_DATA vertexData = 
    {
        m_vertices,     // pSysMem
        0,              // SysMemPitch
        0               // SysMemSlicePitch
    };
    HRESULT hr = d3d.GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if(FAILED(hr))
    {
        return false;
        delete[] indices;
        indices = 0;
    }
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Setup index buffer and then create.
    D3D11_BUFFER_DESC indexBufferDesc = 
    {
        sizeof(unsigned long) * m_indexCount,   // ByteWidth
        D3D11_USAGE_DEFAULT,                    // Usage
        D3D11_BIND_INDEX_BUFFER,                // Bind Flags
        0,                                      // CPUAccessFlags
        0,                                      // MiscFlags
        0                                       // StructureByteStride
    };
    D3D11_SUBRESOURCE_DATA indexData =
    {
        indices,    // pSysMem
        0,          // SysMemPitch
        0           // SysMemSlicePitch
    };
    hr = d3d.GetDevice().CreateBuffer(&indexBufferDesc, & indexData, &m_indexBuffer);
    if(FAILED(hr))
    {
        return false;
        delete[] indices;
        indices = 0;
    }
    //----------------------------------------------------------------------------------------------

    delete[] indices;
    indices = 0;

    return true;
}


void ParticleSystemComponent::Update(float time)
{
    RemoveParticles(time);
    EmitParticles(time);
    UpdateBuffers(GetParent().GetParent().GetParent().GetD3DInstance());

    for(Particle& p : m_engagedParticles)
    {
        /*glm::vec3 test1 = p.velocity * time;
        glm::vec3 test = p.position += p.velocity * time;*/
        p.position = p.position +  (p.velocity * time);
    }
}


void ParticleSystemComponent::Draw(D3D& d3d)
{

    //----------------------------------------------------------------------------------------------
    // Get matrices and put in buffer format.
    ConstantBuffers::MVPBuffer matBuffer =
    {
        glm::transpose(GetParent().GetTransform().GetMatrix()),
        glm::transpose(GetParent().GetParent().GetActiveCamera()->GetViewMatrix()),
        glm::transpose(GetParent().GetParent().GetActiveCamera()->GetProjMatrix())
    };

    m_Shader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), (void*)&matBuffer, 
                                   sizeof(matBuffer), 0);

    // Set texture.
    ID3D11ShaderResourceView* tex = m_texture->GetTexture();
    d3d.GetDeviceContext().PSSetShaderResources(0, 1, &tex);

    // Set vertex buffer and index buffer to active.
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;
    d3d.GetDeviceContext().IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    d3d.GetDeviceContext().IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    d3d.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_Shader->RenderShader(d3d, m_indexCount);
}


void ParticleSystemComponent::RemoveParticles(float time)
{
    // Currently, for testing, remove all particles which go so far away from the start. (3.0).
    for(auto it = m_engagedParticles.begin(); it != m_engagedParticles.end(); )
    {
        if(glm::distance(it->position, GetParent().GetPos()) > 25.0f)
        {
            // If the distance is greater than (5.0f), erase this particle.
            it = m_engagedParticles.erase(it);
            m_currentParticleCount--;
        }
        else
            ++it;
    }
}


void ParticleSystemComponent::EmitParticles(float time)
{
    m_timeSinceLastEmission += time;

    unsigned int particlesToEmit = (unsigned int)(m_timeSinceLastEmission / m_timeBetweenEmissions);
    if(!particlesToEmit)
    {
        // Not ready to emit any particles yet.
        return;
    }

    m_timeSinceLastEmission -= m_timeBetweenEmissions;

    for (int i = 0; (i < particlesToEmit) && (m_currentParticleCount < m_maxParticleCount); i++)
    {
        if(m_particlePool.size() > 0)
        {
            Particle newParticle;
            InitNewEmission(newParticle);
            
            // Find where the particle should go (ordered back->front).
            auto it = std::find_if(m_engagedParticles.begin(), m_engagedParticles.end(), 
                                    [&](Particle const& particle) {
                                        return newParticle.position.z < particle.position.z;
                                    });

            // Insert the new particle in designated position.
            m_engagedParticles.insert(it, newParticle);
            m_currentParticleCount++;
        }
    }
}


void ParticleSystemComponent::InitNewEmission(Particle& newEmission)
{
    newEmission.active = true;

    newEmission.position = m_startPosition;
    newEmission.position.x += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startPositionDeviation.x;
    newEmission.position.y += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startPositionDeviation.y;
    newEmission.position.z += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startPositionDeviation.z;

    newEmission.velocity = m_startVelocity;
    newEmission.velocity.x += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startVelocityDeviation.x;
    newEmission.velocity.y += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startVelocityDeviation.y;
    newEmission.velocity.z += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startVelocityDeviation.z;

    newEmission.color = m_startColor;
    newEmission.color.x += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startColorDeviation.x;
    newEmission.color.y += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startColorDeviation.y;
    newEmission.color.z += (((float)(rand() % 1000) / 500.0f) - 1.0f) * m_startColorDeviation.z;
   /* newEmission.color.x += (((float)rand()-(float)rand()) / RAND_MAX) * m_startColorDeviation.x;
    newEmission.color.y += (((float)rand()-(float)rand()) / RAND_MAX) * m_startColorDeviation.y;
    newEmission.color.z += (((float)rand()-(float)rand()) / RAND_MAX) * m_startColorDeviation.z;*/
}


bool ParticleSystemComponent::UpdateBuffers(D3D& d3d)
{
    memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

    int index = 0;

    // Re-fill vertex buffer with updated info.
    for(Particle p : m_engagedParticles)
    {
        // Bottom left.
        m_vertices[index].position = p.position + glm::vec3(-m_startSize, -m_startSize, 0.0f);
        m_vertices[index].uv       = glm::vec2(0.0f, 1.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Top left.
        m_vertices[index].position = p.position + glm::vec3(-m_startSize, +m_startSize, 0.0f);
        m_vertices[index].uv       = glm::vec2(0.0f, 0.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Bottom right.
        m_vertices[index].position = p.position + glm::vec3(+m_startSize, -m_startSize, 0.0f);
        m_vertices[index].uv       = glm::vec2(1.0f, 1.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Bottom right.
        m_vertices[index].position = p.position + glm::vec3(+m_startSize, -m_startSize, 0.0f);
        m_vertices[index].uv       = glm::vec2(1.0f, 1.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Top left.
        m_vertices[index].position = p.position + glm::vec3(-m_startSize, +m_startSize, 0.0f);
        m_vertices[index].uv       = glm::vec2(0.0f, 0.0f);
        m_vertices[index].color    = p.color;
        index++;
        // Top right.
        m_vertices[index].position = p.position + glm::vec3(+m_startSize, +m_startSize, 0.0f);
        m_vertices[index].uv       = glm::vec2(1.0f, 0.0f);
        m_vertices[index].color    = p.color;
        index++;
    }

    // Lock the vertex buffer to send data to it.
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = d3d.GetDeviceContext().Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, 
                                            &mappedResource);
    if(FAILED(hr))
    {
        return false;
    }

    VertexType* verticesPtr = (VertexType*)mappedResource.pData;

    memcpy(verticesPtr, (void*)m_vertices, (sizeof(VertexType) * m_vertexCount));

    // Unlock the vertex buffer.
    d3d.GetDeviceContext().Unmap(m_vertexBuffer, 0);

    return true;
}

