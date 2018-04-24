SamplerState sampAni : register(s0);

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tHighlight : register(t2);
Texture2D tLIndex : register(t3);

struct INPUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3x3 TBN : TBN;
	float4 color : HIGHLIGHTCOLOR;
	float lIndex : LIGHTINDEX;
};

struct OUTPUT
{
	float4 diffuse	: SV_Target0;
	float4 normal	: SV_Target1;
	float4 pos		: SV_Target2;
	float4 lIndex	: SV_Target3;
};

OUTPUT main(INPUT input)// : SV_TARGET
{
	OUTPUT output = (OUTPUT)0;
	
	float3 normal = (2.0f * tNormal.Sample(sampAni, input.tex) - 1.0f).xyz;
	normal = normalize(mul(normal, input.TBN));
	output.normal = float4(normalize(input.normal + normal), 1.0f);
	output.diffuse = tDiffuse.Sample(sampAni, input.tex);
	if (output.diffuse.r == 0 && output.diffuse.g == 1 && output.diffuse.b == 0)
		discard;
	output.diffuse = output.diffuse * input.color;
	output.pos = input.worldPos;

	

    uint index = round(input.lIndex);
	output.lIndex.a = 1.0f;

	output.lIndex.r = index % 256;
	index /= 256;
	output.lIndex.g = index % 256;
	index /= 256;
	output.lIndex.b = index % 256;
	


	return output;
}