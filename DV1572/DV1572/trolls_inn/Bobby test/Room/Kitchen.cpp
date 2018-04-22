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
	for (auto& tile : m_roomTiles)
		tile->getQuad().Draw();
	for (auto& wall : m_allWalls)
		wall->Draw();

}
std::string Kitchen::toString() const
{
	return "K";
}
