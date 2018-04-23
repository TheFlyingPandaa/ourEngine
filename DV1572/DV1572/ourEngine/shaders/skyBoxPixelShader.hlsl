SamplerState SampleType : register(s0);
Texture2D tDiffuse : register(t0);

cbuffer SUN_BUFFER : register (b2)
{
	float4 sunLightPos;
	float4 sunDir;
	float4 sunColor;
	float4x4 sunViewProjection;
}


struct INPUT {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

float4 main(INPUT input) : SV_TARGET
{
	float3 down = float3(0,-1.0f,0);
	float3 direction = float3(16, 0, 16) - sunLightPos.xyz;
	float angle = max(dot(down, normalize(direction)), 0.0f);
	float4 diffuseTexture = tDiffuse.Sample(SampleType, input.tex);
	float4 ambient = diffuseTexture * 0.1f;
	ambient.a = 1.0f;
			
	float4 final = diffuseTexture * (sunColor * angle);

	return ambient + final;
}