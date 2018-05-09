#include "RoomCtrl.h"
#include "../../Furniture/Table.h"
#include <iostream>

void RoomCtrl::setTileMesh(Mesh * mesh, RoomType roomType)
{
	m_tileMesh[roomType] = mesh;
}

int RoomCtrl::_intersect(DirectX::XMINT2 pos, DirectX::XMINT2 size)
{
	for (int i = 0; i < m_rooms.size(); i++)
	{
		if (pos.x < m_rooms[i]->getX() + m_rooms[i]->getSize().x &&
			pos.x + size.x > m_rooms[i]->getX() &&
			pos.y < m_rooms[i]->getY() + m_rooms[i]->getSize().y &&
			pos.y + size.y > m_rooms[i]->getY())
			return i;

	}
	return -1;
}

bool RoomCtrl::_checkLegal(Room * room)
{

	return dynamic_cast<Room*>(room) == nullptr;
}

bool RoomCtrl::_checkRoomType(Room * room, RoomType type)
{
	return room->getRoomType() == type;
}

void RoomCtrl::_makeRoomConnection(int source, int destination)
{

	m_roomConnectionMap[source][destination] = 1;
	m_roomConnectionMap[destination][source] = 1;

}

void RoomCtrl::_dijkstra(int src, int dst)
{
	auto minDistance = [&](int dist[], bool sptSet[]) -> int
	{
		int min = INT_MAX, min_index;

		for (int v = 0; v < m_roomConnectionMap[0].size(); v++)
			if (sptSet[v] == false && dist[v] <= min)
				min = dist[v], min_index = v;

		return min_index;
	};

	if (m_roomConnectionMap[src].size() == 1) return;

	int* dist = new int[m_roomConnectionMap[src].size()];  // The output array. dist[i] will hold
						  // the shortest distance from src to i

						  // sptSet[i] will true if vertex i is included / in shortest
						  // path tree or shortest distance from src to i is finalized
	bool* sptSet = new bool[m_roomConnectionMap[src].size()];

	// Parent array to store shortest path tree
	int* parent = new int[m_roomConnectionMap[src].size()];

	// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < m_roomConnectionMap[src].size(); i++)
	{
		parent[src] = -1;
		dist[i] = INT_MAX;
		sptSet[i] = false;
	}

	// Distance of source vertex from itself is always 0
	dist[src] = 0;
	// Find shortest path for all vertices
	for (int count = 0; count < m_roomConnectionMap[src].size() - 1; count++)
	{
		// Pick the minimum distance vertex from the set of
		// vertices not yet processed. u is always equal to src
		// in first iteration.
		int u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update dist value of the adjacent vertices of the
		// picked vertex.
		for (int v = 0; v < m_roomConnectionMap[src].size(); v++)

			// Update dist[v] only if is not in sptSet, there is
			// an edge from u to v, and total weight of path from
			// src to v through u is smaller than current value of
			// dist[v]
			if (!sptSet[v] && m_roomConnectionMap[u][v] &&
				dist[u] + m_roomConnectionMap[u][v] < dist[v])
			{
				parent[v] = u;
				dist[v] = dist[u] + m_roomConnectionMap[u][v];
			}

	}


	// print the constructed distance array
	_getSolution(dist, parent, src, dst);
	delete[] dist;
	delete[] parent;
	delete[] sptSet;
}

void RoomCtrl::_getSolution(int dist[], int parent[], int src, int dst)
{
	m_tempPath.push_back(src);
	if (dst != src)
		_traversalPath(parent, dst, src, dst);
}

