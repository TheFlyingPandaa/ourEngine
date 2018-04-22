#include "Kitchen.h"


Kitchen::Kitchen(int posX, int posY, int sizeX, int sizeY, Mesh * m) : Room(posX, posY, sizeX, sizeY, m)
{

}
Kitchen::Kitchen(int posX, int posY, int sizeX, int sizeY, std::vector<std::vector<Tile*>> tiles) : Room(posX, posY, sizeX, sizeY, tiles)
{
}
Kitchen::~Kitchen()
{

}


void Kitchen::Draw()
{
	for (auto& roomObj : m_roomObjects)
		roomObj.Draw();

	for (auto& vec : m_roomTiles)
		for (auto& floor : vec)
			floor->getQuad().Draw();

	for (auto& wall : m_allWalls)
		wall->Draw();


}
std::string Kitchen::toString() const
{
	return "K";
}
