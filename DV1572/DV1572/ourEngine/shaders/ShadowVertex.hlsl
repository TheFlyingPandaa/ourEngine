struct INPUT
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLDPOS;
    float2 tex : TEXELS;
    float3 normal : NORMAL;
    float3x3 TBN : TBN;
    float4 color : HIGHLIGHTCOLOR;
    float inside : INSIDECHECK;
};

cbuffer MATRIX_BUFFER : register(b9)
{
    float4x4 view;
    float4x4 proj;
}


float4 main( INPUT input : POSITION ) : SV_POSITION
{
    float4x4 viewProj = mul(view, proj);
    return mul(input.worldPos, viewProj);
}