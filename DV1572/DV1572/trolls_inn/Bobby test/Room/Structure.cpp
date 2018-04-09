#include "Structure.h"

void Tile::setAdjacent(Tile * tile, Direction dir)
{
	this->adjacent[dir] = tile;
	
}

Tile * Tile::getAdjacent(Direction dir)
{
	return this->adjacent[dir];
}

void Tile::setRoom(Room * room)
{
	this->m_room = room;
}

XMFLOAT2 Tile::getPosition() const
{
	XMFLOAT3 posVector = quad.getPosition();
	XMFLOAT2 returnFloat = { posVector.x, posVector.z };
	return returnFloat;
}

bool Tile::operator==(const Tile & other) const
{
	return m_posX == other.m_posX && m_posY == other.m_posY;
}
