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
public:
	
private:
	std::vector<Room*>	m_rooms;
	Room*				m_entrance;
	std::vector<Wall*>	m_walls;
	Mesh*				m_doorMesh;
	Mesh*				m_wall;
	Mesh*				m_tileMesh[ROOM_TYPE_SIZE];

	std::vector<std::vector<int>> m_roomConnections;
	std::vector<int> m_tempPath;
	
	bool				_checkLegal(Room * room);
	void				_makeRoomConnection(int source, int destination);
	void				_dijkstra(int src, int dst);
	void				_getSolution(int dist[], int parent[], int src, int dst);
	void				_traversalPath(int parent[], int j, int src, int dst);

	void				_printRoomConnections() const;

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
	void				CreateMainDoor(Tile * tile1, Tile * tile2);

	std::vector<int>	roomTraversal(Tile* roomTile1, Tile * roomTile2);

	Room*				getMainRoom() const;
	XMINT2				getMainDoorPosEnter() const;
	XMINT2				getMainDoorPosLeave() const;

	XMINT2				getRoomEnterPos(Room* startRoom, int roomDstIndex);
	XMINT2				getRoomLeavePos(Room* startRoom, int roomDstIndex);

	Room*				getRoomAt(int index);

	Direction			getDirection(Tile*, Tile*);
	XMINT2				getDirection2i(Tile*, Tile*);
	Direction			getDirection(Room*, Room*);
};

