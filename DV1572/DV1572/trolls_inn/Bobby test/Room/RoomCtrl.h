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
	struct RoomConnection
	{
		Room* otherRoom; // if nullptr, then the connection is to the outside
		XMINT2 connectingDoor;
		XMINT2 direction;

		bool operator==(const RoomConnection& other)
		{
			return *otherRoom == *other.otherRoom;
		}
		bool operator==(const Room& other)
		{
			return *otherRoom == other;
		}
		bool operator==(const Room* other)
		{
			if (other == nullptr)
			{
				if (otherRoom == nullptr)
					return true;
				return false;
			}
			if (otherRoom == nullptr)
				return false;

			return *otherRoom == *other;
		}
	};
private:
	std::vector<Room*>	m_rooms;

	Room*				m_entrance;
	Mesh*				m_doorMesh;
	Mesh*				m_wallMesh;
	Mesh*				m_tileMesh[ROOM_TYPE_SIZE];

	std::vector<std::vector<int>> m_roomConnectionMap;

	std::vector<int>	m_tempPath;
	
	bool				_checkLegal(Room * room);
	void				_makeRoomConnection(int source, int destination);
	void				_dijkstra(int src, int dst);
	void				_getSolution(int dist[], int parent[], int src, int dst);
	void				_traversalPath(int parent[], int j, int src, int dst);

	void				_printRoomConnections() const;

public:
	RoomCtrl();
	~RoomCtrl();

	void				addRoomObject(DirectX::XMINT2 pos, Mesh * mesh);
	
	void				setTileMesh(Mesh* tileMesh, RoomType roomType);
	bool				_intersect(Room * room);
	int					_intersect(DirectX::XMINT2 pos, DirectX::XMINT2 size);
	bool				isPlaceable(DirectX::XMINT2 pos, DirectX::XMINT2 size);
	void				setMesh(Mesh * mesh);

	void				AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<Tile*> tiles, bool force = false);
	bool				RemoveRoom(DirectX::XMINT2 pos, std::vector<Tile*>& backtiles, DirectX::XMINT2& delPos, DirectX::XMINT2& delSize);
	void				PickRoomTiles();
	void				Update(Camera * cam);
	void				Draw();

	//This is a expensiv function many many many for loops
	//Be conservative when calling
	void				CreateWalls(Room* currentRoom);
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

