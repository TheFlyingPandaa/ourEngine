#include "Reception.h"



Reception::Reception(int posX, int posY, int sizeX, int sizeY, Mesh * m) : Room(posX, posY, sizeX, sizeY, m)
{
}

Reception::Reception(int posX, int posY, int sizeX, int sizeY, std::vector<std::vector<Tile*>> tiles) : Room(posX, posY, sizeX, sizeY, tiles)
{
}


Reception::~Reception()
{
}

void Reception::Draw()
{
}

std::string Reception::toString() const
{
	return std::string();
}
