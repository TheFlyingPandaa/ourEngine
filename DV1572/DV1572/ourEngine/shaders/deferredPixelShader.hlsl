SamplerState sampAni;

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tPosition : register(t2);

struct Input
{
	float4 pos : SV_POSITION;
	float2 tex : TEXELS;
};

float4 main(Input input) : SV_Target
{
	float4 light = float4(0, 100, 0, 1);

	float3 wordPos = tPosition.Sample(sampAni, input.tex).rgb;
	float3 normal = tNormal.Sample(sampAni, input.tex).rgb;
	
	
	float3 diffuse = tDiffuse.Sample(sampAni, input.tex).rgb;
	float3 ambient = diffuse * 0.2;

	float diffuseFactor = max(dot(normalize(light.xyz - wordPos.xyz), normal), 0.0f);


	return float4(ambient + diffuse * diffuseFactor, 1);
}