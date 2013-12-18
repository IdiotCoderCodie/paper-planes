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
    m_buffers(),
    m_samplerStates()
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

    for(auto samplerState : m_samplerStates)
    {
        d3d_safe_release(samplerState.second);
        samplerState.second = 0;
    }
    m_samplerStates.clear();

    for(auto buffer : m_buffers)
    {
        d3d_safe_release(buffer.second);
        buffer.second = 0;
    }
    m_buffers.clear();
}


bool Shader::SetVertexShader(D3D& d3d, HWND hwnd, WCHAR* filename, CHAR* entryPoint, CHAR* target,
                             D3D11_INPUT_ELEMENT_DESC polygonLayout[], int numElems)
{
    ID3D10Blob* shaderBuff = 0;
    // Compile Shader.
    if(!CompileShader(hwnd, filename, entryPoint, target, &shaderBuff))
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
    result = d3d.GetDevice().CreateInputLayout(polygonLayout, numElems, 
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
    if(!CompileShader(hwnd, filename, entryPoint, target, &shaderBuff))
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
    if(!CompileShader(hwnd, filename, entryPoint, target, &shaderBuff))
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
    if(!CompileShader(hwnd, filename, entryPoint, target, &shaderBuff))
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
    if(!CompileShader(hwnd, filename, entryPoint, target, &shaderBuff))
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


bool Shader::AddSamplerState(D3D& d3d, const std::string& identity, D3D11_SAMPLER_DESC& samplerDesc)
{
    if(m_samplerStates.find(identity) != m_samplerStates.end())
    {
        // SamplerState with "identity" already been added.
        return false;
    }

    ID3D11SamplerState* newSamplerState;
    HRESULT result = d3d.GetDevice().CreateSamplerState(&samplerDesc, &newSamplerState);

    if(FAILED(result))
    {
        return false;
    }

    // Add newly created SamplerState to the map.
    m_samplerStates[identity] = newSamplerState;

    newSamplerState = 0;
    return true;
}


bool Shader::AddBuffer(D3D& d3d, const std::string& identity, D3D11_BUFFER_DESC& bufferDesc)
{
    // Check if a buffer with this identity already exists.
    if(m_buffers.find(identity) != m_buffers.end())
        return false;

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

    return AddBuffer(d3d, identity, bufferDesc);
}

//
//template<class T>
//bool Shader::SetConstBufferData(D3D& d3d, std::string& id, T* data, int bufferNumber) 
//{
//    D3D11_MAPPED_SUBRESOURCE mappedResource;
//
//    // Does buffer with match id exist?
//    if(m_buffers.find(id) == m_buffers.end())
//        return false;
//    
//    HRESULT result;
//    result = d3d.GetDeviceContext().Map(m_buffers[id], 0, D3D11_MAP_WRITE_DISCARD, 0, 
//                                        &mappedResource);
//    if(FAILED(result))
//        return false;
//
//    T* bufferDataPtr = (T*)mappedResource.pData;
//
//    // Copy data into the constant buffer.
//    *bufferDataPtr = *data;
//
//    d3d.GetDeviceContext().Unmap(m_buffers[id], 0);
//
//    return true;
//}


bool Shader::VSSetConstBufferData(D3D& d3d, std::string& id, void* data, size_t size, 
                                  int bufferNumber) 
{
    // Does buffer with match id exist?
    if(m_buffers.find(id) == m_buffers.end())
        return false;

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HRESULT result;
    result = d3d.GetDeviceContext().Map(m_buffers[id], 0, D3D11_MAP_WRITE_DISCARD, 0, 
                                        &mappedResource);
    if(FAILED(result))
        return false;

    void* bufferDataPtr = mappedResource.pData;

    // Copy data into the constant buffer.
    memcpy(bufferDataPtr, data, size);

    d3d.GetDeviceContext().Unmap(m_buffers[id], 0);

    // Set const buffer with updated.
    d3d.GetDeviceContext().VSSetConstantBuffers(bufferNumber, 1, &m_buffers[id]);

    return true;
}

bool Shader::HSSetConstBufferData(D3D& d3d, std::string& id, void* data, size_t size, 
                                  int bufferNumber) 
{
    // Does buffer with match id exist?
    if(m_buffers.find(id) == m_buffers.end())
        return false;

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HRESULT result;
    result = d3d.GetDeviceContext().Map(m_buffers[id], 0, D3D11_MAP_WRITE_DISCARD, 0, 
                                        &mappedResource);
    if(FAILED(result))
        return false;

    void* bufferDataPtr = mappedResource.pData;

    // Copy data into the constant buffer.
    memcpy(bufferDataPtr, data, size);

    d3d.GetDeviceContext().Unmap(m_buffers[id], 0);

    // Set const buffer with updated.
    d3d.GetDeviceContext().HSSetConstantBuffers(bufferNumber, 1, &m_buffers[id]);

    return true;
}

bool Shader::DSSetConstBufferData(D3D& d3d, std::string& id, void* data, size_t size, 
                                  int bufferNumber) 
{
    // Does buffer with match id exist?
    if(m_buffers.find(id) == m_buffers.end())
        return false;

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HRESULT result;
    result = d3d.GetDeviceContext().Map(m_buffers[id], 0, D3D11_MAP_WRITE_DISCARD, 0, 
                                        &mappedResource);
    if(FAILED(result))
        return false;

    void* bufferDataPtr = mappedResource.pData;

    // Copy data into the constant buffer.
    memcpy(bufferDataPtr, data, size);

    d3d.GetDeviceContext().Unmap(m_buffers[id], 0);

    // Set const buffer with updated.
    d3d.GetDeviceContext().DSSetConstantBuffers(bufferNumber, 1, &m_buffers[id]);

    return true;
}

bool Shader::GSSetConstBufferData(D3D& d3d, std::string& id, void* data, size_t size, int 
                                  bufferNumber) 
{
    // Does buffer with match id exist?
    if(m_buffers.find(id) == m_buffers.end())
        return false;

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HRESULT result;
    result = d3d.GetDeviceContext().Map(m_buffers[id], 0, D3D11_MAP_WRITE_DISCARD, 0, 
                                        &mappedResource);
    if(FAILED(result))
        return false;

    void* bufferDataPtr = mappedResource.pData;

    // Copy data into the constant buffer.
    memcpy(bufferDataPtr, data, size);

    d3d.GetDeviceContext().Unmap(m_buffers[id], 0);

    // Set const buffer with updated.
    d3d.GetDeviceContext().GSSetConstantBuffers(bufferNumber, 1, &m_buffers[id]);

    return true;
}

bool Shader::PSSetConstBufferData(D3D& d3d, std::string& id, void* data, size_t size, int 
                                  bufferNumber) 
{
    // Does buffer with match id exist?
    if(m_buffers.find(id) == m_buffers.end())
        return false;

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HRESULT result;
    result = d3d.GetDeviceContext().Map(m_buffers[id], 0, D3D11_MAP_WRITE_DISCARD, 0, 
                                        &mappedResource);
    if(FAILED(result))
        return false;

    void* bufferDataPtr = mappedResource.pData;

    // Copy data into the constant buffer.
    memcpy(bufferDataPtr, data, size);

    d3d.GetDeviceContext().Unmap(m_buffers[id], 0);

    // Set const buffer with updated.
    d3d.GetDeviceContext().PSSetConstantBuffers(bufferNumber, 1, &m_buffers[id]);

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

    // Set the sampler states.
    int i = 0;
    for(auto samplerState : m_samplerStates)
    {
        // NOTE: This isn't entirely correct. Might have a sampler in another shader!
        d3d.GetDeviceContext().PSSetSamplers(i, 1, &samplerState.second);
        i++;
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
                           ID3D10Blob** shaderBuff) const
{
    HRESULT result;
    ID3D10Blob* errMsg = 0;
    // Compile shader.
    result = D3DCompileFromFile(filename, NULL, NULL, entryPoint, target, 0, 0, shaderBuff, 
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

