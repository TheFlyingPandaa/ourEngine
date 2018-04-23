#include "Kitchen.h"


Kitchen::Kitchen(int posX, int posY, int sizeX, int sizeY, Mesh * m) : Room(posX, posY, sizeX, sizeY, m)
{

}
Kitchen::Kitchen(int posX, int posY, int sizeX, int sizeY, std::vector<Tile*> tiles) : Room(posX, posY, sizeX, sizeY, tiles)
{
}
Kitchen::~Kitchen()
{

}


void Kitchen::Draw()
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
std::string Kitchen::toString() const
{
	return "K";
}
