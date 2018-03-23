#pragma once
struct VERTEX
{
	float x, y, z;		//Position
	float u, v;			//Texel
	float nx, ny, nz;	//Normal
	float tx, ty, tz;	//Tangent
};

struct CAMERA_BUFFER
{
	DirectX::XMFLOAT4A pos, target, up;
	DirectX::XMFLOAT4X4A viewMatrix;
};

struct POINT_LIGHT_BUFFER
{
	DirectX::XMFLOAT4A pos, color;
};

struct DIRECTIONAL_LIGHT_BUFFER
{
	DirectX::XMFLOAT4A pos, dir, color;
};

struct MESH_BUFFER
{
	DirectX::XMFLOAT4X4A MVP, world;
};

struct V
{
	float x, y, z;
};
struct VN
{
	float x, y, z;
};
struct VT
{
	float u, v;
};
struct F
{
	int vIndex, vnIndex, vtIndex;
};