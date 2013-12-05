
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
    float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
};


float4 ps_main(PixelInputType input) : SV_TARGET
{
	//return input.color;

    return float4(normalize(abs(input.normal)), 1.0f);
   /* return float4(input.normal.x, input.normal.y, 
                  -input.normal.z, 1.0f);*/
}