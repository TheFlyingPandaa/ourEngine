cbuffer BILLBOARD_MESH_BUFFER : register(b0)
{
	float4x4 View;
	float4x4 Projection;
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
	float lIndex : LIGHTINDEX;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3x3 TBN : TBN;
	float4 color : HIGHLIGHTCOLOR;
	float lIndex : LIGHTINDEX;
};


OUTPUT main(INPUT input)
{
	OUTPUT o;

	float4x4 world = { input.w1, input.w2, input.w3, input.w4 };
	float4 position = float4(input.w4.xyz,1.0f);
	float3 cameraRightWorld = float3(View[0][0], View[1][0], View[2][0]);
	float3 cameraUpWorld = float3(View[0][1], View[1][1], View[2][1]);

	float3 rotatedAndLol = position.xyz + float3(0,1,0) * input.pos.y + cameraRightWorld * input.pos.x;
	
	float4x4 vp = mul(View, Projection);
	o.pos = mul(float4(rotatedAndLol, 1.0f), vp);
	o.worldPos = float4(rotatedAndLol, 1.0f);
	o.tex = input.tex;
	o.normal = input.normal;

	o.TBN[0] = normalize(mul(float4(input.tangent, 0), world)).xyz;
	float3 bt = normalize(cross(input.normal, input.tangent));
	o.TBN[1] = normalize(mul(float4(bt, 0), world)).xyz;
	o.TBN[2] = normalize(mul(float4(input.normal, 0), world)).xyz;

	o.color = input.color;
	o.lIndex = input.lIndex;

	return o;
}