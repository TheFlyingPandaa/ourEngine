struct INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

struct OUTPUT
{
	float4 diffuse	: SV_Target0;
};

OUTPUT main(INPUT input) : SV_TARGET
{
	OUTPUT output = (OUTPUT)0;

	output.diffuse = input.color;

	return output;
}