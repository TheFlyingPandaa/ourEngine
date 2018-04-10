#pragma once
#include <vector>
#include <iostream>
#include "Room.h"
#include "Structure.h"
#include "Kitchen.h"


const unsigned short ROOM_TYPE_SIZE = 2;
enum RoomType {
	kitchen,
	bedroom
};

const float WALLOFFSET = 0.5f;

class RoomCtrl
{
private:
	std::vector<Room*>	m_rooms;
	std::vector<Wall*>	m_walls;
	

	Mesh*				m_wall;
	Mesh*				m_tileMesh[ROOM_TYPE_SIZE];
	
public:
	RoomCtrl();
	~RoomCtrl();

	
	void				setTileMesh(Mesh* tileMesh, RoomType roomType);
	bool				_intersect(Room * room);
	bool				_intersect(DirectX::XMINT2 pos, DirectX::XMINT2 size);
	bool				isPlaceable(DirectX::XMINT2 pos, DirectX::XMINT2 size);
	void				setMesh(Mesh * mesh);

	void				AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<std::vector<Tile*>> tiles, bool force = false);
	void				Update(Camera * cam);
	void				Draw();

	//This is a expensiv function many many many for loops
	//Be conservative when calling
	void				CreateWalls();
	void				CreateDoors();
};

