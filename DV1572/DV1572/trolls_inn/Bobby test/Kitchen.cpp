#include "Kitchen.h"


Kitchen::Kitchen(int posX, int posY, int sizeX, int sizeY, Mesh * m) : Room(posX, posY, sizeX, sizeY, m)
{

}

Kitchen::~Kitchen()
{
	for (int i = 0; i < m_sizeX; i++)
	{
		//delete[] m_tiles[i];
	}
	//delete[] m_tiles;
}

void Kitchen::Draw()
{
	for (size_t x = 0; x < down.size(); x++)
	{
		down[x]->Draw();
		down[x]->CheckPick();
		up[x]->Draw();
	}
	for (size_t y = 0; y < left.size(); y++)
	{
		left[y]->Draw();
		right[y]->Draw();
	}
}

std::string Kitchen::toString() const
{
	return "K";
}
