SamplerState sampAni;

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tPosition : register(t2);

cbuffer LIGHT_BUFFER : register(b0)
{
	float4 lightPos; 
	float4 dir; 
	float4 color; 
	float4x4 viewProjection; 
}

cbuffer CAMERA_POS_BUFFER : register(b1)
{
	float4 camPos;
}

struct Input
{
	float4 pos : SV_POSITION;
	float2 tex : TEXELS;
};

float4 main(Input input) : SV_Target
{
	float4 light = float4(0, 100, 0, 1);

	float3 wordPos = tPosition.Sample(sampAni, input.tex).rgb;
	float3 normal = tNormal.Sample(sampAni, input.tex).rgb;
	float3 diffuseSample = tDiffuse.Sample(sampAni, input.tex).rgb;
	float3 ambient = diffuseSample * 0.2f;
	
	//Diffuse calculation////////////////////////////////////////////////////////////////////////
	float3 lightToObject = normalize(-dir.xyz); 
	//TODO:Hey Future me Remove this
	//return float4(diffuseSample,1);
	float3 diffuse = diffuseSample * max(dot(normal, lightToObject), 0.0f); 
	////////////////////////////////////////////////////////////////////////////////////////////

	//Specular calculation//////////////////////////////////////////////////////////////////////
	float specLevel = 0.5f; 
	float3 viewer = normalize(camPos.xyz - wordPos); 

	float3 halfWayDir; 
	/*WWWHOOOOOOOOO WE'RE...*/ halfWayDir = normalize(lightToObject + viewer); 

	//spec lowest value is 32.
	float spec = pow(max(dot(normal, halfWayDir), 0.0f), 32.0); 

	float3 finalSpec = spec * specLevel; 
	///////////////////////////////////////////////////////////////////////////////////////////


	float3 finalColor = ambient + (finalSpec + diffuse) * color.xyz; 

	return float4(finalColor, 1);
}