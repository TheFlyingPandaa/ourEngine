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

void Wall::WindowDraw()
{
	m_wall.WindowDraw();
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

void Wall::setIsShared(bool isShared)
{
	m_sharedWall = isShared;
}

bool Wall::isShared() const
{
	return m_sharedWall;
}


bool Wall::operator==(const Wall & other)
{
	return false;

	
}

XMINT2 Wall::getNormalPosition()
{
	XMINT2 position = { (int)m_wall.getPosition().x, (int)m_wall.getPosition().z };

	return XMINT2(position.x + m_direction.x, position.y + m_direction.y);
}

XMINT2 Wall::getNegativeNormalPosition()
{
	XMINT2 position = { (int)m_wall.getPosition().x, (int)m_wall.getPosition().z };

	return XMINT2(position.x - m_direction.x, position.y - m_direction.y);
}

XMFLOAT2 Wall::getDirection() const
{
	return m_direction;
}
