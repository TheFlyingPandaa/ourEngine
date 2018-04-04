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
	/*for (size_t x = 0; x < down.size(); x++)
	{
		down[x]->Draw();
		down[x]->CheckPick();
		up[x]->Draw();
	}
	for (size_t y = 0; y < left.size(); y++)
	{
		left[y]->Draw();
		right[y]->Draw();
	}*/
}
std::string Kitchen::toString() const
{
	return "K";
}
