#pragma once
#include <vector>
#include <iostream>
#include "Room.h"
#include "Structure.h"
#include "Kitchen.h"
#include "Bedroom.h"
#include "Reception.h"


const unsigned short ROOM_TYPE_SIZE = 2;
enum RoomType {
	kitchen,
	bedroom,
	reception
};

const float WALLOFFSET = 0.5f;

class RoomCtrl
{
private:
	std::vector<Room*>	m_rooms;
	std::vector<Wall*>	m_walls;
	Mesh*				m_doorMesh;
	Mesh*				m_wall;
	Mesh*				m_tileMesh[ROOM_TYPE_SIZE];
	
	bool				_checkLegal(Room * room);
	

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
	void				CreateDoors(Room * room);
	DirectX::XMINT2		CreateMainDoor(Room * room);
	void				setDoorMesh(Mesh * mesh);
	void				CreateDoor(Tile * tile1, Tile * tile2);
	DirectX::XMINT2		CreateMainDoor(Tile * tile1, Tile * tile2);


	Direction			getDirection(Tile*, Tile*);
	Direction			getDirection(Room*, Room*);
};

