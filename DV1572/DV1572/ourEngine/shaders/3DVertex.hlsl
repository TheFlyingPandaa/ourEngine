cbuffer MESH_BUFFER : register(b0)
{
	float4x4 wvp; 
	float4x4 world; 
}

struct INPUT
{
	float3 pos : POSITION;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct OUTPUT
{
	//float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
};


OUTPUT main(INPUT input)
{
	OUTPUT o;
	//o.pos = mul(float4(input.pos, 1),wvp);
	//o.worldPos = mul(float4(input.pos, 1), world);
	o.worldPos = float4(input.pos,1);
	o.tex = input.tex;
	o.normal = input.normal;
	//o.normal = mul(float4(input.normal, 1), world).xyz;

	return o;
}