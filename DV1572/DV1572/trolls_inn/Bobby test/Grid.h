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
private:
	Tile**				m_tiles;
	std::vector<Room*>	m_rooms;

	bool	_intersect(Room * room) const;
	int		_within(Tile * tiles, Room * a, Room * b);
	
public:
	Grid();
	~Grid();

	void	DrawString();
	void	AddRoom(Room* room, bool force = false);
	Tile**	getGrid() const;
};

