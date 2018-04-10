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

cbuffer SUN_BUFFER : register (b2)
{
	float4 sunLightPos;
	float4 sunDir;
	float4 sunColor;
	float4x4 sunViewProjection;
}

cbuffer MOON_BUFFER : register (b3)
{
	float4 moonLightPos;
	float4 moonDir;
	float4 moonColor;
	float4x4 moonViewProjection;
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

    return float4(diffuseSample, 1);

	float3 ambient = diffuseSample * 0.2f;
	float3 finalColor; 

	//SUN//

	//Diffuse calculation////////////////////////////////////////////////////////////////////////
	float3 sunLightToObject = normalize(-sunDir.xyz);
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

	finalColor = ambient + (finalSpec + diffuse) * sunColor.xyz;

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

	finalColor = saturate(finalColor); 

	return float4(finalColor, 1);
}