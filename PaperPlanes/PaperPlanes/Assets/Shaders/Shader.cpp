#include "Shader.h"

#include <d3dcompiler.h>

#include <fstream>


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

Shader::Shader(void) : 
    m_Initialized(false),
    m_vertexShader(0),
    m_hullShader(0),
    m_domainShader(0),
    m_geomShader(0),
    m_pixelShader(0),
    m_inputLayout(0),
    m_sampleState(0),
    m_buffers()
{
}


Shader::~Shader(void)
{
    d3d_safe_release(m_vertexShader);
    d3d_safe_release(m_hullShader);
    d3d_safe_release(m_domainShader);
    d3d_safe_release(m_geomShader);
    d3d_safe_release(m_pixelShader);
    d3d_safe_release(m_inputLayout);
}

//
//bool Shader::InitShader(D3D& d3d, HWND hwnd, WCHAR* vsFilename, WCHAR* fsFilename)
//{
//    HRESULT result;
//    ID3D10Blob* vertShaderBuff;
//    ID3D10Blob* pixelShaderBuff;
//    ID3D10Blob* errMessage;
//
//    
//
//    // Compile vertex shader.
//    result = D3DCompileFromFile(vsFilename, NULL, NULL, "vp_main", "vs_5_0", 0, 0, &vertShaderBuff, 
//                                &errMessage);
//    if(FAILED(result))
//    {
//        if(errMessage)
//        {
//            OutputShaderErrorMessage(errMessage, hwnd, vsFilename);
//        }
//        else
//        {
//            MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
//        }
//        return false;
//    }
//
//    // HORRIBLE REPEATED CODE. CHANGE THIS!!! O.O
//    // Compile fragment shader.
//    result = D3DCompileFromFile(fsFilename, NULL, NULL, "fp_main", "vs_5_0", 0, 0, &pixelShaderBuff, 
//                                &errMessage);
//    if(FAILED(result))
//    {
//        if(errMessage)
//        {
//            OutputShaderErrorMessage(errMessage, hwnd, vsFilename);
//        }
//        else
//        {
//            MessageBox(hwnd, fsFilename, L"Missing Shader File", MB_OK);
//        }
//        return false;
//    }
//
//    // Create vertex shader.
//    result = d3d.GetDevice().CreateVertexShader(vertShaderBuff->GetBufferPointer(), 
//                                                vertShaderBuff->GetBufferSize(), NULL, 
//                                                &m_vertexShader);
//    if(FAILED(result))
//        return false;
//    // Create fragment shader.
//    result = d3d.GetDevice().CreatePixelShader(pixelShaderBuff->GetBufferPointer(),
//                                               pixelShaderBuff->GetBufferSize(), NULL,
//                                               &m_pixelShader);
//    if(FAILED(result))
//        return false;
//}


bool Shader::SetVertexShader(D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target,
                             D3D11_INPUT_ELEMENT_DESC polygonLayout[])
{
    ID3D10Blob* shaderBuff = 0;
    // Compile Shader.
    if(!CompileShader(hwnd, filename, entryPoint, target, shaderBuff))
    {
        return false;
    }

    // Create shader.
    HRESULT result;
    result = d3d.GetDevice().CreateVertexShader(shaderBuff->GetBufferPointer(),
                shaderBuff->GetBufferSize(), NULL, &m_vertexShader);

    if(FAILED(result))
        return false;

    // Create input layout.
    int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    result = d3d.GetDevice().CreateInputLayout(polygonLayout, numElements, 
                                               shaderBuff->GetBufferPointer(), 
                                               shaderBuff->GetBufferSize(), &m_inputLayout);

    d3d_safe_release(shaderBuff);

    if(FAILED(result))
        return false;

    return true;
}


bool Shader::SetHullShader(D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target)
{
    ID3D10Blob* shaderBuff = 0;
    // Compile Shader.
    if(!CompileShader(hwnd, filename, entryPoint, target, shaderBuff))
    {
        return false;
    }
    // Create Shader.
    HRESULT result = d3d.GetDevice().CreateHullShader(shaderBuff->GetBufferPointer(),
                                                      shaderBuff->GetBufferSize(), NULL, 
                                                      &m_hullShader);

    d3d_safe_release(shaderBuff);

    if(FAILED(result))
        return false;

    return true;
}


bool Shader::SetDomainShader(D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target)
{
    ID3D10Blob* shaderBuff = 0;
    // Compile Shader.
    if(!CompileShader(hwnd, filename, entryPoint, target, shaderBuff))
    {
        return false;
    }
    // Create Shader.
    HRESULT result = d3d.GetDevice().CreateDomainShader(shaderBuff->GetBufferPointer(),
                                                        shaderBuff->GetBufferSize(), NULL, 
                                                        &m_domainShader);

    d3d_safe_release(shaderBuff);

    if(FAILED(result))
        return false;

    return true;
}


