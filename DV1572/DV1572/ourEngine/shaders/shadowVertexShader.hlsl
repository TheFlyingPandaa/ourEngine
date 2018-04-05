cbuffer MESH_BUFFER : register(b0)
{
	float4x4 vp;
}

struct OUTPUT {
	float4 pos : SV_POSITION;
};

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

OUTPUT main(INPUT input)
{
	OUTPUT o;
	float4x4 world = { input.w1, input.w2, input.w3, input.w4 };
	world = mul(world, vp);

	o.pos = mul(float4(input.pos,1), world);
	return o;
}