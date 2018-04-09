#include "Structure.h"

void Tile::setAdjacent(Tile * tile, Direction dir)
{
	this->adjacent[dir] = tile;
	
}

Tile * Tile::getAdjacent(Direction dir)
{
	return this->adjacent[dir];
}

void Tile::setAdjacent(Tile * tile, int dir)
{
	this->adjacent[dir] = tile;

}

Tile * Tile::getAdjacent(int dir)
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
	XMFLOAT3 pos = quad.getPosition();
	XMFLOAT3 otherPos = other.quad.getPosition();
	return pos.x == otherPos.x && pos.y == otherPos.y && pos.z == otherPos.z;
}
