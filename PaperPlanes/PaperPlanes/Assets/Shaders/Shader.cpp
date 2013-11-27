#include "Shader.h"

#include <d3dcompiler.h>

#include <fstream>


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

Shader::Shader(void) : 
    m_Initialized(false),
    m_vertexShader(0),
    m_pixelShader(0),
    m_inputLayout(0)
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


bool Shader::SetSampleState(D3D& d3d, D3D11_SAMPLER_DESC samplerDesc)
{
    HRESULT result = d3d.GetDevice().CreateSamplerState(&samplerDesc, &m_sampleState);
    if(FAILED(result))
        return false;

    return true;
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