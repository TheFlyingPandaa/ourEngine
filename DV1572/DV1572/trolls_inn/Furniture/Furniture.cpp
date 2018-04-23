#include "Furniture.h"

Furniture::Furniture(DirectX::XMFLOAT3 pos, Mesh * mesh)
{
	p_rot = 0;
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.z = pos.z;
}

Furniture::~Furniture()
{
}

Object3D Furniture::getObject3D()
{
	return p_object;
}

Attributes& Furniture::getAttributes()
{
	return p_attributes;
}

int Furniture::getGridSize()
{
	return p_gridSize;
}

DirectX::XMINT3 Furniture::getPosition()
{
	return this->pos;
}

int Furniture::getRotation()
{
	return p_rot;
}



void Furniture::Draw()
{
	p_object.Draw();
}
