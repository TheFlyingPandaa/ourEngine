cbuffer MATRIX_BUFFER : register(b9)
{
	float4x4 view;
	float4x4 projection;
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

	float4 color : HIGHLIGHTCOLOR;
};

struct OUTPUT
{
	//float4 pos : SV_POSITION;
	//float4 color : COLOR;
};

float4 main( INPUT input) : SV_POSITION
{

	float4x4 world = { input.w1, input.w2, input.w3, input.w4 };
	float4 p = mul(float4(input.pos,1), world);
	p = mul(p, mul(view, projection));

	return p;
}