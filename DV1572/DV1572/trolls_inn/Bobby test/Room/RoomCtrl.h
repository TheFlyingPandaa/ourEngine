#pragma once
#include <vector>
#include "Room.h"
#include "Structure.h"
#include "Kitchen.h"
#include "Bedroom.h"
#include "Reception.h"
#include "../../Furniture/Furniture.h"



const unsigned short ROOM_TYPE_SIZE = 2;
enum RoomType {
	kitchen,
	bedroom,
	reception,
	hallway,
	UNDEFINED
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
	struct DoorPassage
	{
		XMINT2 one;
		XMINT2 two;
	};
private:

	std::vector<Room*>	m_rooms;

	Room*				m_entrance;
	std::vector<DoorPassage> m_outsideDoorPos;
	Mesh*				m_doorMesh;
	Mesh*				m_wallMesh;
	Mesh*				m_tileMesh[ROOM_TYPE_SIZE];

	std::vector<std::vector<int>> m_roomConnectionMap;

	std::vector<int>	m_tempPath;

	bool				m_buildingDoors;
	
	bool				_checkLegal(Room * room);
	bool				_checkRoomType(Room* room, RoomType type);
	void				_makeRoomConnection(int source, int destination);
	void				_dijkstra(int src, int dst);
	void				_getSolution(int dist[], int parent[], int src, int dst);
	void				_traversalPath(int parent[], int j, int src, int dst);

	void				_printRoomConnections() const;

public:
	RoomCtrl();
	~RoomCtrl();

	void				AddRoomObject(DirectX::XMFLOAT3 pos, Mesh * mesh);
	void				AddRoomObject(Furniture furniture);
	
	void				setTileMesh(Mesh* tileMesh, RoomType roomType);
	int					_intersect(DirectX::XMINT2 pos, DirectX::XMINT2 size = { 1, 1 });
	bool				isPlaceableObject(DirectX::XMINT2 pos, int size);

	void				AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<Tile*> tiles, bool force = false);
	bool				RemoveRoom(DirectX::XMINT2 pos, std::vector<Tile*>& backtiles, DirectX::XMINT2& delPos, DirectX::XMINT2& delSize);
	bool				CheckAndMarkTilesObject(DirectX::XMINT2 start, int size, int angle = 0);
	void				PickRoomTiles();
	void				PickWalls();
	void				Update(Camera * cam);
	void				Draw();

	int					getRoomConnections(int index) const;
	
	//This is a expensiv function many many many for loops
	//Be conservative when calling
	void				CreateWalls(Room* currentRoom);
	void				setDoorMesh(Mesh * mesh);
	void				CreateDoor(XMFLOAT3 wallPosition);
	
	DoorPassage			getClosestEntranceDoor(XMINT2 startPosition) const;

	std::vector<int>	roomTraversal(Tile* roomTile1, Tile * roomTile2);

	Room*				getRoomAt(int index);

	DirectX::XMFLOAT3	getClosestRoom(XMFLOAT2 position, RoomType type);

	Direction			getDirection(Tile*, Tile*);
	XMINT2				getDirection2i(Tile*, Tile*);
	Direction			getDirection(Room*, Room*);

	bool getIsBuildingDoor();
	void setIsBuildingDoor(bool tje);
};

