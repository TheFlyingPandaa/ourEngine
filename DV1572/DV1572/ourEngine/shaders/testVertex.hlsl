cbuffer MESH_BUFFER : register(b0)
{
	float4x4 wvp; 
	float4x4 world; 
}


struct INPUT
{
	float3 pos : POSITION;
	float2 uv : TEXELS;
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
	o.pos = mul(float4(input.pos, 1),wvp);
	o.color = float4(input.pos + 0.5f, 1);

	return o;
}