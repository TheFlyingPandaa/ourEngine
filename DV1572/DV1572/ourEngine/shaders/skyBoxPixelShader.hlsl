Texture2D tDiffuse : register(t0);

SamplerState SampleType;

struct INPUT {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

struct OUTPUT
{
	float4 diffuse	: SV_Target0;
};

OUTPUT main(INPUT input)// : SV_TARGET
{
	OUTPUT o;
	o.diffuse = tDiffuse.Sample(SampleType, input.tex);
	return o;
}