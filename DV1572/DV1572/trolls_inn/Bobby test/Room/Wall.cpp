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

void Wall::Draw()
{
	//TODO: Remove Debug
	if (m_innerWall)
		m_wall.setScale(1.0f, 0.1f, 1);
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
	this->m_wall.setPos(position.x, 0, position.y);
}

void Wall::setRotation(DirectX::XMFLOAT3 rotation)
{
	m_wall.Rotate(rotation);
}

Tile * Wall::getTile() const
{
	return m_tile;
}

bool Wall::getDir(WallDirection dir) const
{
	return m_wallDir[dir];
}
