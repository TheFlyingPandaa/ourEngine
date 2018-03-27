#pragma once
#include <iostream>
#include <vector>
#include "Room.h"
#include "Kitchen.h"
#include "Bedroom.h"


class Grid
{
private:
	Tile**	m_tiles;

	std::vector<Room*> m_rooms;

public:
	Grid();
	~Grid();

	void DrawString();
	//void AddRoom(Room* room);

	void AddRoom(Room* room, bool force = false);

	Tile** getGrid() const;
};

