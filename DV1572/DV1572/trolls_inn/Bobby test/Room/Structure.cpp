#include "Structure.h"

void Tile::setAdjacent(Tile * tile, Direction dir)
{
	this->adjacent[dir] = tile;
	
}

Tile * Tile::getAdjacent(Direction dir)
{
	return this->adjacent[dir];
}
