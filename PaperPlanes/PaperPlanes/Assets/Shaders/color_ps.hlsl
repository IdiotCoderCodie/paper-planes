Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightColorBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float  specularPow;
    float3 padding;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
    float3 viewDir  : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};


float4 ps_main(PixelInputType input) : SV_TARGET
{
    float4 color = ambientColor;
    float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);

    float lightIntensity = saturate(dot(input.normal, input.lightPos));

    if(lightIntensity > 0.0f)
    {
        // Determine final diffuse based on ambient and diffuse intensity.
        color += (diffuseColor * lightIntensity);
        color = saturate(color);

        // Calculate the reflection vector for spec lighting.
        float3 reflection = normalize(2 * lightIntensity * input.normal - input.lightPos);
        // Calc amount of specular light.
        specular = pow(saturate(dot(reflection, input.viewDir)), specularPow);
    }

    // Multiply texture pixel and color.
    float4 texColor = shaderTexture.Sample(SampleType, input.uv);
    color = color * texColor;

    color = saturate(color + specular);

    return color;
}