SamplerState sampAni : register(s0);
SamplerComparisonState sampAniPoint : register(s1);

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tHighlight : register(t2);
Texture2D tShadow : register(t4);

cbuffer EVERYTHING_BUFFER : register(b1)
{
	float4 camPos;
	float4 pointLPos[100];
	float4 pointLColor[100];
	float4 lightSetup[100];
	float4 nrOfLights;
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

struct INPUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3x3 TBN : TBN;
	float4 color : HIGHLIGHTCOLOR;
	float lIndex : LIGHTINDEX;
};

float4 main(INPUT input) : SV_Target
{
	float3 wordPos = input.worldPos.xyz;
	float3 normal = (2.0f * tNormal.Sample(sampAni, input.tex) - 1.0f).xyz;
	normal = normalize(mul(normal, input.TBN));
	normal = normalize(input.normal + normal);
	float4 diffuseSample = tDiffuse.Sample(sampAni, input.tex);

	if (diffuseSample.a < 0.5f)
		discard;

	
	diffuseSample *= input.color;
	float3 ambient = diffuseSample.xyz * 0.5f;
	float specLevel = 1; // Vet inte om detta var rätt!





	uint index = round(input.lIndex);
	
	float3 finalColorForSun;
	//SUN//

	//Diffuse calculation////////////////////////////////////////////////////////////////////////
	float3 sunLightToObject = normalize(sunLightPos.xyz - wordPos);
	float3 diffuse = diffuseSample.xyz * max(dot(normal, sunLightToObject), 0.0f);

	////////////////////////////////////////////////////////////////////////////////////////////



	//Specular calculation//////////////////////////////////////////////////////////////////////
	float3 viewer = normalize(camPos.xyz - wordPos);

	float3 halfWayDir;
	/*WWWHOOOOOOOOO WE'RE...*/ halfWayDir = normalize(sunLightToObject + viewer);

	//spec lowest value is 32.
	float spec = pow(max(dot(normal, halfWayDir), 0.0f), 32.0f);

	float3 finalSpec = spec * specLevel * 0.5f;
	///////////////////////////////////////////////////////////////////////////////////////////

	//float4 lightPos = view[3];
	float4 posLightH = mul(float4(wordPos, 1.0f), mul(view, projection));
	float shadowCoeff = 1;
	float2 shadowTexCoords;
	shadowTexCoords.x = 0.5f + (posLightH.x * 0.5f);
	shadowTexCoords.y = 0.5f - (posLightH.y * 0.5f);
	float pixelDepth = posLightH.z;

	//float window = 1.0f;

	// If we are in shadow
	if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
		(saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
		pixelDepth > 0)
	{

		float margin = acos(saturate(dot(normal, sunLightToObject)));

		float epsilon = 0.001 / margin;

		epsilon = clamp(epsilon, 0, 0.1);
		float width;
		tShadow.GetDimensions(width, width);
		float texelSize = 1.0f / width;

		for (int x = -1; x <= 1; ++x)
		{
			for (int y = -1; y <= 1; ++y)
			{
				shadowCoeff += float(tShadow.SampleCmpLevelZero(sampAniPoint, shadowTexCoords + (float2(x, y) * texelSize), pixelDepth + epsilon));
			}
		}
		shadowCoeff /= 9.0f;
		shadowCoeff = max(shadowCoeff, 0.2);
		
	}

	finalColorForSun = ambient + (diffuse + finalSpec)* sunColor.xyz * shadowCoeff;

	float3 finalColorForPointLights = float3(0, 0, 0);
	float3 tempColor = float3(0, 0, 0);
	float3 diffuseForPointLight = float3(0, 0, 0);
	float3 halfWayDirPointLight = float3(0, 0, 0);
	float specPointLight = 0;
	float3 finalSpecPointLight = float3(0, 0, 0);
	float att = 0;
	float specLevelPointLight = specLevel * 0.2f;

	int fIndex = index;
	for (int i = 0; i < nrOfLights.r; i++)
	{
		int index = pointLColor[i].a;
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

			finalColorForPointLights = tempColor;
		}
	}

	float3 finalColor = min(finalColorForSun + finalColorForPointLights, float3(1, 1, 1));

	return float4(finalColor, 1);
}