void RoomCtrl::AddRoomObject(Furniture * furniture)
{
	XMINT2 furPos = { static_cast<int>(furniture->getPosition().x), static_cast<int>(furniture->getPosition().z) };
	int index = _intersect(furPos, XMINT2(1, 1));
	Room* cr = m_rooms[index];
	auto m_tiles = cr->getTiles();
	auto _index = [&](int x, int y) ->int
	{
		return static_cast<int>((x - cr->getPosition().x) + (y - cr->getPosition().z) * cr->getSize().x);
	};
	
	if (furniture->getRotation() == 0 || furniture->getRotation() == 180)
	{
		for (int i = 0; i < furniture->getGridSize(); i++)
		{
			if (furniture->getRotation() == 0)
			{
				//m_tiles[_index(furniture.getPosition().x,furniture.getPosition().z + i)]->setIsWalkeble(false);
				m_tiles[_index(furniture->getPosition().x, furniture->getPosition().z + i)]->setHasObject(true);
			}
			else
			{
				//m_tiles[furniture.getPosition().x][furniture.getPosition().z - i]->setIsWalkeble(false);
				m_tiles[_index(furniture->getPosition().x, furniture->getPosition().z - i)]->setHasObject(true);
			}
		}
	}
	if (furniture->getRotation() == 90 || furniture->getRotation() == 270)
	{
		for (int i = 0; i < furniture->getGridSize(); i++)
		{
			if (furniture->getRotation() == 90)
			{
				//m_tiles[_index(furniture.getPosition().x + i, furniture.getPosition().z)]->setIsWalkeble(false);
				m_tiles[_index(furniture->getPosition().x + i, furniture->getPosition().z)]->setHasObject(true);
			}
			else
			{
				//m_tiles[furniture.getPosition().x - i][furniture.getPosition().z]->setIsWalkeble(false);
				m_tiles[_index(furniture->getPosition().x + i, furniture->getPosition().z)]->setHasObject(true);
			}
		}
	}

	//m_tiles[_index(furniture.getPosition().x, furniture.getPosition().z)]->setIsWalkeble(false);
	m_tiles[_index(furniture->getPosition().x, furniture->getPosition().z)]->setHasObject(true);

	//furniture.setLightIndex(cr->getRoomIndex());
	cr->AddRoomObject(furniture);
}

//void RoomCtrl::RemoveRoomObject(DirectX::XMINT2 pos)
//{
//	int index = _intersect(pos, XMINT2(1, 1));
//	if (index != -1)
//	{
//		std::vector<Furniture*> temp = m_rooms[index]->getAllRoomFurnitures();
//		//delete temp.at(0);
//		
//		
//	}
//}

bool RoomCtrl::RemoveRoomObject(Furniture * fur)
{
	int index = _intersect(XMINT2(fur->getPosition().x, fur->getPosition().z), XMINT2(1, 1));
	if (index != -1)
	{
		return m_rooms[index]->RemoveThisFurniture(fur);

		//delete temp.at(0);


	}
	return false;
}

void RoomCtrl::_traversalPath(int parent[], int j, int src, int dst)
{
	if (parent[j] == -1)
		return;

	_traversalPath(parent, parent[j], src, dst);
	
	m_tempPath.push_back(j);
}

