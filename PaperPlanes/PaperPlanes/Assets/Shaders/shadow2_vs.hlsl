cbuffer MatrixBuffer
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
    matrix lightViewMatrix2;
    matrix lightProjectionMatrix2;
};

cbuffer LightPositionBuffer
{
    float3 lightPosition;
    float padding;
    float3 lightPosition2;
    float padding2;
};


struct VertexInputType
{
    float4 position : POSITION;
    float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
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


PixelInputType vp_main( VertexInputType input )
{
    PixelInputType output;
    
    input.position.w = 1.0f;

    // Multiply position by MVP.
    output.position = mul(input.position,  modelMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Get position from light's perspective.
    output.lightViewPosition = mul(input.position,  modelMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

    output.lightViewPosition2 = mul(input.position, modelMatrix);
    output.lightViewPosition2 = mul(output.lightViewPosition2, lightViewMatrix2);
    output.lightViewPosition2 = mul(output.lightViewPosition2, lightProjectionMatrix2);

    output.uv = input.uv;

    // Calculate normal against model matrix. 
    // NOTE: will not work if modelMatrix contains scalars.
    output.normal = normalize( mul(input.normal, (float3x3)modelMatrix) );

    float4 worldPosition = mul(input.position, modelMatrix);

    // Determine light position based off position of vertex in world space.
    output.lightPos  = normalize(lightPosition.xyz - worldPosition.xyz);
    output.lightPos2 = normalize(lightPosition2.xyz - worldPosition.xyz);

	return output;
}
