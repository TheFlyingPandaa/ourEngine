#include "RoomCtrl.h"
#include "../../Furniture/Table.h"
#include "../../Mesh Manager/MeshLoaderPlus.h"

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
		parent[i] = -1;
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

	int counter = 0;
	for (int i = 0; i < m_roomConnectionMap[src].size(); i++)
	{
		if (parent[i] == -1)
			counter++;
	}
	if(counter != m_roomConnectionMap[src].size())
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
	if (index == -1) return;
	Room* cr = m_rooms[index];
	auto m_tiles = cr->getTiles();
	auto _index = [&](int x, int y) ->int
	{
		int xCoord = (x - static_cast<int>(cr->getPosition().x));
		int yCoord = (y - static_cast<int>(cr->getPosition().z));
		int roomSizeX = cr->getSize().x;
		int roomSizeY = cr->getSize().y;
		if (xCoord >= roomSizeX || xCoord < 0) return -1;
		if (yCoord >= roomSizeY || yCoord < 0) return -1;
		return static_cast<int>(xCoord + yCoord * cr->getSize().x);
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
				m_tiles[_index(furniture->getPosition().x - i, furniture->getPosition().z)]->setHasObject(true);
			}
		}
	}

	//m_tiles[_index(furniture.getPosition().x, furniture.getPosition().z)]->setIsWalkeble(false);
	m_tiles[_index(furniture->getPosition().x, furniture->getPosition().z)]->setHasObject(true);

	//furniture.setLightIndex(cr->getRoomIndex());
	cr->AddRoomObject(furniture);
}

bool RoomCtrl::RemoveRoomObject(Furniture * fur)
{
	int index = _intersect(XMINT2(fur->getPosition().x, fur->getPosition().z), XMINT2(1, 1));
	if (index != -1)
	{
		return m_rooms[index]->RemoveThisFurniture(fur);
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


}

void RoomCtrl::_removeFromRoomToRoom(int room1, int room2)
{
	int removesCounter = 0;
	for (std::vector<DoorPassage>::iterator it = m_roomToRoom.begin(); 
		it != m_roomToRoom.end();
		)
	{
		DoorPassage c = *it;
		if ((room1 == c.roomIndexes[0] && room2 == c.roomIndexes[1])
			|| (room2 == c.roomIndexes[0] && room1 == c.roomIndexes[1]))
		{
			it = m_roomToRoom.erase(it); 
			removesCounter++;
		}
		else
			it++;
	}
}

RoomCtrl::RoomCtrl()
{
	MLP::GetInstance().LoadMeshRectangle(MESH::KITCHENTILE);
	MLP::GetInstance().GetMesh(MESH::KITCHENTILE)->setDiffuseTexture("trolls_inn/Resources/GenericTexture/KitchenTile.jpg");
	MLP::GetInstance().GetMesh(MESH::KITCHENTILE)->setNormalTexture("trolls_inn/Resources/woodenfloor/NormalMap.png");

	MLP::GetInstance().LoadMeshRectangle(MESH::BEDROOMTILE);
	MLP::GetInstance().GetMesh(MESH::BEDROOMTILE)->setDiffuseTexture("trolls_inn/Resources/GenericTexture/BedRoomTile.jpg");
	MLP::GetInstance().GetMesh(MESH::BEDROOMTILE)->setNormalTexture("trolls_inn/Resources/DefaultNormal.png");

	MLP::GetInstance().LoadMeshRectangle(MESH::RECEPTIONTILE);
	MLP::GetInstance().GetMesh(MESH::RECEPTIONTILE)->setDiffuseTexture("trolls_inn/Resources/GenericTexture/ReceptionTile.jpg");
	MLP::GetInstance().GetMesh(MESH::RECEPTIONTILE)->setNormalTexture("trolls_inn/Resources/DefaultNormal.png");

	MLP::GetInstance().LoadMeshRectangle(MESH::HALLWAYTILE);
	MLP::GetInstance().GetMesh(MESH::HALLWAYTILE)->setDiffuseTexture("trolls_inn/Resources/GenericTexture/HallwayTile.jpg");
	MLP::GetInstance().GetMesh(MESH::HALLWAYTILE)->setNormalTexture("trolls_inn/Resources/DefaultNormal.png");

	MLP::GetInstance().LoadMeshRectangle(MESH::WOODTILE);
	MLP::GetInstance().GetMesh(MESH::WOODTILE)->setDiffuseTexture("trolls_inn/Resources/GenericTexture/wood.png");
	MLP::GetInstance().GetMesh(MESH::WOODTILE)->setNormalTexture("trolls_inn/Resources/woodenfloor/NormalMap.png");
	

	MLP::GetInstance().LoadMesh(MESH::WALL, "wall3.obj");
	
	MLP::GetInstance().LoadMesh(MESH::DOOR, "door/Door.obj");
	

	m_buildingDoors = false;
}


RoomCtrl::~RoomCtrl()
{
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		delete m_rooms[i];
	}
	m_rooms.clear();
}


