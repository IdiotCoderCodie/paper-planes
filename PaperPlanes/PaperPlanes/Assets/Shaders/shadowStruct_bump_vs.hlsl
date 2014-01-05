#define MAX_SHADOWCASTING_LIGHTS 2

cbuffer MatrixBuffer
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix[MAX_SHADOWCASTING_LIGHTS];
    matrix lightProjectionMatrix[MAX_SHADOWCASTING_LIGHTS];
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
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
};


struct PixelInputType
{
    float4 position                                     : SV_POSITION;
    float2 uv                                           : TEXCOORD0;
    float3 normal                                       : NORMAL;
    float3 tangent			                            : TANGENT;
    float3 binormal                                     : BINORMAL;
    float4 lightViewPosition[MAX_SHADOWCASTING_LIGHTS]  : TEXCOORD1;
    float3 lightPos[MAX_SHADOWCASTING_LIGHTS]           : COLOR0;
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
    output.lightViewPosition[0] = mul(input.position,  modelMatrix);
    output.lightViewPosition[0] = mul(output.lightViewPosition[0], lightViewMatrix[0]);
    output.lightViewPosition[0] = mul(output.lightViewPosition[0], lightProjectionMatrix[0]);

    output.lightViewPosition[1] = mul(input.position, modelMatrix);
    output.lightViewPosition[1] = mul(output.lightViewPosition[1], lightViewMatrix[1]);
    output.lightViewPosition[1] = mul(output.lightViewPosition[1], lightProjectionMatrix[1]);

    output.uv = input.uv;

    // Calculate normal against model matrix. 
    // NOTE: will not work if modelMatrix contains scalars.
    output.normal   = normalize( mul(input.normal,   (float3x3)modelMatrix) );
    output.tangent  = normalize( mul(input.tangent,  (float3x3)modelMatrix) );
    output.binormal = normalize( mul(input.binormal, (float3x3)modelMatrix) );

    float4 worldPosition = mul(input.position, modelMatrix);

    // Determine light position based off position of vertex in world space.
    output.lightPos[0] = normalize(lightPosition.xyz - worldPosition.xyz);
    output.lightPos[1] = normalize(lightPosition2.xyz - worldPosition.xyz);

    return output;
}