#include <iostream>
void RoomCtrl::_printRoomConnections() const
{
	
	std::cout << " ";
	for (int i = 0; i < m_roomConnectionMap.size(); i++)
	{
		std::cout << " " << i;
	}
	std::endl(std::cout);
	for (int i = 0; i < m_roomConnectionMap.size(); i++)
	{
		std::cout << i << " ";
		for (int j = 0; j < m_roomConnectionMap[i].size(); j++)
		{
			std::cout << m_roomConnectionMap[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "Room " << m_rooms.size() - 1 << "->" << 0 << ": ";
	for (int i = 0; i < m_tempPath.size(); i++)
		std::cout << m_tempPath[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < m_roomConnectionMap.size(); i++)
	{
		for (int ii = 0; ii < m_roomConnectionMap[i].size(); ii++)
		{
			for (int j = 0; j < m_roomToRoom.size(); j++)
			{
				if (m_roomToRoom[j].roomIndexes[0] == i && m_roomToRoom[j].roomIndexes[1] == ii)
				{
					std::cout << "Room " << i << "-> " << ii << "\n";
					std::cout << "One door Pos: (" << m_roomToRoom[j].one.x << "," << m_roomToRoom[j].one.y << ")\n";
					std::cout << "Two door Pos: (" << m_roomToRoom[j].two.x << "," << m_roomToRoom[j].two.y << ")\n";
				}
			}

		}
	}


}

RoomCtrl::RoomCtrl()
{
	
	m_tileMesh[0] = new Mesh();
	m_tileMesh[0]->MakeRectangle();
	m_tileMesh[0]->setDiffuseTexture("trolls_inn/Resources/GenericTexture/KitchenTile.jpg");
	m_tileMesh[0]->setNormalTexture("trolls_inn/Resources/woodenfloor/NormalMap.png");

	m_tileMesh[1] = new Mesh();
	m_tileMesh[1]->MakeRectangle();
	m_tileMesh[1]->setDiffuseTexture("trolls_inn/Resources/GenericTexture/BedRoomTile.jpg");
	m_tileMesh[1]->setNormalTexture("trolls_inn/Resources/DefaultNormal.png");

	m_tileMesh[2] = new Mesh();
	m_tileMesh[2]->MakeRectangle();
	m_tileMesh[2]->setDiffuseTexture("trolls_inn/Resources/GenericTexture/ReceptionTile.jpg");
	m_tileMesh[2]->setNormalTexture("trolls_inn/Resources/DefaultNormal.png");

	m_tileMesh[3] = new Mesh();
	m_tileMesh[3]->MakeRectangle();
	m_tileMesh[3]->setDiffuseTexture("trolls_inn/Resources/GenericTexture/HallwayTile.jpg");
	m_tileMesh[3]->setNormalTexture("trolls_inn/Resources/DefaultNormal.png");

	m_tileMesh[4] = new Mesh();
	m_tileMesh[4]->MakeRectangle();
	m_tileMesh[4]->setDiffuseTexture("trolls_inn/Resources/GenericTexture/wood.png");
	m_tileMesh[4]->setNormalTexture("trolls_inn/Resources/woodenfloor/NormalMap.png");
	
	m_wallMesh = new Mesh();
	m_wallMesh->LoadModel("trolls_inn/Resources/wall3.obj");

	m_doorMesh = new Mesh();
	m_doorMesh->LoadModel("trolls_inn/Resources/door/Door.obj");

	m_buildingDoors = false;
}


RoomCtrl::~RoomCtrl()
{
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		delete m_rooms[i];
	}

	delete m_wallMesh;
	//delete m_tileMesh[0];
	delete m_doorMesh;
	for (auto& element : m_tileMesh)
	{
		delete element;
	}
	m_rooms.clear();
}

void RoomCtrl::AddRoomObject(DirectX::XMFLOAT3 pos, Mesh * mesh)
{
	//Table tempObj = Table(pos, mesh);
	
	//m_roomObjects.push_back(tempObj);
	
}


void RoomCtrl::AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<Tile*> tiles, bool force)
{
	Room * currentRoom = nullptr;

	switch (roomType)
	{
	case kitchen:
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(m_tileMesh[0]);
		break;
	case bedroom:
		//Duno just copied the 
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(m_tileMesh[1]);
		break;
	case reception:
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(m_tileMesh[2]);
		break;
	case hallway:
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(m_tileMesh[3]);
		break;
	case bar:
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(m_tileMesh[4]);
		break;
	}

	CreateWalls(currentRoom);
	currentRoom->ApplyIndexOnMesh();
	m_rooms.push_back(currentRoom);
	

	
	if (m_roomConnectionMap.size() < m_rooms.size())
	{
		m_roomConnectionMap.push_back(std::vector<int>());
		
		for (int j = 0; j < m_roomConnectionMap.size();)
		{
			if (m_roomConnectionMap[j].size() != m_roomConnectionMap.size())
				m_roomConnectionMap[j].push_back(0);
			else
				j++;
		}
	}

	int lastDoorIndex = static_cast<int>(m_rooms.size()) - 1;

	for (int i = 0; i < m_outsideDoorPos.size(); ++i)
	{
		int currentIndex = _intersect(m_outsideDoorPos[i].one);
		if (lastDoorIndex == currentIndex)
		{
			DoorPassage dp;
			dp.one = m_outsideDoorPos[i].one;
			dp.two = m_outsideDoorPos[i].two;
			dp.roomIndexes[0] = currentIndex;
			dp.roomIndexes[1] = m_outsideDoorPos[i].roomIndexes[1];

			DoorPassage dp2;
			dp2.one = dp.two;
			dp2.two = dp.one;
			dp2.roomIndexes[0] = dp.roomIndexes[1];
			dp2.roomIndexes[1] = dp.roomIndexes[0];

			m_roomToRoom.push_back(dp);
			m_roomToRoom.push_back(dp2);
			_makeRoomConnection(currentIndex, m_outsideDoorPos[i].roomIndexes[1]);
			m_outsideDoorPos.erase(m_outsideDoorPos.begin() + i);
			i = 0;

		}
	}


	_printRoomConnections();


	
}

// Recieves tile pos, it knows what room you picked.
// returns the room tiles and position and size
// sent these into grid->insertTiles();

 bool RoomCtrl::RemoveRoom(DirectX::XMINT2 pos, std::vector<Tile*>& backtiles, DirectX::XMINT2& delPos, DirectX::XMINT2& delSize)
{
	int index = _intersect(pos, XMINT2(1, 1));
	
	if (index != -1)
	{

		backtiles = m_rooms[index]->ReturnTiles();
		XMFLOAT3 _pos = m_rooms[index]->getPosition();
		delSize = m_rooms[index]->getSize();
		delPos = { static_cast<int>(_pos.x), static_cast<int>(_pos.z) };
		delete m_rooms[index];
		m_rooms.erase(m_rooms.begin() + index);
	}
	
	return index != -1;
}

 std::tuple<bool, int> RoomCtrl::RemoveRoomTuple(DirectX::XMINT2 pos, std::vector<Tile*>& backtiles, DirectX::XMINT2 & delPos, DirectX::XMINT2 & delSize)
 {
	 int index = _intersect(pos, XMINT2(1, 1));
	 int payBack = 0;
	 if (index != -1)
	 {
		 payBack = m_rooms[index]->getPriceOfAllObjects();
		 backtiles = m_rooms[index]->ReturnTiles();
		 XMFLOAT3 _pos = m_rooms[index]->getPosition();
		 delSize = m_rooms[index]->getSize();
		 delPos = { static_cast<int>(_pos.x), static_cast<int>(_pos.z) };
		 delete m_rooms[index];
		 m_rooms.erase(m_rooms.begin() + index);
		 payBack += (delSize.x * delSize.y) * 20;
	 } 
	 return { index != -1, payBack/2 }; //c++17 way of making a tuple
 }

 bool RoomCtrl::CheckAndMarkTilesObject(DirectX::XMINT2 start, int size, int angle)
 {
	 bool isFalse = false;
	 int index = _intersect(start, XMINT2(1, 1));
	 if (index == -1)
	 {
		 return false;
	 }
	 Room*  cr = m_rooms[index];
	 auto tiles = cr->getTiles();

	 auto _index = [&](int x, int y) ->int
	 {
		 return static_cast<int>((x - cr->getPosition().x) + (y - cr->getPosition().z) * cr->getSize().x);
	 };
	 
	if (angle == 0 || angle == 180)
	{
		for (int i = 0; i < size; i++)
		{
			int ii = _index(start.x, start.y + i);

			if (ii >= tiles.size()) {
				tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
				return false;
			}

			if (angle == 0)
			{
				Tile* t = tiles[ii];
				if (t && t->getHasObject() == false)
				{
					t->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else if(t)
				{
					t->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
			else
			{
				Tile* t = tiles[_index(start.x, start.y - i)];
				if (t && t->getHasObject() == false)
				{
					t->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else if(t)
				{
					t->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
		}
	}
	if (angle == 90 || angle == 270)
	{
		for (int i = 0; i < size; i++)
		{
			if (angle == 90)
			{
				if (tiles[_index(start.x + i, start.y)]->getHasObject() == false)
				{
					tiles[_index(start.x + i, start.y)]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else
				{
					tiles[_index(start.x + i, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
			else
			{
				if (tiles[_index(start.x - i, start.y)]->getHasObject() == false)
				{
					tiles[_index(start.x - i, start.y)]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else
				{
					tiles[_index(start.x - i, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
		}
	}

	if (tiles[_index(start.x, start.y)]->getHasObject() == true)
	{
		tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
		return false;
	}
	else
	{
		if (isFalse == true)
		{
			tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
			return false;
		}
		else
		{
			tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
			return true;
		}
	}
	return true;
 }

 bool RoomCtrl::MarkAllTilesRedObject(DirectX::XMINT2 start, int size, int angle)
 {
	 bool isFalse = false;
	 int index = _intersect(start, XMINT2(1, 1));
	 if (index == -1)
	 {
		 return false;
	 }
	 Room*  cr = m_rooms[index];
	 auto tiles = cr->getTiles();

	 auto _index = [&](int x, int y) ->int
	 {
		 return static_cast<int>((x - cr->getPosition().x) + (y - cr->getPosition().z) * cr->getSize().x);
	 };

	 if (angle == 0 || angle == 180)
	 {
		 for (int i = 0; i < size; i++)
		 {
			 int ii = _index(start.x, start.y + i);

			 if (ii >= tiles.size()) {
				 tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
				 return false;
			 }

			 if (angle == 0)
			 {
				 Tile* t = tiles[ii];
				 if (t && t->getHasObject() == false)
				 {
					 t->getQuad().setColor(XMFLOAT3(8.0f, 0.5f, 0.5f));
				 }
				 else if (t)
				 {
					 t->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
					 isFalse = false;
				 }
			 }
			 else
			 {
				 Tile* t = tiles[_index(start.x, start.y - i)];
				 if (t && t->getHasObject() == false)
				 {
					 t->getQuad().setColor(XMFLOAT3(8.0f, 0.5f, 0.5f));
				 }
				 else if (t)
				 {
					 t->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
					 isFalse = false;
				 }
			 }
		 }
	 }
	 if (angle == 90 || angle == 270)
	 {
		 for (int i = 0; i < size; i++)
		 {
			 if (angle == 90)
			 {
				 if (tiles[_index(start.x + i, start.y)]->getHasObject() == false)
				 {
					 tiles[_index(start.x + i, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
				 }
				 else
				 {
					 tiles[_index(start.x + i, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
					 isFalse = false;
				 }
			 }
			 else
			 {
				 if (tiles[_index(start.x - i, start.y)]->getHasObject() == false)
				 {
					 tiles[_index(start.x - i, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
				 }
				 else
				 {
					 tiles[_index(start.x - i, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
					 isFalse = false;
				 }
			 }
		 }
	 }

	 if (tiles[_index(start.x, start.y)]->getHasObject() == true)
	 {
		 tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
		 return false;
	 }
	 else
	 {
		 if (isFalse == true)
		 {
			 tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
			 return false;
		 }
		 else
		 {
			 tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
			 return false;
		 }
	 }
	 return false;
 }

void RoomCtrl::PickRoomTiles()
{
	for (auto& room : m_rooms)
		room->PickTiles();
}

void RoomCtrl::PickWalls()
{
	for (auto& room : m_rooms)
		room->PickWalls();
}

void RoomCtrl::PickAllFurnitures()
{
	for (auto& element : m_rooms)
	{
		element->PickFurnitures();
	}
}

void RoomCtrl::Update(Camera * cam)
{
	
	for (int i = 0; i < m_rooms.size(); i++)
	{
		m_rooms[i]->Update(cam);
		if (m_buildingDoors)
		{
			m_rooms[i]->setIsBuildingDoor(true);
		}
		else
		{
			m_rooms[i]->setIsBuildingDoor(false);
		}
	}
}

void RoomCtrl::Draw()
{
	for (Room* r : m_rooms)
	{
		r->CastShadow();
		r->Draw();
	}

}

int RoomCtrl::getRoomConnections(int index) const
{	
	int connections = 0;
	for (int i = 0; i < m_roomConnectionMap.size(); i++)
	{
		connections += m_roomConnectionMap[index][i];
	}
	for (int i = 0; i < m_outsideDoorPos.size(); i++)
	{
		connections += (m_outsideDoorPos[i].roomIndexes[1] == index);
	}
	return connections;
}

void RoomCtrl::CreateWalls(Room* currentRoom)
{
		
	XMFLOAT3 currentRoomPos = { currentRoom->getPosition().x + 0.5f, 0.0f, currentRoom->getPosition().z + 0.5f };

	int currentRoomSizeY = currentRoom->getSize().y;
	int currentRoomSizeX = currentRoom->getSize().x;

	std::vector<bool> allowedWallsDown;
	std::vector<bool> allowedWallsUp;
	std::vector<bool> allowedWallsLeft;
	std::vector<bool> allowedWallsRight;

	if (m_rooms.size() == 0)
	{
		for (int i = 0; i < currentRoomSizeX; i++)
		{
			allowedWallsDown.push_back(true);
			allowedWallsUp.push_back(true);
		}
		for (int i = 0; i < currentRoomSizeY; i++)
		{
			allowedWallsLeft.push_back(true);
			allowedWallsRight.push_back(true);
		}
	}
	else
	{
		for (auto& room : m_rooms)
		{
			for (int i = 0; i < currentRoomSizeX; i++)
			{
				allowedWallsDown.push_back(true);
				allowedWallsUp.push_back(true);
				for (auto& wall : room->getWalls(Direction::up))
				{
					XMFLOAT2 lol = { i + currentRoomPos.x, currentRoomPos.z };
					XMFLOAT2 upPos = { wall->getObject3D().getPosition().x, wall->getObject3D().getPosition().z + 0.5f };
					if (lol.x == upPos.x && lol.y == upPos.y)
					{
						allowedWallsDown[i] = false;
						wall->setIsShared(true);
						
					}
				}

				for (auto& wall : room->getWalls(Direction::down))
				{
					XMFLOAT2 lol = { i + currentRoomPos.x, currentRoomPos.z + currentRoomSizeY };
					XMFLOAT2 upPos = { wall->getObject3D().getPosition().x, wall->getObject3D().getPosition().z + 0.5f };
					if (lol.x == upPos.x && lol.y == upPos.y)
					{
						allowedWallsUp[i] = false;
						wall->setIsShared(true);
						
					}
				}

			}

			for (int i = 0; i < currentRoomSizeY; i++)
			{
				allowedWallsLeft.push_back(true);
				allowedWallsRight.push_back(true);
				for (auto& wall : room->getWalls(Direction::left))
				{
					XMFLOAT2 lol = { currentRoomPos.x + currentRoomSizeX , currentRoomPos.z + i };
					XMFLOAT2 leftPos = { wall->getObject3D().getPosition().x + 0.5f, wall->getObject3D().getPosition().z };
					if (lol.x == leftPos.x && lol.y == leftPos.y)
					{
						allowedWallsRight[i] = false;
						wall->setIsShared(true);
						
					}
				}

				for (auto& wall : room->getWalls(Direction::right))
				{
					XMFLOAT2 lol = { currentRoomPos.x, currentRoomPos.z + i };
					XMFLOAT2 rightPos = { wall->getObject3D().getPosition().x + 0.5f, wall->getObject3D().getPosition().z };
					if (lol.x == rightPos.x && lol.y == rightPos.y)
					{
						allowedWallsLeft[i] = false;
						wall->setIsShared(true);
						
					}
				}

			}
		

		}
	}
	currentRoom->CreateWallSide(m_wallMesh, allowedWallsDown, down);
	currentRoom->CreateWallSide(m_wallMesh, allowedWallsUp, up);
	currentRoom->CreateWallSide(m_wallMesh, allowedWallsRight, right);
	currentRoom->CreateWallSide(m_wallMesh, allowedWallsLeft, left);
	
}

void RoomCtrl::setDoorMesh(Mesh * mesh)
{
	this->m_doorMesh = mesh;
}

void RoomCtrl::CreateDoor(XMFLOAT3 wallPosition)
{
	for (auto& rooms : m_rooms)
	{
		for (int i = 0; i < 4; i++)
		{
			for (auto& wall : rooms->getWalls(static_cast<Direction>(i)))
			{
				if (wall->getObject3D().getPosition().x == wallPosition.x && wall->getObject3D().getPosition().z == wallPosition.z)
				{
					wall->getObject3D().setMesh(m_doorMesh);
					wall->setIsDoor(true);
					if (wall->isShared())
					{
						DoorPassage dp;
						DoorPassage dp2;

						// Create connection between rooms
						XMINT2 room1 = wall->getNormalPosition();
						XMINT2 room2 = wall->getNegativeNormalPosition();
						int room1Index = _intersect(room1);
						int room2Index = _intersect(room2);

						dp.one = dp2.two = room1;
						dp.two = dp2.one = room2;
						dp.roomIndexes[0] = room1Index;
						dp.roomIndexes[1] = room2Index;
						dp2.roomIndexes[0] = dp.roomIndexes[1];
						dp2.roomIndexes[1] = dp.roomIndexes[0];
						m_roomToRoom.push_back(dp);
						m_roomToRoom.push_back(dp2);

						_makeRoomConnection(room1Index, room2Index);
						_printRoomConnections();
					}
					else
					{
						DoorPassage dp;

						XMINT2 room1 = wall->getNormalPosition();
						XMINT2 room2 = wall->getNegativeNormalPosition();
						int room1Index = _intersect(room1);
						int room2Index = _intersect(room2);
						if (room2Index == -1) // This is the entrance from grid
						{
							dp.one = room2;
							dp.two = room1;
							dp.roomIndexes[0] = room2Index;
							dp.roomIndexes[1] = room1Index;
							std::vector<DoorPassage>::iterator it = std::find(m_outsideDoorPos.begin(), m_outsideDoorPos.end(), dp);
							
							if(it == m_outsideDoorPos.end())
								m_outsideDoorPos.push_back(dp);
						}
						else
						{
							dp.one = room1;
							dp.two = room2;
							dp.roomIndexes[0] = room1Index;
							dp.roomIndexes[1] = room2Index;
							std::vector<DoorPassage>::iterator it = std::find(m_outsideDoorPos.begin(), m_outsideDoorPos.end(), dp);

							if (it == m_outsideDoorPos.end())
								m_outsideDoorPos.push_back(dp);
						}

						std::cout << "This door is Outside->Inside || Inside->Outside";
					}

					return;
				}
			}

		}
	}
}

RoomCtrl::DoorPassage RoomCtrl::getClosestEntranceDoor(XMINT2 startPosition) const
{
	XMVECTOR  ourPos = XMLoadSInt2(&startPosition);
	int index = 0;
	int length = 1000;
	for (int i = 0; i < m_outsideDoorPos.size(); i++)
	{
		XMVECTOR door1 = XMLoadSInt2(&m_outsideDoorPos[i].one);
		int c = static_cast<int>(XMVectorGetX(XMVector2Length(ourPos - door1)));
		if (length > c)
		{
			length = c;
			index = i;
		}
	}
	return m_outsideDoorPos[index];
}

RoomCtrl::DoorPassage RoomCtrl::getDoorPassage(int index1, int index2) const
{
	for (int i = 0; i < m_roomToRoom.size(); i++)
	{
		if (m_roomToRoom[i].roomIndexes[0] == index1 && m_roomToRoom[i].roomIndexes[1] == index2)
			return m_roomToRoom[i];
	}
	return DoorPassage();
}

std::vector<int> RoomCtrl::roomTraversal(Tile * roomTile1, Tile * roomTile2)
{
	m_tempPath.clear();
	int indexes[2] = { -1 };
	XMINT2 room1Pos = { (int)roomTile1->getQuad().getPosition().x, (int)roomTile1->getQuad().getPosition().z };
	XMINT2 room2Pos = { (int)roomTile2->getQuad().getPosition().x, (int)roomTile2->getQuad().getPosition().z };
	indexes[0] = _intersect(room1Pos);
	indexes[1] = _intersect(room2Pos);

	_dijkstra(indexes[0], indexes[1]);
		
	return m_tempPath;
}

Room * RoomCtrl::getRoomAt(int index)
{
	return m_rooms[index];
}

Room * RoomCtrl::getRoomAtPos(XMINT2 pos)
{
	int index = _intersect(pos);
	Room* targetRoom = getRoomAt(index);
	return targetRoom;
}

std::vector<Room*> RoomCtrl::getAllTheRooms() const
{
	return m_rooms;
}

Furniture * RoomCtrl::getFurnitureAtPos(XMINT2 pos)
{
	Furniture * temp = nullptr;
	for (auto& element : m_rooms)
	{
		temp = element->getFurnitureAtPos(pos);
		if (temp)
		{
			return temp;
		}
	}
	return nullptr;
}

DirectX::XMFLOAT3 RoomCtrl::getClosestRoom(XMFLOAT2 position, RoomType type)
{
	auto getLength = [&](XMFLOAT2 pos, XMFLOAT3 roomPos)->int {
		XMFLOAT3 temp = { pos.x, 0, pos.y };
		XMVECTOR first = XMLoadFloat3(&temp);
		XMVECTOR second = XMLoadFloat3(&roomPos);

	
		return (int)XMVectorGetX(XMVector3Length(second - first));
	};
	Room* closestRoom = nullptr;
	int closestDistance = 99999;
	for (auto& room : m_rooms)
	{
		if (_checkRoomType(room, type))
		{
			int distance = getLength(position, room->getPosition());
			if (closestDistance > distance)
			{
				closestRoom = room;
				closestDistance = distance;
			}
				
		}
	}
	if (closestRoom == nullptr)
		return DirectX::XMFLOAT3(-1, -1,-1);
	
	return closestRoom->getPosition();
}

Direction RoomCtrl::getDirection(Tile * t1, Tile * t2)
{
	
	XMVECTOR oldPosWithoutOffset = XMLoadFloat3(&t1->getQuad().getPosition());
	XMVECTOR xmNewPos = XMLoadFloat3(&t2->getQuad().getPosition());
	XMVECTOR xmDeltaPos = xmNewPos - oldPosWithoutOffset;
	XMFLOAT3 result;
	XMStoreFloat3(&result, xmDeltaPos);
	Direction dir;

	if (result.x > 0)		dir = Direction::right;
	else if (result.x < 0)	dir = Direction::left;
	else if (result.z > 0)	dir = Direction::up;
	else if (result.z < 0)	dir = Direction::down;

	return dir;
}

XMINT2 RoomCtrl::getDirection2i(Tile *t1, Tile *t2)
{
	XMVECTOR oldPosWithoutOffset = XMLoadFloat3(&t1->getQuad().getPosition());
	XMVECTOR xmNewPos = XMLoadFloat3(&t2->getQuad().getPosition());
	XMVECTOR xmDeltaPos = xmNewPos - oldPosWithoutOffset;
	XMFLOAT3 resultFloat3;
	XMStoreFloat3(&resultFloat3, xmDeltaPos);
	
	XMINT2 result = { static_cast<int>(resultFloat3.x), static_cast<int>(resultFloat3.z )};

	return result;
}

Direction RoomCtrl::getDirection(Room * r1, Room * r2)
{
	XMVECTOR r1Pos = XMLoadFloat3(&r1->getPosition());

	XMFLOAT3 r2PosLOL = r2->getPosition();
	int r2SizeX = r2->getSize().x;
	int r2SizeY = r2->getSize().y;

	XMVECTOR r2Pos[4];
	r2Pos[0] = r2Pos[1] = r2Pos[2] = r2Pos[3] = XMLoadFloat3(&r2PosLOL);
	
	r2Pos[1] = XMVectorSetX(r2Pos[1], XMVectorGetX(r2Pos[1]) + r2SizeX);
		 
	r2Pos[2] = XMVectorSetX(r2Pos[2], XMVectorGetX(r2Pos[2]) + r2SizeX);
	r2Pos[2] = XMVectorSetZ(r2Pos[2], XMVectorGetZ(r2Pos[2]) + r2SizeY);
		 
	r2Pos[3] = XMVectorSetZ(r2Pos[3], XMVectorGetZ(r2Pos[3]) + r2SizeY);
	
	float distanceToR1 = 99999.0f;
	int index = -1;
	for (int i = 0; i < 4; i++)
	{
		float temp = XMVectorGetX(XMVector3Length(r2Pos[i] - r1Pos));
		if (temp < distanceToR1)
		{
			distanceToR1 = temp;
			index = i;
		}
	}

	XMVECTOR oldPosWithoutOffset = r1Pos;
	XMVECTOR xmNewPos = r2Pos[index];
	XMVECTOR xmDeltaPos = xmNewPos - oldPosWithoutOffset;
	XMFLOAT3 result;
	XMStoreFloat3(&result, xmDeltaPos);
	Direction dir;

	if (result.x > 0)		dir = Direction::right;
	else if (result.x < 0)	dir = Direction::left;
	else if (result.z > 0)	dir = Direction::up;
	else if (result.z < 0)	dir = Direction::down;

	return dir;
}

std::vector<Furniture*> RoomCtrl::getNoneBusyFurnitureInRoom(DirectX::XMINT2 pos)
{
	return getRoomAtPos(pos)->getNoneBusyFurnitures();
}
std::vector<Furniture*> RoomCtrl::getNoneBusyFurnitureInRoom(DirectX::XMFLOAT2 pos)
{
	return getRoomAtPos(XMINT2(static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.y)))->getNoneBusyFurnitures();
}
std::vector<Furniture*> RoomCtrl::getNoneBusyFurnitureInRoom(DirectX::XMFLOAT3 pos)
{
	return getRoomAtPos(XMINT2(static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.z)))->getNoneBusyFurnitures();
}

bool RoomCtrl::getIsBuildingDoor()
{
	return m_buildingDoors;
}

void RoomCtrl::setIsBuildingDoor(bool tje)
{
	m_buildingDoors = tje;
}

