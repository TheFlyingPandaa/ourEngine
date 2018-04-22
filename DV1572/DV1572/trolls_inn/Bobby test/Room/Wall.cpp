#include "Structure.h"



Wall::Wall(Mesh * mesh, XMFLOAT2 dir)
{
	m_wall.setMesh(mesh);
	m_direction = dir;
}


Wall::~Wall()
{
}

void Wall::Draw()
{
	m_wall.Draw();		
}

void Wall::setPosition(float x, float y)
{
	setPosition(XMFLOAT2(x, y));
}

void Wall::setPosition(DirectX::XMFLOAT2 position)
{
	XMFLOAT2 position2 = DirectX::XMFLOAT2(position.x, position.y);
	this->m_wall.setPos(position2.x, 0, position2.y);
}

void Wall::setRotation(float x, float y, float z)
{
	setRotation(XMFLOAT3(x, y, z));
}

void Wall::setRotation(DirectX::XMFLOAT3 rotation)
{
	m_wall.Rotate(rotation);
}

void Wall::setScale(float x, float y, float z)
{
	this->m_wall.setScale(x, y, z);
}

void Wall::setMesh(Mesh * mesh)
{
	this->m_wall.setMesh(mesh);
}


bool Wall::operator==(const Wall & other)
{
	return false;

	
}

XMFLOAT2 Wall::getDirection() const
{
	return m_direction;
}
