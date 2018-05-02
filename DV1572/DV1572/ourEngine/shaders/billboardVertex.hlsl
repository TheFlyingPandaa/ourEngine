cbuffer BILLBOARD_MESH_BUFFER : register(b0)
{
	float4x4 View;
	float4x4 Projection;
	float spriteIndex;
}

struct INPUT
{
	float3 pos : POSITION;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;

	float4 w4 : INSTANCEWORLDFOUR;

	float4 color : HIGHLIGHTCOLOR;
	float4 charDir : CHARDIR;
	float lIndex : LIGHTINDEX;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
	float2 tex : TEXELS;
	float3 normal : NORMAL;
	float3x3 TBN : TBN;
	float4 color : HIGHLIGHTCOLOR;
	float lIndex : LIGHTINDEX;
};


OUTPUT main(INPUT input)
{
	OUTPUT o;

	float4x4 world = { input.w4 , input.w4 , input.w4 , input.w4 };
	float4 position = float4(input.w4.xyz,1.0f);
	float3 cameraRightWorld = float3(View[0][0], View[1][0], View[2][0]);
	float3 cameraUpWorld = float3(View[0][1], View[1][1], View[2][1]);

	float3 rotatedAndLol = position.xyz + float3(0,1.5,0) * input.pos.y + (cameraRightWorld*1.5) * input.pos.x;
	rotatedAndLol.y += 0.2f;
	
	float4x4 vp = mul(View, Projection);
	o.pos = mul(float4(rotatedAndLol, 1.0f), View);
	float4 viewPosition = o.pos;
	o.pos.w = 1.0f;
	o.pos = mul(o.pos, Projection);
	o.worldPos = float4(rotatedAndLol, 1.0f);
	o.tex = input.tex;
	float3 charDir = input.charDir.xyz;
	o.color = input.color;
	if (spriteIndex != -1)
	{
		// Increase in X is to swap direction

		if(charDir.z == -1)
			o.tex.x = o.tex.x + 0.75f;
		else if(charDir.z == 1)
			o.tex.x = o.tex.x + 0.5f;
		else if (charDir.x == 1)
			o.tex.x = o.tex.x + 0.5f;

		//float3 camDir = normalize(-viewPosition);

		////float angle = acos(dot(camDir, tempDir)) * 57;
		//float d = dot(camDir, tempDir);
		//float3 C = cross(camDir, tempDir);
		//float angle = acos(d);
		//float dir = dot(C, camDir);
		//if (dir < 0) angle = -angle;

		//if (angle < 0)
		//{
		//}
		//
		//// Increase in Y is to swap in animation
		
		o.tex.y = o.tex.y + (0.25f*spriteIndex);
		//o.color = float4(angle, angle, angle, 1.0f);
	}

	o.normal = input.normal;

	o.TBN[0] = normalize(mul(float4(input.tangent, 0), world)).xyz;
	float3 bt = normalize(cross(input.normal, input.tangent));
	o.TBN[1] = normalize(mul(float4(bt, 0), world)).xyz;
	o.TBN[2] = normalize(mul(float4(input.normal, 0), world)).xyz;

	o.lIndex = input.lIndex;

	return o;
}