#include "Structure.h"
#include <iostream>


Wall::Wall(Mesh * mesh, XMFLOAT2 dir)
{
	m_wall = new Object3D();
	m_wall->setMesh(mesh);
	m_direction = dir;
}


Wall::~Wall()
{
	delete m_wall;
}

void Wall::Draw()
{
	if (m_wall)
	{
		if (m_wall->GetMesh())
		{
			m_wall->Draw();
		}
	}
}

void Wall::WindowDraw()
{
	m_wall->WindowDraw();
}

void Wall::setPosition(float x, float y)
{
	setPosition(XMFLOAT2(x, y));
}

void Wall::setPosition(DirectX::XMFLOAT2 position)
{
	XMFLOAT2 position2 = DirectX::XMFLOAT2(position.x, position.y);
	this->m_wall->setPos(position2.x, 0, position2.y);
}

void Wall::setRotation(float x, float y, float z)
{
	setRotation(XMFLOAT3(x, y, z));
}

void Wall::setRotation(DirectX::XMFLOAT3 rotation)
{
	m_wall->setRotation(rotation);
}

void Wall::setScale(float x, float y, float z)
{
	this->m_wall->setScale(x, y, z);
}

void Wall::setMesh(Mesh * mesh)
{
	this->m_wall->setMesh(mesh);
}

void Wall::setIsShared(bool isShared)
{
	m_sharedWall = isShared;
}

bool Wall::isShared() const
{
	return m_sharedWall;
}

bool Wall::getIsDoor() const
{
	return m_isDoor;
}

bool Wall::setIsDoor(const bool tjue)
{
	return m_isDoor = tjue;
}


bool Wall::operator==(const Wall & other)
{
	return false;

	
}
// round float to n decimals precision
float round2_n(float num, int dec)
{
	float m = (num < 0.0f) ? -1.0f : 1.0f;   // check if input is negative
	float pwr = pow(10.0f, dec);
	return float((float)floor((double)num * m * pwr + 0.5) / pwr) * m;
}
XMINT2 Wall::getNormalPosition()
{
	XMINT2 position;
	if(m_direction.x == 1.0 && m_direction.y == 0)
		position = { (int)(round2_n(m_wall->getPosition().x,1)), (int)(round2_n(m_wall->getPosition().z ,1))};
	else if (m_direction.x == 0.0 && m_direction.y == -1)
		position = { (int)(round2_n(m_wall->getPosition().x,1)), (int)(round2_n(m_wall->getPosition().z - 1 ,1)) };
	else if (m_direction.x == 0.0 && m_direction.y == 1)
		position = { (int)(round2_n(m_wall->getPosition().x,1)), (int)(round2_n(m_wall->getPosition().z ,1)) };
	else
		position = { (int)(round2_n(m_wall->getPosition().x - 1,1)), (int)(round2_n(m_wall->getPosition().z ,1)) };
	return position;
}

XMINT2 Wall::getNegativeNormalPosition()
{
	XMINT2 position;
	if (m_direction.x == 1.0 && m_direction.y == 0)
		position = { (int)(round2_n(m_wall->getPosition().x - m_direction.x,1)), (int)(round2_n(m_wall->getPosition().z + m_direction.y ,1)) };
	else if (m_direction.x == 0.0 && m_direction.y == -1)
		position = { (int)(round2_n(m_wall->getPosition().x,1)), (int)(round2_n(m_wall->getPosition().z ,1)) };
	else if (m_direction.x == 0.0 && m_direction.y == 1)
		position = { (int)(round2_n(m_wall->getPosition().x,1)), (int)(round2_n(m_wall->getPosition().z - 1,1)) };
	else
		position = { (int)(round2_n(m_wall->getPosition().x ,1)), (int)(round2_n(m_wall->getPosition().z  ,1)) };
	return position;
}

XMFLOAT2 Wall::getDirection() const
{
	return m_direction;
}
