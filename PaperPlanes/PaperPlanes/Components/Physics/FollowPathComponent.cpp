#include "FollowPathComponent.h"
#include "../../Assets/Shaders/ShaderManager.h"
#include "../../Assets/Shaders/ShaderResources/constant_buffers.h"
#include "../../Scenes/Scene.h"
#include "../Camera/CameraComponent.h"

extern ShaderManager G_ShaderManager;

FollowPathComponent::FollowPathComponent(void)
    : m_nodes(),
      m_nextNode(3),
      m_speed(1.0f),
      m_t(0.0f),
      m_delayed(false),
      m_currentDelayTime(0.0f),
      m_reverse(false)
{
}


FollowPathComponent::~FollowPathComponent(void)
{
}


void FollowPathComponent::Update(float time)
{
    if(m_delayed)
    {
        // If delayed, increment current delay time. Then check if it's ready to go on to the next 
        // node. If it is, get the next node.
        m_currentDelayTime += time;
        if(m_currentDelayTime > m_nodes[m_nextNode].delay)
        {
            m_currentDelayTime = 0.0f;
            m_delayed = false;
            GetNextNode(); // Now get the next destination.
        }
        return;
    }

    if(m_nodes.size() > m_nextNode)
    {
        m_t += time;
        float t = m_t / m_nodes[m_nextNode].timeToReach;

        glm::vec3 nextPoint;
        if(m_reverse)
        {
            nextPoint = CalculateBezierPoint(t, 
            m_nodes[m_nextNode+3].position, // p0
            m_nodes[m_nextNode+2].position, // p1
            m_nodes[m_nextNode+1].position, // p2
            m_nodes[m_nextNode].position);  // p3
        }
        else
        {
            nextPoint = CalculateBezierPoint(t, 
            m_nodes[m_nextNode-3].position, // p0
            m_nodes[m_nextNode-2].position, // p1
            m_nodes[m_nextNode-1].position, // p2
            m_nodes[m_nextNode].position);  // p3
        }

        GetParent().SetPos(nextPoint);

        if(t >= 1.0f)
        {
            // Arrived at node. Now check if there is a delay on it before getting next destination.
            if(m_nodes[m_nextNode].delay > 0.0001f)
            {
                m_delayed = true;
                m_currentDelayTime = 0.0f;
            }
            else
            {
                GetNextNode();
            }
            m_t = 0.0f;           
        }
    }
}


int FollowPathComponent::GetNextNode()
{
    if(m_reverse)
    {
        m_nextNode -= 3;
        if(m_nextNode < 0) 
        {
            // Gone beyond the scope, must be time to switch direction again.
            m_nextNode = 3;
            m_reverse = false;
        }
    }
    else
    {
        m_nextNode += 3;
        if(m_nextNode >= m_nodes.size())
        {
            // Gone beyond the scope, must be time to reverse.
            m_nextNode -= 6;
            m_reverse = true;
        }
    }
    
    return m_nextNode;
}


void FollowPathComponent::AddNode(const glm::vec3& position, float arrivalTime, float delay)
{
    Node node = 
    {
        position,
        arrivalTime,
        delay
    };
    m_nodes.push_back(node);
}


void FollowPathComponent::AddNode(Entity const* nodeEnt)
{
    // Check the pointer given is not null before pushing onto the vector.
    if(!nodeEnt)
    {
        return;
    }
    AddNode(nodeEnt->GetPos(), 1.0f, 0.0f);
}


void FollowPathComponent::ClearNodes()
{
    m_nodes.clear();
}


glm::vec3 FollowPathComponent::CalculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, 
                               const glm::vec3& p2, const glm::vec3& p3) const
{
    // [x,y,z] = ((1-t)^3)*P0 + (3(1-t)^2)t*P1 + (3(1-t)t^2)*P2 + (t^3)*P3
    float d   = 1.0f - t;
    float dd  = d*d;
    float ddd = dd*d;
    float tt  = t*t;
    float ttt = tt*t;

    glm::vec3 point = ddd * p0;
    point += 3 * dd * t * p1;
    point += 3 * d * tt * p2;
    point += ttt * p3;

    return point;
}

void FollowPathComponent::Restart()
{
    m_delayed = false;
    m_nextNode = 3;
    m_reverse = false;
    m_t = 0.0f;
    m_currentDelayTime = 0.0f;
}

void FollowPathComponent::InitDebugBuffers(D3D& d3d)
{
    std::vector<glm::vec3> vertices;
   
    // Create the vertices for the path.
    for(int i = 0; i < m_nodes.size() - 3; i += 3)
    {
        if((i + 3) < m_nodes.size())
        {
            for(float t = 0.0f; t < 1.01f; t+= 0.1f)
            {
                vertices.push_back(CalculateBezierPoint(t, m_nodes[i].position, 
                                                        m_nodes[i+1].position, 
                                                        m_nodes[i+2].position, 
                                                        m_nodes[i+3].position));
            }
        }
    }

    m_indexNum = vertices.size();
    unsigned long* indices = new unsigned long[m_indexNum];
    for(int i = 0; i < m_indexNum; i++)
    {
        indices[i] = i;
    }

    D3D11_BUFFER_DESC vertexBufferDesc =
    {
        sizeof(glm::vec3) * m_indexNum, // ByteWidth
        D3D11_USAGE_DEFAULT,                // Usage
        D3D11_BIND_VERTEX_BUFFER,           // BindFlags
        0,                                  // CPUAccessFlags
        0,                                  // MiscFlags
        0                                   // StructureByteStride
    };

    D3D11_SUBRESOURCE_DATA vertexData =
    {
        &vertices[0],   // pSysMem
        0,              // SysMemPitch
        0,              // SysMemSlicePitch
    };

    HRESULT hr = d3d.GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if(FAILED(hr))
    {
        delete[] indices;
        indices = 0;
        return;
    }

    D3D11_BUFFER_DESC indexBufferDesc = 
    {
        sizeof(unsigned long) * m_indexNum, // ByteWidth
        D3D11_USAGE_DEFAULT,                    // Usage
        D3D11_BIND_INDEX_BUFFER,                // BindFlags
        0,                                      // CPUAccessFlags
        0,                                      // MiscFlags
        0       
    };

    D3D11_SUBRESOURCE_DATA indexData = 
    {
        indices,    // pSysMem
        0,          // SysMemPitch
        0           // SysMemSlicePitch
    };

    hr = d3d.GetDevice().CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if(FAILED(hr))
    {
        delete[] indices;
        indices = 0;
        return;
    }

    delete[] indices;
    indices = 0;
}

void FollowPathComponent::Draw(D3D& d3d)
{
    unsigned int stride = sizeof(glm::vec3);
    unsigned int offset = 0;

    // Set vertex buffer to active in the input assembler.
    d3d.GetDeviceContext().IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set index buffer.
    d3d.GetDeviceContext().IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set type of primitive to read vertex data as.
    d3d.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    
    Shader* shader = G_ShaderManager.GetShader("Lines");
    ConstantBuffers::MVPBuffer matrixBuffer = 
    {
        glm::transpose(glm::mat4(1.0f)),
        glm::transpose(GetParent().GetParent().GetActiveCamera()->GetViewMatrix()),
        glm::transpose(GetParent().GetParent().GetActiveCamera()->GetProjMatrix())
    };

    shader->VSSetConstBufferData(d3d, std::string("MatrixBuffer"), &matrixBuffer, 
                                 sizeof(matrixBuffer), 0);

    shader->RenderShader(d3d, m_indexNum);
}
