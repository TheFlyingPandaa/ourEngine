SamplerState sampAni : register(s0);

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tHighlight : register(t2);

struct INPUT
{
	float4 worldPos : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : HIGHLIGHTCOLOR;
};

float4 main(INPUT input) : SV_TARGET
{
	float4 finalColor = tDiffuse.Sample(sampAni, input.tex);

	float3 modified = (finalColor.xyz == float3(0.0f, 0.0f, 0.0f)) ? float3(0.05f, 0.05f, 0.05f) : finalColor.xyz;
	finalColor.xyz = modified.xyz;

	return finalColor * input.color;
	
}