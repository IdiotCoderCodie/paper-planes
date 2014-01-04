#define MAX_SHADOWCASTING_LIGHTS 1
#define BIAS 0.001

Texture2D modelTexture      : register(t0);
Texture2D shadowMapTexture1 : register(t1);
Texture2D shadowMapTexture2 : register(t2);
Texture2D normalTexture		: register(t3);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);

struct light
{
    bool   enabled;
    float4 position;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float  spotCutoff;
    float3 spotDirection;
    float  spotExponent;
    float3 attenuation;
    float3 padding;
};


StructuredBuffer<light> LightBuffer : register(t4);


cbuffer CameraBuffer
{
    float3 cameraPosition;
    float  padding;
};


struct PixelInputType
{
    float4 position           : SV_POSITION;
    //float4 worldPosition      : TEXCOORD5;
    float2 uv                 : TEXCOORD0;
    float3 normal             : NORMAL;
    float3 tangent			  : TANGENT;
    float3 binormal           : BINORMAL;
    float4 light1ViewPosition : TEXCOORD1;
    float3 lightPos           : TEXCOORD2;
    float4 lightViewPosition2 : TEXCOORD3;
    float3 lightPos2          : TEXCOORD4;
};


void accumulateLights(StructuredBuffer<light> lights, float3 pos, float3 norm, float3 eye, 
                      float shininess, inout float4 ambient, inout float4 diffuse,
                      inout float4 specular, PixelInputType input)
{
    uint numLights = 0;
    uint dummy = 0;
    lights.GetDimensions(numLights, dummy);

    for(uint index = 0; index < numLights; ++index)
    {
        if(lights[index].enabled)
        {
            // Light is active.
            // Get the light vector.
            float3 lightVec = normalize(input.lightPos);
            
            float NdotL = max(dot(norm, lightVec), 0.0);
            float2 projectTexCoord;
            
            if(index < MAX_SHADOWCASTING_LIGHTS)
            {
                // Calculate projected texture coords.
                projectTexCoord.x =  input.light1ViewPosition.x / input.light1ViewPosition.w / 2.0f + 0.5f;
                projectTexCoord.y = -input.light1ViewPosition.y / input.light1ViewPosition.w / 2.0f + 0.5f;
                
                // Check if projected coords are within 0 to 1. If so, the pixel is in view of the light.
                if( (saturate(projectTexCoord.x) == projectTexCoord.x) &&
                    (saturate(projectTexCoord.y) == projectTexCoord.y) )
                {
                    // Sample the shadow map at projected tex coordinate.
                    float depthValue = shadowMapTexture1.Sample(SampleTypeClamp, projectTexCoord).r;

                    // Calculate the depth of the light.
                    float lightDepthValue = input.light1ViewPosition.z / input.light1ViewPosition.w;

                    lightDepthValue = lightDepthValue - BIAS;

                    if(lightDepthValue < depthValue)
                    {
                        if(NdotL > 0.0f)
                        {
                            float3 H         = normalize(lightVec - normalize(pos - eye));
                            float  NdotH     = max(dot(norm, H), 0.0);
                            float  exponent  = max(128.0 / max(0.0, min(128.0, shininess)), 0.0);
                            float4 litResult = lit(NdotL, NdotH, exponent);

                            // Light the pixel.
                            ambient  += lights[index].ambient  * litResult.x;
                            diffuse  += lights[index].diffuse  * litResult.y;
                            specular += lights[index].specular * litResult.z;
                        }
                    }
                }
            }
        }
    }
}

float4 ps_main(PixelInputType input) : SV_TARGET
{
    /*if(LightBuffer[0].enabled)
        return LightBuffer[0].diffuse;
    else
        return LightBuffer[0].ambient;*/

    float4 color = float4(0.0, 0.0, 0.0, 1.0);
    float lightIntensity = 0.0f;

    // Sample the normal texture, put int range -1.0 to +1.0.
    float4 bumpMap    = (normalTexture.Sample(SampleTypeWrap, input.uv) * 2.0f) - 1.0f;
    //return bumpMap;
	float3 bumpNormal = normalize( 
                        input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal 
                        ); 
    float4 ambient  = float4(0.0, 0.0, 0.0, 1.0);
    float4 diffuse  = float4(0.0, 0.0, 0.0, 1.0);
    float4 specular = float4(0.0, 0.0, 0.0, 1.0);

    accumulateLights(LightBuffer, input.position, bumpNormal, cameraPosition, 0.0, 
                     ambient, diffuse, specular, input);

    color += ambient;
    color += diffuse;
    //color += specular;

    color = saturate(color);

    // Sample the pixel color from the texture.
    float4 textureColor = modelTexture.Sample(SampleTypeWrap, input.uv);
    //return textureColor;

    //Combine final colors.
    color = color * textureColor;

    return color;
}
