cbuffer MESH_BUFFER : register(b0)
{
	float4x4 vp; 
	float inside;
	float inside1;
	float inside2;
	float inside3;
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
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4x4 world : WORLDMAT;
	float4 color : HIGHLIGHTCOLOR;
	float inside : INSIDECHECK;
};


OUTPUT main(INPUT input)
{
	OUTPUT o;
	float4x4 world = { input.w1, input.w2, input.w3, input.w4 };
	o.world = world;
	o.worldPos = mul(float4(input.pos,1), world);
	o.tex = input.tex;
	o.normal = input.normal;
	o.tangent = input.tangent;
	o.color = input.color;
	o.inside = inside;
	return o;
}