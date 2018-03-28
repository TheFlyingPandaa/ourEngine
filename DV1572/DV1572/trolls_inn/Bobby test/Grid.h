#pragma once
#include <iostream>
#include <vector>
#include "Room.h"
#include "Kitchen.h"
#include "Bedroom.h"

class Grid
{
private:

	int					m_posX, m_posY;
	int					m_sizeX, m_sizeY;
	bool				m_walls[4] = { false };
	Tile**				m_tiles;
	std::vector<Room*>	m_rooms;


	bool	_intersect(Room * room, bool close = false, Room ** otherRef = nullptr, bool getRef = false);
	
public:
	Grid(int posX = 0, int posY = 0, int sizeX = 8, int sizeY = 8);
	~Grid();

	Tile**	getGrid() const;
	void	AddRoom(Room* room, bool force = false);
	void	Draw();
	void	DrawString();

	void	CreateWalls(Mesh * mesh);
};

