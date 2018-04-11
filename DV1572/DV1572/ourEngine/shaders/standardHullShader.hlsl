// Input control point
struct VS_CONTROL_POINT_INPUT
{
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4x4 world : WORLDMAT;
	float4 color : HIGHLIGHTCOLOR;
	float inside : INSIDECHECK;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float4 Pos : WORLDPOS;
	float2 Tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4x4 world : WORLDMAT;
	float4 color : HIGHLIGHTCOLOR;
	float inside : INSIDECHECK;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_INPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] = 
	Output.EdgeTessFactor[1] = 
	Output.EdgeTessFactor[2] = 
	Output.InsideTessFactor = 1.0f; // e.g. could calculate dynamic tessellation factors instead
	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_INPUT, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.Pos = ip[i].worldPos;
	Output.Tex = ip[i].tex;
	Output.normal = ip[i].normal;
	Output.tangent = ip[i].tangent;
	Output.world = ip[i].world;
	Output.color = ip[i].color;
	Output.inside = ip[i].inside;
	
	return Output;
}
