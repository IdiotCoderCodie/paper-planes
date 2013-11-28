#include "StaticMesh.h"
#include "../d3d_safe_release.h"
#include <string>
#include <cassert>


StaticMesh::StaticMesh(const std::string& filename, D3D& d3d)
    :   m_filename(filename),
        m_modelData(),
        m_vertexBuffer(0),
        m_indexBuffer(0)
{
    LoadObj(filename);
    InitBuffers(d3d);
}


StaticMesh::~StaticMesh(void)
{
    d3d_safe_release(m_indexBuffer);
    d3d_safe_release(m_vertexBuffer);
}


bool StaticMesh::LoadObj(const std::string& filename)
{
    std::vector<glm::vec3> tempVerts;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempUVs;

    std::ifstream input(filename);
    if(input.is_open())
    {
        bool readingFaces = false;
        std::string line;
        while(input.good())
        {
            std::getline(input, line);
            
            // these two temp vectors prevent the creation. TEST WHEN COMPLETE.
            /*glm::vec3 tempVec3;
            glm::vec2 tempVec2;*/

            if(line[0] == 'v' && line[1] != 't' && line[1] != 'p' && line[1] != 'n')
            {
                // Read in all vertices.
                glm::vec3 newVert;
                char c;
                input >> c >> newVert.x >> newVert.y >> newVert.z; 
                //sscanf(line.c_str(), "%*c %f %f %f", &x, &y, &z);
                tempVerts.push_back(newVert);
            }
            else if(line[0] == 'v' && line[1] == 'n')
            {
                // Read in all normals.
                glm::vec3 newNorm;
                char c;
                input >> c >> c >> newNorm.x >> newNorm.y >> newNorm.z;
                tempNormals.push_back(newNorm);
            }
            else if(line[0] == 'v' && line[1] == 't')
            {
                // Read in all texture coords.
                glm::vec2 newTex;
                char c;
                input >> c >> c >> newTex.x >> newTex.y;
                tempUVs.push_back(newTex);
            }
            else if(line[0] == 'f')
            {
                static int faceCount = 0;

                char c;
                input >> c; // Remove the 'f'.

                for(int i = 0; i != 3; i++)
                {
                    int v, n, t;
                    input >> v >> c >> t >> c >> n;
                    
                    ModelVertex newModVert;
                    // Assign vertex positions.
                    newModVert.x    = tempVerts[v].x; 
                    newModVert.y    = tempVerts[v].y;
                    newModVert.z    = tempVerts[v].z;
                    // Assign texture coords.
                    newModVert.tu   = tempVerts[t].x;
                    newModVert.tv   = tempVerts[t].y;
                    //Assign normals.
                    newModVert.nx   = tempVerts[n].x;
                    newModVert.ny   = tempVerts[n].y;
                    newModVert.nz   = tempVerts[n].z;

                    m_modelData.push_back(newModVert);

                    // Read in next 2 lines, don't read in last loop, don't need this line yet.
                    if(i != 2)
                        std::getline(input, line);

                }
                faceCount++;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}


bool StaticMesh::InitBuffers(D3D& d3d)
{
    unsigned long vertexCount = m_modelData.size();
    VertexFormat* vertices = new VertexFormat[vertexCount];
    unsigned long* indices = new unsigned long[vertexCount];

    // Convert data from Model type to vertex type.
    // NOTE: Could probably change this, and just get rid of all this, just load straight into 
    //       container and then create buffers from there, instead of loading and then converting.
    int i = 0;
    for (auto it = m_modelData.begin(); it != m_modelData.end(); it++, i++)
    {
        vertices[i].position    = glm::vec3(it->x, it->y, it->z);
        vertices[i].texture     = glm::vec2(it->tu, it->tv);
        vertices[i].normal      = glm::vec3(it->nx, it->ny, it->nz);

        indices[i] = i;
    }

    // Setup description of static vertex buffer.
    D3D11_BUFFER_DESC vertexBufferDesc; 
    vertexBufferDesc.Usage                  = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth              = sizeof(VertexFormat) * vertexCount;
    vertexBufferDesc.BindFlags              = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags         = 0;
	vertexBufferDesc.MiscFlags              = 0;
	vertexBufferDesc.StructureByteStride    = 0;
    // Setup subresource data with pointer to the vertex data.
    D3D11_SUBRESOURCE_DATA  vertexData;
    vertexData.pSysMem                      = vertices;
    vertexData.SysMemPitch                  = 0;
    vertexData.SysMemSlicePitch             = 0;

    // Create vertex buffer.
    HRESULT result = d3d.GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if(FAILED(result))
    {
        assert(true);
        delete[] vertices;
        vertices = 0;
        delete[] indices;
        indices = 0;
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    // vertexCount == indexCount.
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * vertexCount; 
	indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags      = 0;
	indexBufferDesc.MiscFlags           = 0;
	indexBufferDesc.StructureByteStride = 0;
    // Setup subresource data with pointer to the index data.
    D3D11_SUBRESOURCE_DATA  indexData;
    indexData.pSysMem                   = indices;
    indexData.SysMemPitch               = 0;
    indexData.SysMemSlicePitch          = 0;

    // Create index buffer.
    result = d3d.GetDevice().CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if(FAILED(result))
    {
        assert(true);
        delete[] vertices;
        vertices = 0;
        delete[] indices;
        vertices = 0;
        return false;
    }

    delete[] vertices;
    vertices = 0;
    delete[] indices;
    vertices = 0;

    return true;
}


void StaticMesh::Render(D3D& d3d)
{
    unsigned int stride = sizeof(VertexFormat);
    unsigned int offset = 0;

    // Set vertex buffer to active in the input assembler.
    d3d.GetDeviceContext().IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set index buffer to active.
    d3d.GetDeviceContext().IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set type of primitive to be rendered.
    d3d.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