void RoomCtrl::AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<Tile*> tiles, bool force)
{
	Room * currentRoom = nullptr;

	switch (roomType)
	{
	case kitchen:
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(MLP::GetInstance().GetMesh(MESH::KITCHENTILE));
		break;
	case bedroom:
		//Duno just copied the 
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(MLP::GetInstance().GetMesh(MESH::BEDROOMTILE));
		break;
	case reception:
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(MLP::GetInstance().GetMesh(MESH::RECEPTIONTILE));
		break;
	case hallway:
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(MLP::GetInstance().GetMesh(MESH::HALLWAYTILE));
		break;
	case bar:
		currentRoom = new Room(pos.x, pos.y, size.x, size.y, tiles, roomType);
		currentRoom->setFloorMesh(MLP::GetInstance().GetMesh(MESH::WOODTILE));
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

	// If we create a room where an outside door was, the door is instead connected to the created room
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
#pragma optimize ("", off)

std::tuple<bool, int> RoomCtrl::RemoveRoomTuple(DirectX::XMINT2 pos, std::vector<Tile*>& backtiles, DirectX::XMINT2 & delPos, DirectX::XMINT2 & delSize)
{

	int index = _intersect(pos, XMINT2(1, 1));
	int payBack = 0;
	
	if (index != -1)
	{
		

		// Remove all this rooms doors
		std::vector<DoorPassage> cpy(m_roomToRoom);
		int oldSize = m_roomToRoom.size();
	
		for (auto& dp : cpy)
		{
			if (dp.roomIndexes[0] == index)
			{
				XMFLOAT3 t1(dp.one.x, 0.0f, dp.one.y);
				XMFLOAT3 t2(dp.two.x, 0.0f, dp.two.y);

				// t1 -> t2
				XMFLOAT3 wallPosition(((t2.x + t1.x) * 0.5f) + 0.5f, 0.0f, ((t2.z + t1.z) * 0.5f) + 0.5f);
				std::cout << "delete wallPos (" << wallPosition.x << "," << wallPosition.z << ")\n";
				assert(RemoveDoor(wallPosition));
				assert(oldSize != m_roomToRoom.size());
				oldSize = m_roomToRoom.size();
			}
			
		}

	
		std::cout << "Size of m_roomToRoom= " << m_roomToRoom.size() << "\n";
		
		
		for (int i = 0; i < m_outsideDoorPos.size(); i++)
		{
			if (m_outsideDoorPos[i].roomIndexes[1] == index)
			{
				m_outsideDoorPos.erase(m_outsideDoorPos.begin() + i);
				i = 0;
			}
		}

		for (auto& row : m_roomConnectionMap)
		{
			row.erase(row.begin() + index);
		}
		m_roomConnectionMap.erase(m_roomConnectionMap.begin() + index);

		payBack = m_rooms[index]->getPriceOfAllObjects();
		backtiles = m_rooms[index]->ReturnTiles();
		XMFLOAT3 _pos = m_rooms[index]->getPosition();
		delSize = m_rooms[index]->getSize();
		delPos = { static_cast<int>(_pos.x), static_cast<int>(_pos.z) };
		delete m_rooms[index];
		m_rooms.erase(m_rooms.begin() + index);
		for (int i = 0; i < m_roomToRoom.size(); i++)
		{
			int firstRoomIndex = _intersect(m_roomToRoom[i].one);
			int secondRoomIndex = _intersect(m_roomToRoom[i].two);


			m_roomToRoom[i].roomIndexes[0] = firstRoomIndex;
			m_roomToRoom[i].roomIndexes[1] = secondRoomIndex;


		}

		for (auto& r : m_rooms)
		{
			CreateWalls(r);
		}

		payBack += (delSize.x * delSize.y) * 20;
		} 
	_printRoomConnections();
	return { index != -1, payBack >> 1 }; //c++17 way of making a tuple
}
#pragma optimize ("", on)

 bool RoomCtrl::CheckAndMarkTilesObject(DirectX::XMINT2 start, int size, int angle)
 {
	 bool isFalse = false;
	 int indexx = _intersect(start, XMINT2(1, 1));
	 if (indexx == -1)
	 {
		 return false;
	 }
	 Room*  cr = m_rooms[indexx];
	 auto tiles = cr->getTiles();
	 auto _index = [&](int x, int y) ->int
	 {
		 int xCoord = x - static_cast<int>(cr->getPosition().x);
		 int yCoord = y - static_cast<int>(cr->getPosition().z);

		 if (xCoord >= 0 && xCoord < cr->getSize().x && yCoord >= 0 && yCoord < cr->getSize().y)
			 return static_cast<int>(xCoord + yCoord * cr->getSize().x);
		 else
			 return -1;
	 };
	 
	if (angle == 0 || angle == 180)
	{
		for (int i = 0; i < size; i++)
		{
		

			if (angle == 0)
			{
				int ii = _index(start.x, start.y + i);
				if (ii == -1)
				{
					return false;
				}
				Tile* t = tiles[ii];
				if (t && t->getHasObject() == false)
				{
					//t->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else if(t)
				{
					//t->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
			else
			{
				int tileIndex = _index(start.x, start.y - i);
				if (tileIndex != -1)
				{
					Tile* t = tiles[_index(start.x, start.y - i)];
					if (t && t->getHasObject() == false)
					{
						//t->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
					}
					else if (t)
					{
						//t->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
						isFalse = true;
					}
				}
				else
				{
					return false;
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
				int index = _index(start.x + i, start.y);
				if (index == -1)
					return false;
				
					if (tiles[_index(start.x + i, start.y)]->getHasObject() == false)
					{
						//tiles[_index(start.x + i, start.y)]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
					}
					else
					{
						//tiles[_index(start.x + i, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
						isFalse = true;
					}				
			}
			else
			{
				int ii = _index(start.x - i, start.y);
				if (ii == -1)
				{
					return false;
				}
				if (tiles[_index(start.x - i, start.y)]->getHasObject() == false)
				{
					//tiles[_index(start.x - i, start.y)]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else
				{
					//tiles[_index(start.x - i, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
		}
	}

	if (tiles[_index(start.x, start.y)]->getHasObject() == true)
	{
		//tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
		return false;
	}
	else
	{
		if (isFalse == true)
		{
			//tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
			return false;
		}
		else
		{
			//tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
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
		 int xCoord = x - static_cast<int>(cr->getPosition().x);
		 int yCoord = y - static_cast<int>(cr->getPosition().z);

		 if (xCoord >= 0 && xCoord < cr->getSize().x && yCoord >= 0 && yCoord < cr->getSize().y)
			 return static_cast<int>(xCoord + yCoord * cr->getSize().x);
		 else
			 return -1;
	 };

	 if (angle == 0 || angle == 180)
	 {
		 for (int i = 0; i < size; i++)
		 {
			 int ii = _index(start.x, start.y + i);
			 if (ii == -1) return false;
			 if (ii >= tiles.size()) {
				 //tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
				 return false;
			 }

			 if (angle == 0)
			 {
				 Tile* t = tiles[ii];
				 if (t && t->getHasObject() == false)
				 {
					// t->getQuad().setColor(XMFLOAT3(8.0f, 0.5f, 0.5f));
				 }
				 else if (t)
				 {
					// t->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
					 isFalse = false;
				 }
			 }
			 else
			 {
				 Tile* t = tiles[_index(start.x, start.y - i)];
				 if (t && t->getHasObject() == false)
				 {
					// t->getQuad().setColor(XMFLOAT3(8.0f, 0.5f, 0.5f));
				 }
				 else if (t)
				 {
					// t->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
					 isFalse = false;
				 }
			 }
		 }
	 }
	 if (angle == 90 || angle == 270)
	 {
		 for (int i = 0; i < size; i++)
		 {
			 int ii = _index(start.x + i, start.y);
			 if (ii == -1) continue;

			 if (angle == 90)
			 {
				

				 if (tiles[ii]->getHasObject() == false)
				 {
					// tiles[ii]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
				 }
				 else
				 {
					// tiles[ii]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
					 isFalse = false;
				 }
			 }
			 else
			 {
				 if (tiles[ii]->getHasObject() == false)
				 {
					// tiles[ii]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
				 }
				 else
				 {
					// tiles[ii]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
					 isFalse = false;
				 }
			 }
		 }
	 }

	 if (tiles[_index(start.x, start.y)]->getHasObject() == true)
	 {
		// tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
		 return false;
	 }
	 else
	 {
		 if (isFalse == true)
		 {
			 //tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
			 return false;
		 }
		 else
		 {
			// tiles[_index(start.x, start.y)]->getQuad().setColor(XMFLOAT3(8.5f, 0.5f, 0.5f));
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
		if (r)
		{
			r->CastShadow();
			r->Draw();
		}
	}

}

int RoomCtrl::getRoomConnections(int index) const
{	
	int connections = 0;
	for (int i = 0; i < m_roomConnectionMap.size(); i++)
	{
		connections += m_roomConnectionMap[index][i];
	}
	return connections;
}

int RoomCtrl::HasEntranceDoor() const
{
	return m_outsideDoorPos.size();
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
					if ((lol.x == upPos.x && lol.y == upPos.y))
					{
						allowedWallsDown[i] = false;
						wall->setIsShared(true);
						if (wall->getIsDoor())
						{
							allowedWallsUp[i] = false;
							wall->setIsShared(false);
						}
					}
					
					
					
					
				}

				for (auto& wall : room->getWalls(Direction::down))
				{
					
					
					XMFLOAT2 lol = { i + currentRoomPos.x, currentRoomPos.z + currentRoomSizeY };
					XMFLOAT2 upPos = { wall->getObject3D().getPosition().x, wall->getObject3D().getPosition().z + 0.5f };

					if ((lol.x == upPos.x && lol.y == upPos.y))
					{
						allowedWallsUp[i] = false;
						wall->setIsShared(true);
						if (wall->getIsDoor())
						{
							allowedWallsDown[i] = false;
							wall->setIsShared(false);
						}
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
					if ((lol.x == leftPos.x && lol.y == leftPos.y))
					{
						allowedWallsRight[i] = false;
						wall->setIsShared(true);
						if (wall->getIsDoor())
						{
							allowedWallsLeft[i] = false;
							wall->setIsShared(false);
						}
					}
					
					
					
				}

				for (auto& wall : room->getWalls(Direction::right))
				{
					
					
					XMFLOAT2 lol = { currentRoomPos.x, currentRoomPos.z + i };
					XMFLOAT2 rightPos = { wall->getObject3D().getPosition().x + 0.5f, wall->getObject3D().getPosition().z };
					if ((lol.x == rightPos.x && lol.y == rightPos.y))
					{
						allowedWallsLeft[i] = false;
						wall->setIsShared(true);
						if (wall->getIsDoor())
						{
							allowedWallsRight[i] = false;
							wall->setIsShared(false);
						}
					}
					
					
				}

			}
		

		}
	}
	currentRoom->CreateWallSide(MLP::GetInstance().GetMesh(MESH::WALL), allowedWallsDown, down);
	currentRoom->CreateWallSide(MLP::GetInstance().GetMesh(MESH::WALL), allowedWallsUp, up);
	currentRoom->CreateWallSide(MLP::GetInstance().GetMesh(MESH::WALL), allowedWallsRight, right);
	currentRoom->CreateWallSide(MLP::GetInstance().GetMesh(MESH::WALL), allowedWallsLeft, left);
	
}

void RoomCtrl::CreateDoor(XMFLOAT3 wallPosition, float rot)
{
	// We need to search for the room with this wallPosition
	for (auto& rooms : m_rooms)
	{
		for (int i = 0; i < 4; i++)
		{
			for (auto& wall : rooms->getWalls(static_cast<Direction>(i)))
			{
				if (wall->getObject3D().getPosition().x == wallPosition.x && wall->getObject3D().getPosition().z == wallPosition.z)
				{
					if (!wall->getIsDoor())
					{
						std::cout << "Wall Position (" << wall->getObject3D().getPosition().x << "," << wall->getObject3D().getPosition().z << ")\n";
						wall->getObject3D().setMesh(MLP::GetInstance().GetMesh(MESH::DOOR));

						if (rot != 0.0)
						{
							DirectX::XMFLOAT3 r = wall->getObject3D().getRotation();
							r.y = DirectX::XMConvertToDegrees(rot);
							wall->setRotation(r);
						}
						wall->setIsDoor(true);
						// If the wall is shared, then the door will connect two rooms
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
							//_printRoomConnections();
						}
						else // This door leads to the outside
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

								if (it == m_outsideDoorPos.end())
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

						}
						// We are done!
						return;
					}
			
				}
			}

		}
	}
}

bool RoomCtrl::RemoveDoor(XMFLOAT3 wallPosition)
{
	bool haveRemoved = false;
	for (auto& rooms : m_rooms)
	{
		for (auto& wall : *rooms->getAllWalls())
		{
			XMFLOAT3 wallPos = wall->getObject3D().getPosition();
				
			if (wallPos.x == wallPosition.x && wallPos.z == wallPosition.z)
			{
				//// Should be a door
				assert(wall->getIsDoor());
					
				wall->getObject3D().setMesh(MLP::GetInstance().GetMesh(MESH::WALL));
				wall->setIsDoor(false);

				// Now we need to remove the door from door to door passage arrays

				// Create connection between rooms
				XMINT2 room1 = wall->getNormalPosition();
				XMINT2 room2 = wall->getNegativeNormalPosition();
				int room1Index = _intersect(room1);
				int room2Index = _intersect(room2);
				assert(room1Index != -1 || room2Index != -1);

				_removeFromRoomToRoom(room1Index, room2Index);

				haveRemoved = true;

			}
		}

	}
	return haveRemoved;
}

std::vector<RoomCtrl::DoorPassage>& RoomCtrl::getAllEntranceDoors()
{
	return m_outsideDoorPos;
}

RoomCtrl::DoorPassage RoomCtrl::getClosestEntranceDoor(XMINT2 startPosition) const
{
	XMVECTOR  ourPos = XMLoadSInt2(&startPosition);
	int index = -1;
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
	if (index > -1)
		return m_outsideDoorPos[index];
	else
		throw "NO EXITS";
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
	
	// We cant perfrom dijkstra on the same room
	if(indexes[0] != indexes[1])
		_dijkstra(indexes[0], indexes[1]);

		
	return m_tempPath;
}

Room * RoomCtrl::getRoomAt(int index)
{
	if (index == -1)
	{
		return nullptr;
	}
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
			if (closestDistance > distance && room->CheckRoomComplete())
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

