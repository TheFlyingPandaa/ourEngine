#include "Room.h"

Room::Room(int xPos, int yPos, int width, int height)
{
	m_corners = new Point[4];

	m_corners[0].x = xPos;
	m_corners[0].y = yPos;

	m_corners[1].x = xPos + (width-1);
	m_corners[1].y = yPos;

	m_corners[2].x = xPos + (width-1);
	m_corners[2].y = yPos + (height-1);

	m_corners[3].x = xPos;
	m_corners[3].y = yPos + (width-1);

	m_tiles = new Tile[width * height];

	m_width = width;
	m_height = height;

	for (int  y = 0; y < m_height; y++)
	{
		for (int  x = 0; x < m_width; x++)
		{
			m_tiles[x + y * m_width].type = MODULE;
			m_tiles[x + y * m_width].x = x;
			m_tiles[x + y * m_width].y = y;
			m_tiles[x + y * m_width].myRoom = this;
		}
	}
}

Point Room::getPosition() const
{
	return m_corners[0];
}

int Room::getWidth() const
{
	return m_width;
}

int Room::getHeight() const
{
	return m_height;
}

Room::~Room()
{
	delete[] m_tiles;
	delete[] m_corners;
}

Tile * Room::getTiles() const
{
	return m_tiles;
}

Point* Room::getCorners() const
{
	return m_corners;
}
