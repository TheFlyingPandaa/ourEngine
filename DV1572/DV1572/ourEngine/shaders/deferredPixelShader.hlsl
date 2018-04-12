SamplerState sampAni;

Texture2D tDiffuse : register(t0);
Texture2D tNormal : register(t1);
Texture2D tPosition : register(t2);
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
	float4x4 worldMatrix;
	float4x4 view;
	float4x4 projection;
};

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

	//return tShadow.Sample(sampAni, input.tex);

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
	finalColor = ambient + (diffuse + finalSpec) * sunColor.xyz;
	finalColor = saturate(finalColor); 

	float4 posLightH = mul(float4(wordPos,1.0f), mul(view, projection)); // Translate the world position into the view space of the light
	//posLightH = mul(posLightH, lightProj);					// Translate the view position into the projection space of the light
	posLightH.xy /= posLightH.w;							// Get the texture coords of the "object" in the shadow map
	if (abs(posLightH.x) > 1.0f)							// Check if we are outside the shadow map (we are not in the light)
		return float4(finalColor, 1);
	if (abs(posLightH.y) > 1.0f)							// Check if we are outside the shadow map (we are not in the light)
		return float4(finalColor, 1);
	float2 smTex = float2(0.5f * posLightH.x + 0.5f, -0.5f * posLightH.y + 0.5f);	// Texcoords are not [-1, 1], change the coords to [0, 1]
	float depth = posLightH.z / posLightH.w;										// Get the actual depth (seen from the camera)
	float SHADOW_EPSILON = 0.001f;													// Some small value
	float shadowCoeff = (tShadow.Sample(sampAni, smTex).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;	// If the depth from camera is larger than depth from light,

	finalColor *= shadowCoeff;

	return float4(finalColor, 1);
}