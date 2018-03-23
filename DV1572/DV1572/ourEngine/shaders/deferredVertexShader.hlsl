
struct IOput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXELS;
};

IOput main(uint id : SV_VertexID )
{
	IOput output = (IOput)0;
	if (id == 0) // Bottom left
	{
		output.pos = float4(-1.0f, -1.0f, 0.0f, 1.0f);
		output.tex = float2(0.0f, 1.0f);
	}
	else if (id == 1) // Top left
	{
		output.pos = float4(-1.0f, 1.0f, 0.0f, 1.0f);
		output.tex = float2(0.0f, 0.0f);
	}
	else if (id == 2) // Bottom right
	{
		output.pos = float4(1.0f, -1.0f, 0.0f, 1.0f);
		output.tex = float2(1.0f, 1.0f);
	}
	else if (id == 3) // Top right
	{
		output.pos = float4(1.0f, 1.0f, 0.0f, 1.0f);
		output.tex = float2(1.0f, 0.0f);
	}
	return output;
}