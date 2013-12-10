#pragma once
#include "../../glm/glm.hpp"
#include <d3d11.h>
#include <vector>
#include <fstream>

#include "../../D3D.h"

class StaticMesh
{
private:
    struct VertexFormat
    {
        glm::vec3 position;
        glm::vec2 texture;
        glm::vec3 normal;
    };

    struct ModelVertex
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

public:
    //StaticMesh();
    StaticMesh(const std::string& filename, D3D& d3d);
    ~StaticMesh(void);

    bool LoadObj(const std::string& filename);
    bool InitBuffers(D3D& d3d);

    void Render(D3D& d3d);

    int GetVertexCount() const { return m_vertexCount; }
    int GetIndexCount() const { return m_indexCount;}

    void SetVertexCount(int vertexCount) { m_vertexCount = vertexCount; }
    void SetIndexCount(int indexCount) { m_indexCount = indexCount; }

private:
    std::string                 m_filename;
    int                         m_vertexCount;
    int                         m_indexCount;
    ID3D11Buffer*               m_vertexBuffer;
    ID3D11Buffer*               m_indexBuffer;

    std::vector<ModelVertex>    m_modelData;
};

