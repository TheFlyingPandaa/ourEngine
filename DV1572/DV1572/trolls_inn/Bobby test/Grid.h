#pragma once
#include <iostream>
#include <vector>
#include "Room.h"
#include "Kitchen.h"
#include "Bedroom.h"


///<summary>
///	This a base grid that rooms can be place upon
///</summary>
class Grid
{
public:
	enum WallDirection {
		up,
		down,
		left,
		right
	};

private:
	Tile**				m_tiles;
	std::vector<Room*>	m_rooms;
	bool				m_walls[4] = { false };

	

	int m_posX, m_posY;
	int m_sizeX, m_sizeY;


	bool	_intersect(Room * room, bool close = false, Room ** otherRef = nullptr, bool getRef = false);
	
public:
	Grid(int posX = 0, int posY = 0, int sizeX = 32, int sizeY = 32);
	~Grid();

	void	DrawString();
	void	AddRoom(Room* room, bool force = false);
	Tile**	getGrid() const;
	bool	isWall(WallDirection dir) const;

};

