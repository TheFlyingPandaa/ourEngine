cbuffer MESH_BUFFER : register(b0)
{
	float4x4 vp;
}

struct INPUT
{
	float3 pos : POSITION;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;


	float4 w1 : INSTANCEWORLDONE;
	float4 w2 : INSTANCEWORLDTWO;
	float4 w3 : INSTANCEWORLDTHREE;
	float4 w4 : INSTANCEWORLDFOUR;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

OUTPUT main(INPUT input, uint id : SV_InstanceID)
{
	OUTPUT o;
	float4x4 world = { input.w1, input.w2, input.w3, input.w4 };
    o.pos = mul(float4(input.pos, 1), world);

	o.pos = mul(o.pos, vp);

	uint index = id + 1;
	o.color.w = 1.0f;
	o.color.r = index % 256;
	index /= 256;
	o.color.g = index % 256;
	index /= 256;
	o.color.b = index % 256;

	return o;
}