#include "Grid.h"
#include <math.h>


bool Grid::_intersect(Room * room, bool close, Room ** otherRef, bool getRef)
{
	bool intersec = false;
	for (int i = 0; i < m_rooms.size() && !intersec; i++)
	{
		if (!close) {
			intersec =	(std::abs(room->getX() - m_rooms[i]->getX()) * 2) < (room->getSizeX() + m_rooms[i]->getSizeX()) &&
						(std::abs(room->getY() - m_rooms[i]->getY()) * 2) < (room->getSizeY() + m_rooms[i]->getSizeY());

			if (intersec && getRef)
				*otherRef = m_rooms[i];
		}
		else {
			intersec =	(std::abs(room->getX() - m_rooms[i]->getX()) * 2) <= (room->getSizeX() + m_rooms[i]->getSizeX()) &&
						(std::abs(room->getY() - m_rooms[i]->getY()) * 2) <= (room->getSizeY() + m_rooms[i]->getSizeY());

			if (intersec && getRef)
				*otherRef = m_rooms[i];
		}
	}
	return intersec;
}

Grid::Grid(int posX, int posY, int sizeX, int sizeY)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;

	this->m_tiles = new Tile*[sizeX];
	for (int i = 0; i < sizeX; i++)
	{
		this->m_tiles[i] = new Tile[sizeY];
		for (int j = 0; j < sizeY; j++)
		{
			this->m_tiles[i][j].m_posX = i + posX;
			this->m_tiles[i][j].m_posY = j + posY;
			this->m_tiles[i][j].quad.setScale(2.0f, 2.0f, 2.0f);
			this->m_tiles[i][j].quad.setPos(i + posX, 0, j + posY);
		}
	}
}


Grid::~Grid()
{
	for (int i = 0; i < m_sizeX; i++)
	{
		delete[] m_tiles[i];
	}
	delete[] m_tiles;

	for (int i = 0; i < m_rooms.size(); i++)
	{
		delete m_rooms[i];
	}
	m_rooms.clear();
}

void Grid::DrawString()
{
	for (int y = 0; y < m_sizeY; y++)
	{
		for (int x = 0; x < m_sizeX; x++)
		{
			bool room = false;
			for (int i = 0; i < m_rooms.size() && !room; i++)
			{
				if (m_rooms[i]->Inside(x, y)) {
					std::cout << m_rooms[i]->toString() << " ";
					room = true;
				}
			}
			if (!room)
				 std::cout << "0" << " ";
		}
		std::cout << std::endl;
	}
}

void Grid::AddRoom(Room * room, bool force)
{
	/*Room * f_ref = nullptr;
	if (!this->_intersect(room) && this->_intersect(room, true, &f_ref, true) || force) {
		if (force)
			m_rooms.push_back(room);
		else {

			if (room->getX() >= f_ref->getX() && room->getX() <= f_ref->getX() + f_ref->getSizeX()) {
				m_rooms.push_back(room);
			}
			else if (room->getX() + room->getSizeX() > f_ref->getX() && room->getX() + room->getSizeX() < f_ref->getX() + f_ref->getSizeX()) {
				m_rooms.push_back(room);
			}
		}
	}*/
	m_rooms.push_back(room);
}

Tile ** Grid::getGrid() const
{
	return m_tiles;
}

void Grid::Draw()
{
	for (int i = 0; i < m_sizeX; i++)
	{
		for (int j = 0; j < m_sizeY; j++)
		{
			m_tiles[i][j].quad.Draw();
		}
	}
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		m_rooms[i]->Draw();
	}
}

