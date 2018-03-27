cbuffer PICK_BUFFER : register(b0)
{
	float4 index;
	float4x4 WVP;
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
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};


OUTPUT main(INPUT input)
{
	OUTPUT o;
	o.pos = mul(float4(input.pos, 1), WVP);
	o.color = index;
	return o;
}