bool Shader::SetGeomShader(D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target)
{
    ID3D10Blob* shaderBuff = 0;
    // Compile Shader.
    if(!CompileShader(hwnd, filename, entryPoint, target, shaderBuff))
    {
        return false;
    }
    // Create Shader.
    HRESULT result = d3d.GetDevice().CreateGeometryShader(shaderBuff->GetBufferPointer(),
                                                          shaderBuff->GetBufferSize(), NULL, 
                                                          &m_geomShader);

    d3d_safe_release(shaderBuff);

    if(FAILED(result))
        return false;

    return true;
}


bool Shader::SetPixelShader(D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target)
{
    ID3D10Blob* shaderBuff = 0;
    // Compile Shader.
    if(!CompileShader(hwnd, filename, entryPoint, target, shaderBuff))
    {
        return false;
    }
    // Create Shader.
    HRESULT result = d3d.GetDevice().CreatePixelShader(shaderBuff->GetBufferPointer(),
                                                          shaderBuff->GetBufferSize(), NULL, 
                                                          &m_pixelShader);

    d3d_safe_release(shaderBuff);

    if(FAILED(result))
        return false;

    return true;
}


bool Shader::SetSampleState(D3D& d3d, D3D11_SAMPLER_DESC& samplerDesc)
{
    HRESULT result = d3d.GetDevice().CreateSamplerState(&samplerDesc, &m_sampleState);
    if(FAILED(result))
        return false;

    return true;
}


bool Shader::AddBuffer(D3D& d3d, const std::string& identity, D3D11_BUFFER_DESC& bufferDesc)
{
    // Attempt to create the buffer.
    ID3D11Buffer* newBuffer;
    HRESULT result = d3d.GetDevice().CreateBuffer(&bufferDesc, NULL, &newBuffer);

    if(FAILED(result))
        return false;

    // If successful, add buffer to buffer map.
    m_buffers[identity] = newBuffer;

    newBuffer = 0;
    return true;
}


bool Shader::AddBuffer(D3D& d3d, const std::string& identity, D3D11_USAGE usage, UINT byteWidth, 
                       UINT bindFlags, UINT cpuAccessFlags, UINT miscFlags, UINT byteStride)
{
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage                = usage;
    bufferDesc.ByteWidth            = byteWidth;
    bufferDesc.BindFlags            = bindFlags;
    bufferDesc.CPUAccessFlags       = cpuAccessFlags;
    bufferDesc.MiscFlags            = miscFlags;
    bufferDesc.StructureByteStride  = byteStride;

    AddBuffer(d3d, identity, bufferDesc);

    return true;
}


void Shader::RenderShader(D3D& d3d, int indexCount)
{
    // Set the vertex input layout.
    if(!m_inputLayout)
    {
        assert(true);
        return;
    }
    d3d.GetDeviceContext().IASetInputLayout(m_inputLayout);

    // Set the vertex and pixel shader.
    if(!m_vertexShader || !m_pixelShader)
    {
        assert(true);
        return;
    }
    d3d.GetDeviceContext().VSSetShader(m_vertexShader, NULL, 0);
    d3d.GetDeviceContext().PSSetShader(m_pixelShader, NULL, 0);

    // Set Hull and Domain shader, need both so only if both exist.
    if(m_hullShader && m_domainShader)
    {
        d3d.GetDeviceContext().HSSetShader(m_hullShader, NULL, 0);
        d3d.GetDeviceContext().DSSetShader(m_domainShader, NULL, 0);
    }

    // Set geom shader.
    if(m_geomShader)
    {
        d3d.GetDeviceContext().GSSetShader(m_geomShader, NULL, 0);
    }

    // Set the sampler state in pixel shader.
    if(m_sampleState)
    {
        d3d.GetDeviceContext().PSSetSamplers(0, 1, &m_sampleState);
    }

    // Render.
    d3d.GetDeviceContext().DrawIndexed(indexCount, 0, 0);
}


void Shader::OutputShaderErrorMessage(ID3D10Blob* errMsg, HWND hwnd, WCHAR* shaderFilename) const
{
    // Get pointer to the error message text buffer and buffer size.
    char* compileErrors = (char*)(errMsg->GetBufferPointer());
    int bufferSize = errMsg->GetBufferSize();

    // Open file to write error to.
    std::ofstream fout;
    fout.open("shader-error.txt");

    // Write error.
    for(int i = 0; i < bufferSize; i++)
    {
        fout << compileErrors[i];
    }

    fout.close();

    d3d_safe_release(errMsg);

    // Show message box to inform user.
    MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename,
               MB_OK);
}


bool Shader::CompileShader(HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target, 
                           ID3D10Blob* shaderBuff) const
{
    HRESULT result;
    ID3D10Blob* errMsg;
    // Compile shader.
    result = D3DCompileFromFile(filename, NULL, NULL, entryPoint, target, 0, 0, &shaderBuff, 
                                &errMsg);

    if(FAILED(result))
    {
        if(errMsg)
        {
            OutputShaderErrorMessage(errMsg, hwnd, filename);
        }
        else
        {
            MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
        }
        return false;
    }

    return true;
}

