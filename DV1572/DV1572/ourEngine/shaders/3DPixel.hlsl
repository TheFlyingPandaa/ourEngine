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
};

struct OUTPUT
{
	float4 diffuse	: SV_Target0;
	float4 normal	: SV_Target1;
	float4 pos		: SV_Target2;
};

OUTPUT main(INPUT input) : SV_TARGET
{
	OUTPUT output = (OUTPUT)0;
	
	output.diffuse = tDiffuse.Sample(sampAni, input.tex);
	//output.diffuse = float4(input.normal, 1);
	output.normal = float4(input.normal, 1);
	output.pos = input.worldPos;
	
	return output;
}