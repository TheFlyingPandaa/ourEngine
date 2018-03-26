#pragma once
#include <string>

class Room;

struct Tile {
	Tile(int x = 0, int y = 0, int sizeX = 0, int sizeY = 0, Room * room = nullptr)
	{
		m_posX = x;
		m_posY = y;
		m_sizeX = sizeX;
		m_sizeY = sizeY;
		m_room = room;
	}
	int		m_posY;
	int		m_posX;
	int		m_sizeX;
	int		m_sizeY;
	Room*	m_room;
};

class Room
{
protected:
	int		m_posX, m_posY;
	int		m_sizeX, m_sizeY;
	Tile**	m_tiles;

public:
	Room(int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0){
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
				this->m_tiles[i][j].m_room = this;
			}
		}
	}
	virtual ~Room() {};

	virtual Tile**		getTiles() const { return m_tiles; }
	virtual int			getX() const { return m_posX; }
	virtual int			getY() const { return m_posY; }
	virtual int			getSizeX() const { return m_sizeX; }
	virtual int			getSizeY() const { return m_sizeY; }

	virtual bool		Inside(int x, int y) {
		return x >= m_posX && x < m_posX + m_sizeX && 
				y >= m_posY && y < m_posY + m_sizeY;
	}
	virtual std::string	toString() const = 0;

};

