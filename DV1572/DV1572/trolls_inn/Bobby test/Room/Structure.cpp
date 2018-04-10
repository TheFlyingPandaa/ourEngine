#include "Structure.h"

void Tile::setAdjacent(Tile * tile, Direction dir)
{
	this->adjacent[dir] = tile;
	
}

Room * Tile::getRoom() const
{
	return this->m_room;
}

Tile * Tile::getAdjacent(Direction dir)
{
	return this->adjacent[dir];
}

bool Tile::getIsWalkeble() const
{
	return m_isWalkeble;
}

void Tile::setTileWalls(Direction dir, Wall* value)
{
	m_w[dir] = value;
}

Wall* Tile::getTileWalls(Direction dir) const
{
	return m_w[dir];
}

bool Tile::getWallSpotPopulated(Direction dir) const
{
	return this->m_walls[dir];
}

Object3D & Tile::getQuad()
{
	return quad;
}

void Tile::setMesh(Mesh * mesh)
{
	this->quad.setMesh(mesh);
}

void Tile::setWallSpotPopulated(Direction dir, bool value)
{
	this->m_walls[dir] = value;
}

void Tile::setRoom(Room * room)
{
	this->m_room = room;
}

void Tile::setInside(bool value)
{
	this->m_inside = value;
}

void Tile::setIsWalkeble(bool value)
{
	this->m_isWalkeble = value;
}

int Tile::getPosX() const
{
	return 0;
}

int Tile::getPosY() const
{
	return 0;
}

XMFLOAT2 Tile::getPosition() const
{
	XMFLOAT3 posVector = quad.getPosition();
	XMFLOAT2 returnFloat = { posVector.x, posVector.z };
	return returnFloat;
}

bool Tile::operator==(const Tile & other) const
{
	XMFLOAT3 pos = quad.getPosition();
	XMFLOAT3 otherPos = other.quad.getPosition();
	return pos.x == otherPos.x && pos.y == otherPos.y && pos.z == otherPos.z;
}
