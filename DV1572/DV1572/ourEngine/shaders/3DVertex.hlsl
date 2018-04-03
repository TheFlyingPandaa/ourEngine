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
	float4 w1 : COLONE;
	float4 w2 : COLTWO;
	float4 w3 : COLTHREE;
	float4 w4 : COLFOUR;
};

struct OUTPUT
{
	//float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4x4 worldMat : HENRIK;
	float4x4 wvp : WVP;
};


OUTPUT main(INPUT input, uint lol : SV_InstanceID)
{
	OUTPUT o;
	//o.pos = mul(float4(input.pos, 1),wvp);
	//o.worldPos = mul(float4(input.pos, 1), world);
	float4x4 world;
	world[0] = input.w1;
	world[1] = input.w2;
	world[2] = input.w3;
	world[3] = input.w4;
	
	o.worldMat = world;
	o.worldPos = mul(float4(input.pos,1), o.worldMat);
	o.wvp = world * wvp;
	o.worldPos.x += lol;
	o.tex = input.tex;
	o.normal = input.normal;
	o.tangent = input.tangent;
	//o.normal = mul(float4(input.normal, 1), world).xyz;

	return o;
}