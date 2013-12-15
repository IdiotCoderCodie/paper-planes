#pragma once
#include <unordered_map>

#include "Shader.h"


class ShaderManager
{
public:
    ShaderManager(D3D& d3d);

    bool LoadShaders(const std::string& configFilename);

    Shader* GetShader(const std::string& id);

    ~ShaderManager(void);

private:
    D3D& m_d3dInstance;
    std::unordered_map<std::string, Shader> m_shaders;
};

