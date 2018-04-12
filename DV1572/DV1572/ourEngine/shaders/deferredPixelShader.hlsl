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

cbuffer POINT_LIGHT_COLLECTION_BUFFER : register (b6)
{
	float4 pointLPos[100];
	float4 pointLColor[100];
	float4 lightSetup[100]; 
	float4 nrOfLights; 
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
	float3 finalColorForSun; 
	float mul = 1;

	if (inside)
		mul = 0;

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
	float spec = pow(max(dot(normal, halfWayDir), 0.0f), 32.0f);

	float3 finalSpec = spec * specLevel;
	///////////////////////////////////////////////////////////////////////////////////////////
	finalColorForSun = ambient + (diffuse + finalSpec) * sunColor * mul;
	

	float3 finalColorForPointLights = float3(0,0,0);
	float3 tempColor = float3(0,0,0);
	float3 diffuseForPointLight = float3(0,0,0); 
	float3 halfWayDirPointLight = float3(0,0,0); 
	float specPointLight = 0;
	float3 finalSpecPointLight = float3(0,0,0);
	float att = 0; 
	float specLevelPointLight = 0.2f; 

	for (int i = 0; i < 2; i++)
	{
		//Diffuse 
		float3 pointLightToObject = normalize(pointLPos[i] - wordPos);
		diffuseForPointLight = diffuseSample * max(dot(normal, pointLightToObject), 0.0f); 
		//Specular
		halfWayDirPointLight = normalize(pointLightToObject + viewer); 
		specPointLight = pow(max(dot(normal, halfWayDirPointLight), 0.0f), 32.0f); 
		finalSpecPointLight = specPointLight * specLevelPointLight; 

		//PointLight Attenuation

		float distanceFromPointLight = length(wordPos - pointLPos[i]);
		att = 1.0f / (lightSetup[i].g + lightSetup[i].b * distanceFromPointLight + lightSetup[i].a * distanceFromPointLight);
		
		tempColor = (diffuseForPointLight + finalSpecPointLight) * att * pointLColor[i];

		finalColorForPointLights += tempColor;
	}

	float3 finalColor = saturate(finalColorForSun + finalColorForPointLights);

	return float4(finalColor,1);
}