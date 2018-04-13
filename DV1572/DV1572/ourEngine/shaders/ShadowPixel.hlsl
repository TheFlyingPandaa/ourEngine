
struct INPUT
{
	//float4 pos : SV_POSITION;
	//float4 color : COLOR;
};

float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
	return pos;
}