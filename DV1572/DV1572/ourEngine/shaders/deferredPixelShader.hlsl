SamplerState sampAni;

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tPosition : register(t2);


cbuffer CAMERA_POS_BUFFER : register(b1)
{
	float4 camPos;
}

cbuffer SUN_BUFFER : register (b2)
{
	float4 sunLightPos;
	float4 sunDir;
	float4 sunColor;
	float4x4 sunViewProjection;
}

struct Input
{
	float4 pos : SV_POSITION;
	float2 tex : TEXELS;
};

float4 main(Input input) : SV_Target
{
	float3 wordPos = tPosition.Sample(sampAni, input.tex).rgb;
	float3 normal = tNormal.Sample(sampAni, input.tex).rgb;
	float3 diffuseSample = tDiffuse.Sample(sampAni, input.tex).rgb;
	float inside = tNormal.Sample(sampAni, input.tex).a;

	float3 ambient = diffuseSample * 0.2f;
	float3 finalColor; 

	if (inside)
		return float4(ambient, 1);

	//SUN//
	
	//Diffuse calculation////////////////////////////////////////////////////////////////////////
	float3 sunLightToObject = normalize(-sunDir);
	//TODO:Hey Future me Remove this
	//return float4(diffuseSample,1);
	float3 diffuse = diffuseSample * max(dot(normal, sunLightToObject), 0.0f);
	
	////////////////////////////////////////////////////////////////////////////////////////////

	//Specular calculation//////////////////////////////////////////////////////////////////////
	float specLevel = 0.5f;
	float3 viewer = normalize(camPos.xyz - wordPos);

	float3 halfWayDir;
	/*WWWHOOOOOOOOO WE'RE...*/ halfWayDir = normalize(sunLightToObject + viewer);

	//spec lowest value is 32.
	float spec = pow(max(dot(normal, halfWayDir), 0.0f), 32.0);

	float3 finalSpec = spec * specLevel;
	///////////////////////////////////////////////////////////////////////////////////////////

	//float distanceSquare = length(wordPos - sunLightPos.xyz) * length(wordPos - sunLightPos.xyz);

	
	finalColor = ambient + ((finalSpec + diffuse) * sunColor.xyz * 2.0f);
	finalColor = saturate(finalColor);
    //MOON//

	//Diffuse calculation////////////////////////////////////////////////////////////////////////
	/*float3 moonLightToObject = normalize(-moonDir.xyz);
	//TODO:Hey Future me Remove this
	//return float4(diffuseSample,1);
	diffuse = diffuseSample * max(dot(normal, moonLightToObject), 0.0f);
	////////////////////////////////////////////////////////////////////////////////////////////

	//Specular calculation//////////////////////////////////////////////////////////////////////

	/*WWWHOOOOOOOOO WE'RE...*/ /*halfWayDir = normalize(moonLightToObject + viewer);

	//spec lowest value is 32.
	spec = pow(max(dot(normal, halfWayDir), 0.0f), 32.0);

	finalSpec = spec * specLevel;

	finalColor += ambient + (finalSpec + diffuse) * moonColor.xyz;*/


	return float4(finalColor, 1);
}