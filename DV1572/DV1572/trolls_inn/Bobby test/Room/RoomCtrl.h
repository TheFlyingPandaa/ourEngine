#pragma once
#include <vector>
#include "Room.h"
#include "Structure.h"
#include "../../Furniture/Furniture.h"
#include <tuple>


const unsigned short ROOM_TYPE_SIZE = 5;


const float WALLOFFSET = 0.5f;

class RoomCtrl
{
public:
	struct DoorPassage
	{
		int roomIndexes[2];
		XMINT2 one;
		XMINT2 two;
		bool operator==(const DoorPassage& other)const
		{
			return one.x == other.one.x && one.y == other.one.y;
		}
	};
	
private:

	std::vector<Room*>	m_rooms;

	std::vector<DoorPassage> m_outsideDoorPos;
	std::vector<DoorPassage> m_roomToRoom;
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
	void				_removeFromRoomToRoom(int room1, int room2);

public:
	RoomCtrl();
	~RoomCtrl();

	void				AddRoomObject(Furniture * furniture);
	bool				RemoveRoomObject(Furniture* fur);
	
	int					_intersect(DirectX::XMINT2 pos, DirectX::XMINT2 size = { 1, 1 });

	void				AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<Tile*> tiles, bool force = false);
	std::tuple<bool,int> RemoveRoomTuple(DirectX::XMINT2 pos, std::vector<Tile*>& backtiles, DirectX::XMINT2& delPos, DirectX::XMINT2& delSize);

	bool				CheckAndMarkTilesObject(DirectX::XMINT2 start, int size, int angle = 0);
	bool				MarkAllTilesRedObject(DirectX::XMINT2 start, int size, int angle = 0);

	void				PickRoomTiles();
	void				PickWalls();
	void				PickAllFurnitures();

	void				Update(Camera * cam);
	void				Draw();

	int					getRoomConnections(int index) const;
	int					HasEntranceDoor(int index) const;
	
	void				CreateWalls(Room* currentRoom);
	void				CreateDoor(XMFLOAT3 wallPosition, float rot = 0.0f);
	bool				RemoveDoor(XMFLOAT3 wallPosition);
	
	std::vector<DoorPassage>&	getAllEntranceDoors();
	DoorPassage			getClosestEntranceDoor(XMINT2 startPosition) const;
	DoorPassage			getDoorPassage(int index1, int index2) const;

	std::vector<int>	roomTraversal(Tile* roomTile1, Tile * roomTile2);

	Room*				getRoomAt(int index);
	Room*				getRoomAtPos(XMINT2 pos);
	std::vector<Room*>	getAllTheRooms() const;

	Furniture*			getFurnitureAtPos(XMINT2 pos);

	DirectX::XMFLOAT3	getClosestRoom(XMFLOAT2 position, RoomType type);

	Direction			getDirection(Tile*, Tile*);
	XMINT2				getDirection2i(Tile*, Tile*);
	Direction			getDirection(Room*, Room*);

	std::vector<Furniture*> getNoneBusyFurnitureInRoom(DirectX::XMINT2 pos);
	std::vector<Furniture*> getNoneBusyFurnitureInRoom(DirectX::XMFLOAT2 pos);
	std::vector<Furniture*> getNoneBusyFurnitureInRoom(DirectX::XMFLOAT3 pos);

	bool getIsBuildingDoor();
	void setIsBuildingDoor(bool tje);
};

