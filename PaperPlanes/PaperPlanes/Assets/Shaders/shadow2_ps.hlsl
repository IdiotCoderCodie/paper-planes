Texture2D modelTexture      : register(t0);
Texture2D shadowMapTexture  : register(t1);
Texture2D shadowMapTexture2 : register(t2);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);

cbuffer LightColorBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 diffuseColor2;
};


struct PixelInputType
{
    float4 position           : SV_POSITION;
    float2 uv                 : TEXCOORD0;
    float3 normal             : NORMAL;
    float4 lightViewPosition  : TEXCOORD1;
    float3 lightPos           : TEXCOORD2;
    float4 lightViewPosition2 : TEXCOORD3;
    float3 lightPos2          : TEXCOORD4;
};


float4 ps_main(PixelInputType input) : SV_TARGET
{
    // Because of the low floating point precision of depth map, need a bias.
    float bias = 0.001f;

    float4 color = ambientColor;
    float lightIntensity = 0.0f;

    //----------------------------------------------------------------------------------------------
    // First light.
    // Calculate projected texture coords.
    float2 projectTexCoord;
    projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

    // Check if projected coords are within 0 to 1, if they are the pixel is in view of the light.
    if( (saturate(projectTexCoord.x) == projectTexCoord.x) && 
        (saturate(projectTexCoord.y) == projectTexCoord.y) )
    {
        // Sample the shadow map at projected texture coordinate location.
        float depthValue = shadowMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

        // Calculate the depth of the light.
        float lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

        // Subtract bias to get around floating point precision issue.
        lightDepthValue = lightDepthValue - bias;

        // Compare light depth and object depth.
        if(lightDepthValue < depthValue)
        {
            // Light the pixel, light is in front of object.
            lightIntensity = saturate(dot(input.normal, input.lightPos));

            if(lightIntensity > 0.0f)
            {
                // Get the final diffus color.
                color += (diffuseColor * lightIntensity);

                // Saturate final color
                //color = saturate(color);
            }
        }
    }
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Second light.
    projectTexCoord.x =  (input.lightViewPosition2.x / input.lightViewPosition2.w / 2.0f) + 0.5f;
    projectTexCoord.y = -(input.lightViewPosition2.y / input.lightViewPosition2.w / 2.0f) + 0.5f;

    if( (saturate(projectTexCoord.x) == projectTexCoord.x) && 
        (saturate(projectTexCoord.y) == projectTexCoord.y) )
    {
        float depthValue2 = shadowMapTexture2.Sample(SampleTypeClamp, projectTexCoord).r;

        float lightDepthValue2 = input.lightViewPosition2.z / input.lightViewPosition2.w;
        lightDepthValue2 = lightDepthValue2 - bias;

        if(lightDepthValue2 < depthValue2)
        {
            lightIntensity = saturate(dot(input.normal, input.lightPos2));
            if(lightIntensity > 0.0f)
            {
                color += (diffuseColor2 * lightIntensity);
            }
        }

    }
    //----------------------------------------------------------------------------------------------

    color = saturate(color);


    // Sample the pixel color from the texture.
    float4 textureColor = modelTexture.Sample(SampleTypeWrap, input.uv);

    //Combine final colors.
    color = color * textureColor;

    return color;
}
