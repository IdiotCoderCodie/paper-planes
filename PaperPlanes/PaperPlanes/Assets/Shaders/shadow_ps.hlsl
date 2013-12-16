Texture2D modelTexture      : register(t0);
Texture2D shadowMapTexture  : register(t1);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);

cbuffer LightColorBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
};


struct PixelInputType
{
    float4 position          : POSITION;
    float2 uv                : TEXCOORD0;
    float3 normal            : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos          : TEXCOORD2;
};


float4 ps_main(PixelInputType input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);

    // Because of the low floating point precision of depth map, need a bias.
    float bias = 0.001f;

    float4 color = ambientColor;
    float lightIntensity = 0.0f;

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
        lightDepthValue -= bias;

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
                color = saturate(color);
            }
        }
    }

    // Sample the pixel color from the texture.
    float4 textureColor = modelTexture.Sample(SampleTypeWrap, input.uv);

    //Combine final colors.
    color = color * textureColor;

    return color;
}
