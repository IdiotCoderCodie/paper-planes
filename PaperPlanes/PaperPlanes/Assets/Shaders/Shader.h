#pragma once
#include <D3D11.h>

#include "../../glm/glm.hpp"
#include "../../D3D.h"


enum SHADER_TYPE
{
    VERTEX_SHADER,
    HULL_SHADER,
    DOMAIN_SHADER,
    GEOMETRY_SHADER,
    PIXEL_SHADER
};

class Shader
{
public:
    Shader(void);
    ~Shader(void);

    //bool InitShader(D3D& d3d, HWND hwnd, WCHAR* vsFilename, WCHAR* fsFilename);

    /**
     * Compiles and then sets the Shaders vertex shader to the shader in "filename".
     * polygonLayout is used to create the input layout of the vertices, and so should match the
     * input type in the shader file. 
     * Returns whether the action succeeded or not.
     */
    bool SetVertexShader(D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target,
                         D3D11_INPUT_ELEMENT_DESC polygonLayout[]);
    /**
     * Compiles shader at "filename" and sets it as this Shader objects hull shader.
     * Returns whether the action succeeded or not.
     */
    bool SetHullShader  (D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target);
    /**
     * Compiles shader at "filename" and sets it as this Shader objects domain shader.
     * Returns whether the action succeeded or not.
     */
    bool SetDomainShader(D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target);
    /**
     * Compiles shader at "filename" and sets it as this Shader objects geometry shader.
     * Returns whether the action succeeded or not.
     */
    bool SetGeomShader  (D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target);
    /**
     * Compiles shader at "filename" and sets it as this Shader objects pixel shader.
     * Returns whether the action succeeded or not.
     */
    bool SetPixelShader (D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target);


    /**
     * Set the shaders sample state using samplerDesc.
     */
    bool SetSampleState (D3D& d3d, D3D11_SAMPLER_DESC samplerDesc);

    void RenderShader   (D3D& d3d, int indexCount);

private:
    void OutputShaderErrorMessage(ID3D10Blob* errMsg, HWND hwnd, WCHAR* shaderFilename) const;

    bool CompileShader(HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target, 
                       ID3D10Blob* shaderBuff) const;


private:
    bool                    m_Initialized;

    ID3D11VertexShader*     m_vertexShader;
    ID3D11HullShader*       m_hullShader;
    ID3D11DomainShader*     m_domainShader;
    ID3D11GeometryShader*   m_geomShader;
    ID3D11PixelShader*      m_pixelShader;

    ID3D11InputLayout*      m_inputLayout;

    ID3D11SamplerState*     m_sampleState;
};

template<class T>
void d3d_safe_release(T* obj)
{
    if(obj)
    {
        obj->Release();
        obj = 0;
    }
}
