cbuffer BILLBOARD_MESH_BUFFER : register(b0)
{
	float4x4 View;
	float4x4 Projection;
	float4	CamPos;
	float4	CamDir;
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
	float spriteIndex : SPRITEINDEX;
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
	o.pos.w = 1.0f;
	o.pos = mul(o.pos, Projection);
	o.worldPos = float4(rotatedAndLol, 1.0f);
	o.tex = input.tex;
	o.color = input.color;
	if (input.spriteIndex != -1)
	{
		float3 charDir = input.charDir.xyz; // Character Direction

		if (!(charDir.z == -1 && charDir.x == -1 && charDir.y == -1))
		{
			float3 CamView = float3(View[0][2], View[1][2], View[1][2]); // CamLookAt
			float3 camTarget = normalize(CamDir.xyz);

			float angle = dot(camTarget, charDir);
			float dirChange = cross(camTarget, charDir).y;

			if (charDir.z)
			{
				angle *= -1;
				dirChange *= -1;
			}
			if (angle > 0.66)
				o.tex = input.tex;		// DEFAULT = UP
			else if (angle <= 0.66 && angle > -0.66)
			{
				if (dirChange < 0)
					o.tex.x = o.tex.x + 0.25f; // LEFT
				else
					o.tex.x = o.tex.x + 0.50f; // RIGHT
			}
			else
				o.tex.x = o.tex.x + 0.75f; // DOWN

		}
		o.tex.y = o.tex.y + (0.25f*input.spriteIndex);
	}
	o.normal = input.normal;

	o.TBN[0] = normalize(mul(float4(input.tangent, 0), world)).xyz;
	float3 bt = normalize(cross(input.normal, input.tangent));
	o.TBN[1] = normalize(mul(float4(bt, 0), world)).xyz;
	o.TBN[2] = normalize(mul(float4(input.normal, 0), world)).xyz;

	o.lIndex = input.lIndex;

	return o;
}