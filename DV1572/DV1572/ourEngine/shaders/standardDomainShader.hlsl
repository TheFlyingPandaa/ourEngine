cbuffer MESH_BUFFER : register(b0)
{
	float4x4 vp;
}

struct DS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
    float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3x3 TBN : TBN;
	float4 color : HIGHLIGHTCOLOR;
	float lIndex : LIGHTINDEX;

	// TODO: change/add other stuff
};

// Output control point
struct CONTROL_POINT_INPUT
{
	float4 Pos : WORLDPOS;
	float2 Tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4x4 world : WORLDMAT;
	float4 color : HIGHLIGHTCOLOR;
	float lIndex : LIGHTINDEX;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<CONTROL_POINT_INPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	float4 positions = patch[0].Pos * domain.x + patch[1].Pos * domain.y + patch[2].Pos * domain.z;
	Output.worldPos = positions;
	Output.pos = mul(positions, vp);
	Output.tex = patch[0].Tex * domain.x + patch[1].Tex * domain.y + patch[2].Tex * domain.z;

	float3 n = patch[0].normal * domain.x + patch[1].normal * domain.y + patch[2].normal * domain.z;
	Output.normal = normalize(mul(float4(n, 0), patch[0].world)).xyz;
	float3 t = patch[0].tangent * domain.x + patch[1].tangent * domain.y + patch[2].tangent * domain.z;
	t = normalize(t - dot(t, n) * n);

	Output.TBN[0] = normalize(mul(float4(t, 0), patch[0].world).xyz);
	float3 bt = normalize(cross(n, t)); //Might be other way around... lol
	Output.TBN[1] = normalize(mul(float4(bt, 0), patch[0].world).xyz);
	Output.TBN[2] = Output.normal;

	Output.color = patch[0].color * domain.x + patch[1].color * domain.y + patch[2].color * domain.z;
	Output.lIndex = patch[0].lIndex;
	return Output;
}
