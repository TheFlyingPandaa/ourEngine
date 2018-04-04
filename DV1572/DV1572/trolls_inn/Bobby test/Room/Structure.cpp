#include "Structure.h"

void Tile::setAdjacent(Tile * tile, WallDirection dir)
{
	this->adjacent[dir] = tile;
	
}

Tile * Tile::getAdjacent(WallDirection dir)
{
	return this->adjacent[dir];
}
