#include "Structure.h"

void Tile::setAdjacent(Tile * tile, Direction dir)
{
	this->adjacent[dir] = tile;
	
}

Tile * Tile::getAdjacent(Direction dir)
{
	return this->adjacent[dir];
}

void Tile::setMesh(Mesh * mesh)
{
	this->quad.setMesh(mesh);
	
	//quad.setPos(static_cast<float>(m_posX), 0.0f, static_cast<float>(m_posY));
	//quad.setRotation(90.0f, 0.0f, 0.0f);
}
