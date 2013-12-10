#pragma once
#include "../../glm/glm.hpp"
#include "../../D3D.h"
#include "Texture.h"

namespace bitmap
{
    struct VertexStruct
    {
        glm::vec3 position;
        glm::vec2 uv;
    };

    struct MatrixBufferStruct
    {
        glm::mat4 projectionMat;
    };
};

class Bitmap
{
public:
    Bitmap(D3D& d3d, Texture& texture, int height, int width, int screenWidth, int screenHeight);
    ~Bitmap(void);

    Texture& GetTexture() const { return m_texture; }

    bool UpdateBuffers(D3D& d3d, int positionX, int positionY);

    void Draw(D3D& d3d);

private:
    Texture& m_texture;
    ID3D11Buffer* m_vertexBuffer;
    ID3D11Buffer* m_indexBuffer;
    int m_vertexCount;
    int m_indexCount;

    int m_screenWidth;
    int m_screenHeight;
    int m_bitmapWidth;
    int m_bitmapHeight;
};

