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

void Furniture::setPosition(DirectX::XMFLOAT3 pos)
{
	p_object.setPos(pos);
	this->pos.x = static_cast<int>(pos.x);
	this->pos.y = static_cast<int>(pos.y);
	this->pos.z = static_cast<int>(pos.z);
}

void Furniture::setPosition(float x, float y, float z)
{
	p_object.setPos(x, y, z);
	this->pos.x = static_cast<int>(x);
	this->pos.y = static_cast<int>(y);
	this->pos.z = static_cast<int>(z);
}

void Furniture::setRotation(int rot)
{
	p_rot = rot;
	p_object.setRotation(0, rot * (-90), 0);
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
