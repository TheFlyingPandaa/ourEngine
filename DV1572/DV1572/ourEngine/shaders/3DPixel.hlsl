SamplerState sampAni;

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tHighlight : register(t2);

struct INPUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3x3 TBN : TBN;
};

struct OUTPUT
{
	float4 diffuse	: SV_Target0;
	float4 normal	: SV_Target1;
	float4 pos		: SV_Target2;
};

OUTPUT main(INPUT input)// : SV_TARGET
{
	OUTPUT output = (OUTPUT)0;
	
	float3 normal = (2.0f * tNormal.Sample(sampAni, input.tex) - 1.0f).xyz;
	normal = normalize(mul(normal, input.TBN));
	output.normal = float4(normalize(input.normal + normal), 0.0f);
	output.diffuse = tDiffuse.Sample(sampAni, input.tex);
	output.pos = input.worldPos;
	
	return output;
}