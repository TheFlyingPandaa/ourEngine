#include "Grid.h"



Grid::Grid()
{
	this->m_tiles = new Tile*[25];
	for (int i = 0; i < 25; i++)
	{
		this->m_tiles[i] = new Tile[25];
		for (int j = 0; j < 25; j++)
		{
			this->m_tiles[i][j].m_posX = i;
			this->m_tiles[i][j].m_posY = j;
		}
	}
}


Grid::~Grid()
{
}

void Grid::drawString()
{
	for (int y = 0; y < 25; y++)
	{
		for (int x = 0; x < 25; x++)
		{
			bool room = false;
			for (int i = 0; i < rooms.size() && !room; i++)
			{
				if (rooms[i]->inside(x, y)) {
					std::cout << rooms[i]->toString() << " ";
					room = true;
				}
			}
			if (!room)
				 std::cout << "0" << " ";
		}
		std::cout << std::endl;
	}
}

void Grid::addRoom(Room * room)
{
	rooms.push_back(room);
}

Tile ** Grid::getGrid() const
{
	return m_tiles;
}
