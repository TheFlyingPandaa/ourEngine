#pragma once
struct VERTEX
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
	float tx, ty, tz;
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