#include "Structure.h"



Wall::Wall(Tile* tile, Mesh * mesh)
{
	this->m_tile = tile;
	if (mesh)
		this->m_wall.setMesh(mesh);
}


Wall::~Wall()
{
}

void Wall::setIsDoor(bool value)
{
	this->isDoor = value;
}

bool Wall::getIsDoor() const
{
	return isDoor;
}

void Wall::Draw()
{
	//TODO: Remove Debug
	m_wall.Draw();
		//m_wall.Scale(1, .2f, 1);
}

bool Wall::getIsInner() const
{
	return m_innerWall;
}

void Wall::setIsInner(bool isInner)
{
	m_innerWall = isInner;
}

void Wall::setPosition(DirectX::XMFLOAT2 position)
{
	this->position = DirectX::XMINT2(static_cast<int>(position.x + 0.5f), static_cast<int>(position.y + 0.5f));
	this->m_wall.setPos(position.x, 0, position.y);
}

void Wall::setRotation(DirectX::XMFLOAT3 rotation)
{
	m_wall.Rotate(rotation);
}

void Wall::setScale(float x, float y, float z)
{
	this->m_wall.setScale(x, y, z);
}

DirectX::XMINT2 Wall::getPosition() const
{
	return position;
}

void Wall::setMesh(Mesh * mesh)
{
	this->m_wall.setMesh(mesh);
}

Tile * Wall::getTile() const
{
	return m_tile;
}

bool Wall::operator==(const Wall & other)
{
	return false;

	
}



bool Wall::getDir(Direction dir) const
{
	return m_wallDir[dir];
}
