SamplerState sampAni;

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tHighlight : register(t2);

struct INPUT
{
	float4 worldPos : SV_POSITION;
	float2 tex : TEXELS;
};

float4 main(INPUT input) : SV_TARGET
{
	return float4(tDiffuse.Sample(sampAni, input.tex));
}