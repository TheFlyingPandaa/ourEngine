#include "Reception.h"



Reception::Reception(int posX, int posY, int sizeX, int sizeY, Mesh * m) : Room(posX, posY, sizeX, sizeY, m)
{
}

Reception::Reception(int posX, int posY, int sizeX, int sizeY, std::vector<Tile*> tiles) : Room(posX, posY, sizeX, sizeY, tiles)
{
}


Reception::~Reception()
{

}

void Reception::Draw()
{
	m_wholeFloor.Draw();
	
	for (auto& fur : m_roomObjects)
		fur.Draw();

	for (auto& tile : m_roomTiles)
	{
		if (tile->getQuad().getColor().x != 1.0f)
			tile->getQuad().Draw();
	}

	for (auto& wall : m_allWalls)
	{
		wall->Draw();
	}
}

std::string Reception::toString() const
{
	return std::string();
}
