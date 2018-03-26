#include "Kitchen.h"


Kitchen::Kitchen(int posX, int posY, int sizeX, int sizeY) : Room(posX, posY, sizeX, sizeY)
{

}

Kitchen::~Kitchen()
{
	for (int i = 0; i < m_sizeX; i++)
	{
		delete[] m_tiles[i];
	}
	delete[] m_tiles;
}

std::string Kitchen::toString() const
{
	return "K";
}
