#include "Structure.h"

void Tile::setAdjacent(Tile * tile, Direction dir)
{
	this->adjacent[dir] = tile;
	
}

Tile * Tile::getAdjacent(Direction dir)
{
	return this->adjacent[dir];
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
	return this->m_posX;
}

int Tile::getPosY() const
{
	return this->m_posY;
}
