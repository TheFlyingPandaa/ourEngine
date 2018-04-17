struct INPUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

float4 main(INPUT input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 0.2f);
}