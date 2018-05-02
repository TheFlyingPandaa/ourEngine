#pragma once
#include <vector>
#include "Room.h"
#include "Structure.h"
#include "../../Furniture/Furniture.h"



const unsigned short ROOM_TYPE_SIZE = 4;


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

	Mesh*				m_doorMesh;
	Mesh*				m_wallMesh;
	Mesh*				m_tileMesh[ROOM_TYPE_SIZE];

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
	DoorPassage			getDoorPassage(int index1, int index2) const;

	std::vector<int>	roomTraversal(Tile* roomTile1, Tile * roomTile2);

	Room*				getRoomAt(int index);
	Room*				getRoomAtPos(XMINT2 pos);
	std::vector<Room*>	getAllTheRooms() const;

	DirectX::XMFLOAT3	getClosestRoom(XMFLOAT2 position, RoomType type);

	Direction			getDirection(Tile*, Tile*);
	XMINT2				getDirection2i(Tile*, Tile*);
	Direction			getDirection(Room*, Room*);

	std::vector<Furniture> getNoneBusyFurnitureInRoom(DirectX::XMINT2 pos);
	std::vector<Furniture> getNoneBusyFurnitureInRoom(DirectX::XMFLOAT2 pos);
	std::vector<Furniture> getNoneBusyFurnitureInRoom(DirectX::XMFLOAT3 pos);

	bool getIsBuildingDoor();
	void setIsBuildingDoor(bool tje);
};

