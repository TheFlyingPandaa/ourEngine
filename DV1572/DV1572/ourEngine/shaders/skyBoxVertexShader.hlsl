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

	float4 color : HIGHLIGHTCOLOR;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

OUTPUT main(INPUT input)
{
	OUTPUT o;

	float4x4 world = { input.w1, input.w2, input.w3, input.w4 };
	o.pos = mul(float4(input.pos,1), world);
	o.pos = mul(o.pos, vp);
	//o.pos.z = o.pos.w - 0.0000001f;
	o.tex = input.tex;

	return o;
}