Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightColorBuffer
{
    float4 ambientColor1;   
    float4 diffuseColor1;    
    float4 specularColor1;    
    float  specularPow1;   
    float  padding;

    float4 ambientColor2;
    float4 diffuseColor2;
    float4 specularColor2;
    float  specularPow2;
    float  padding2;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
    float3 viewDir  : TEXCOORD1;
    float3 lightPos1 : TEXCOORD2;
    float3 lightPos2 : TEXCOORD3;
};


float4 ps_main(PixelInputType input) : SV_TARGET
{
    float4 color1 = ambientColor1;
    float4 color2 = ambientColor2;

    float4 specular1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 specular2 = float4(0.0f, 0.0f, 0.0f, 1.0f);

    float lightIntensity1 = saturate(dot(input.normal, input.lightPos1));
    float lightIntensity2 = saturate(dot(input.normal, input.lightPos2));

    float3 reflection;
    if(lightIntensity1 > 0.0f)
    {
        color1           += saturate(diffuseColor1 * lightIntensity1);
        reflection = normalize(2 * lightIntensity1 * input.normal - input.lightPos1);
        specular1         = pow(saturate(dot(reflection, input.viewDir)), specularPow1);
        specular1 = saturate(specular1 * specularColor1);
    }

    if(lightIntensity2 > 0.0f)
    {
        color2           += saturate(diffuseColor2 * lightIntensity2);
        reflection  = normalize(2 * lightIntensity2 * input.normal - input.lightPos2);
        specular2         = pow(saturate(dot(reflection, input.viewDir)), specularPow2);
        specular2 = saturate(specular2 * specularColor2);
    }
    
    float4 specular = specular1 + specular2;

   /* color1 = saturate(color1 + specular1);
    color2 = saturate(color2 + specular2);*/

    float4 color = saturate(color1 + color2);

    // Multiply texture pixel and color.
    float4 texColor = shaderTexture.Sample(SampleType, input.uv);
    color = color * texColor;

    color = saturate(color + specular);

    return color;
}