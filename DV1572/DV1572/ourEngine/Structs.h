#pragma once
#include <DirectXMath.h>

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

struct CAMERA_POS_BUFFER
{
	DirectX::XMFLOAT4A pos;
};

struct POINT_LIGHT_BUFFER
{
	DirectX::XMFLOAT4A pos, color;
};

struct DIRECTIONAL_LIGHT_BUFFER
{
	DirectX::XMFLOAT4A pos, dir, color;
	DirectX::XMFLOAT4X4A viewProjection;
};

struct MESH_BUFFER
{
	DirectX::XMFLOAT4X4A VP;
	float inside;
	float inside2;
	float inside3;
	float inside4;
};

struct PICK_BUFFER
{
	DirectX::XMFLOAT4A index;
	DirectX::XMFLOAT4X4A MVP;
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