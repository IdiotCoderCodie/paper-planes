#pragma once
#include "VisualComponent.h"
#include "../../Assets/Textures/Texture.h"

class ParticleSystemComponent : public VisualComponent
{
private:
    struct Particle
    {
        bool active;
        glm::vec3 position;
        glm::vec4 color;
        glm::vec3 velocity;
        float size;
    };
    struct VertexType
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec4 color;
    };

public:
    ParticleSystemComponent(D3D& d3d);
    ~ParticleSystemComponent(void);

    virtual void ComponentID(componentId_t& out) const { out = "ParticleSystemComponent"; }

    virtual void Update(float time);

    virtual void Draw(D3D& d3d);

    void Start();

private:
    bool InitBuffers(D3D& d3d);

    /**
     * Called in the Update method, removes any particles that need to be removed.
     */
    void RemoveParticles(float time);
    /**
     * Called in the Update method. Emits particles if enough time has passed.
     */
    void EmitParticles(float time);
    /**
     * Called within EmitParticles. Initializes "newParticle" data.
     */
    void InitNewEmission(Particle& newParticle);

    bool UpdateBuffers(D3D& d3d);

    bool TweakBarSetup();

private:
    std::vector<Particle>  m_engagedParticles;
    unsigned int           m_maxParticleCount;
    unsigned int           m_currentParticleCount;
    float                  m_emissionFreq;
    float                  m_timeBetweenEmissions;
    float                  m_timeSinceLastEmission;
    float                  m_systemLifetime;
    glm::vec3              m_startPosition;
    glm::vec3              m_startPositionDeviation;
    glm::vec3              m_startVelocity;
    glm::vec3              m_startVelocityDeviation;
    glm::vec4              m_startColor;
    glm::vec4              m_startColorDeviation;
    glm::vec4              m_colorChangePerSec;
    float                  m_startSize;
    float                  m_startSizeDeviation;
    float                  m_sizeChangePerSec;
    Texture*               m_texture;

    int                    m_vertexCount;
    int                    m_indexCount;
    VertexType*            m_vertices;
    ID3D11Buffer*          m_vertexBuffer;
    ID3D11Buffer*          m_indexBuffer;

    bool                   m_tweakBarSetup;
};

