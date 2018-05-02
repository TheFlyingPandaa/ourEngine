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
	float gridscaleX;
	float gridscaleY;
};

struct BILLBOARD_MESH_BUFFER
{
	DirectX::XMFLOAT4X4A View;
	DirectX::XMFLOAT4X4A Projection;
	float spriteIndex;
};

struct PICK_BUFFER
{
	DirectX::XMFLOAT4A index;
	DirectX::XMFLOAT4X4A MVP;
};

struct POINT_LIGHT_COLLECTION
{
	DirectX::XMFLOAT4A positionArray[100];
	DirectX::XMFLOAT4A colorArray[100];
	//Setup//
	//r Range 
	//g Constant Attenuation
	//b Inverse Attenuation
	//a Square Inverse Attenuation
	DirectX::XMFLOAT4A lightSetup[100];
	DirectX::XMFLOAT4A nrOfLights; 
};
struct SHADOW_MATRIX_BUFFER
{
	DirectX::XMFLOAT4X4A view;
	DirectX::XMFLOAT4X4A projection;
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

