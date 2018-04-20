SamplerState sampAni : register(s0);
SamplerComparisonState sampAniPoint : register(s1);

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tPosition : register(t2);
Texture2D tLIndex : register(t3);
Texture2D tShadow : register(t4);

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

cbuffer LIGHTMATRIXBUFFER : register(b9)
{
	float4x4 view;
	float4x4 projection;
};

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
	float specLevel = tPosition.Sample(sampAni, input.tex).a;
	float3 normal = tNormal.Sample(sampAni, input.tex).rgb;
	float3 diffuseSample = tDiffuse.Sample(sampAni, input.tex).rgb;
	float inside = tNormal.Sample(sampAni, input.tex).a;

	//return tShadow.Sample(sampAni, input.tex);

	float3 ambient = diffuseSample * 0.2f;
	float3 finalColorForSun;


	//SUN//



	//Diffuse calculation////////////////////////////////////////////////////////////////////////
	float3 sunLightToObject = normalize(-sunDir.xyz);
	//TODO:Hey Future me Remove this
	//return float4(diffuseSample,1);
	float3 diffuse = diffuseSample * max(dot(normal, sunLightToObject), 0.0f);

	////////////////////////////////////////////////////////////////////////////////////////////



	//Specular calculation//////////////////////////////////////////////////////////////////////
	float3 viewer = normalize(camPos.xyz - wordPos);

	float3 halfWayDir;
	/*WWWHOOOOOOOOO WE'RE...*/ halfWayDir = normalize(sunLightToObject + viewer);

	//spec lowest value is 32.
	float spec = pow(max(dot(normal, halfWayDir), 0.0f), 32.0f);

	float3 finalSpec = spec * specLevel;
	///////////////////////////////////////////////////////////////////////////////////////////

	//float4 lightPos = view[3];
	float4 posLightH = mul(float4(wordPos, 1.0f), mul(view, projection));
	float shadowCoeff = 1;
	float2 shadowTexCoords;
	shadowTexCoords.x = 0.5f + (posLightH.x / posLightH.w * 0.5f);
	shadowTexCoords.y = 0.5f - (posLightH.y / posLightH.w * 0.5f);
	float pixelDepth = posLightH.z / posLightH.w;

	if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
		(saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
		pixelDepth > 0)
	{
		float margin = acos(saturate(dot(normal,sunLightToObject)));

		float epsilon = 0.001 / margin;

		epsilon = clamp(epsilon, 0, 0.1);
		float width;
		tShadow.GetDimensions(width, width);
		float texelSize = 1.0f / width;
		for (int x = -5; x <= 5; ++x)
		{
			for (int y = -5; y <= 5; ++y)
			{
				shadowCoeff += float(tShadow.SampleCmpLevelZero(sampAniPoint, shadowTexCoords + (float2(x,y) * texelSize), pixelDepth + epsilon));
			}
		}
		shadowCoeff /= 100.0f;
		shadowCoeff = max(shadowCoeff, 0.2);

	}
	

	finalColorForSun = ambient + (diffuse + finalSpec) * sunColor.rgb * shadowCoeff;
	





	float3 finalColorForPointLights = float3(0,0,0);
	float3 tempColor = float3(0,0,0);
	float3 diffuseForPointLight = float3(0,0,0); 
	float3 halfWayDirPointLight = float3(0,0,0); 
	float specPointLight = 0;
	float3 finalSpecPointLight = float3(0,0,0);
	float att = 0; 
	float specLevelPointLight = specLevel * 0.2f;

	float3 lIndex = tLIndex.Sample(sampAni, input.tex).rgb;
	int fIndex = (int)(lIndex.x + (lIndex.y * 256) + (lIndex.z * 256 * 256) + 0.5f);
	
	for (int i = 0; i < nrOfLights.r; i++)
	{
		int index = (int)(pointLColor[i].a + 0.5f);
		if (index == fIndex)
		{
			//Diffuse 
			float3 pointLightToObject = normalize(pointLPos[i].xyz - wordPos);
			diffuseForPointLight = diffuseSample * max(dot(normal, pointLightToObject), 0.0f);
			//Specular
			halfWayDirPointLight = normalize(pointLightToObject + viewer);
			specPointLight = pow(max(dot(normal, halfWayDirPointLight), 0.0f), 32.0f);
			finalSpecPointLight = specPointLight * specLevelPointLight;

			//PointLight Attenuation

			float distanceFromPointLight = length(wordPos - pointLPos[i].xyz);
			att = lightSetup[i].r / (lightSetup[i].g + lightSetup[i].b * distanceFromPointLight + lightSetup[i].a * distanceFromPointLight);

			tempColor = (diffuseForPointLight + finalSpecPointLight) * att * pointLColor[i].xyz;

			finalColorForPointLights += tempColor;
		}
	}

	float3 finalColor = saturate(finalColorForSun + finalColorForPointLights);

	return float4(finalColor, 1);
}