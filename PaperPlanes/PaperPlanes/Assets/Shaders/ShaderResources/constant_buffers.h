#pragma once
#include  "../../../glm/glm.hpp"

namespace ConstantBuffers
{
    struct MVPBuffer
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };


    struct ProjectionOnlyBuffer
    {
        glm::mat4 projectionMat;
    };


    struct CameraPosBuffer
    {
        glm::vec3 cameraPos;
        float padding;
    };


    struct LightColorBuffer
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float specPow;
        glm::vec3 padding;
    };

    struct LightPosBuffer
    {
        glm::vec4 lightPosition;
    };
};