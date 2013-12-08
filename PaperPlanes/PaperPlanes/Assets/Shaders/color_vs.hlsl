cbuffer MatrixBuffer
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


cbuffer LightPositionBuffer
{
    float4 lightPosIn;
};

cbuffer CameraBuffer
{
    float3 cameraPos;
    float padding;
};


struct VertexInputType
{
	float4 position : POSITION;
	float2 uv	: TEXCOORD0;
    float3 normal : NORMAL;
};


struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
    float3 viewDir  : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

PixelInputType vp_main( VertexInputType input ) 
{
	PixelInputType output;

    float4 worldPosition;

	input.position.w = 1.0f;

	output.position = mul(input.position, modelMatrix); 
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    output.uv = input.uv;

    // NOTE: This will not give correct normals if scale is in model matrix.
    output.normal = mul(input.normal, (float3x3)modelMatrix);

    output.normal = normalize(output.normal);

    worldPosition = mul(input.position, modelMatrix);
    output.lightPos = normalize(lightPosIn.xyz - worldPosition.xyz);

    output.viewDir = normalize(cameraPos.xyz - worldPosition.xyz);


	return output;
}