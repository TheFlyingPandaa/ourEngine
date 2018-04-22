#include "RoomCtrl.h"



void RoomCtrl::setTileMesh(Mesh * mesh, RoomType roomType)
{
	m_tileMesh[roomType] = mesh;
}

bool RoomCtrl::_intersect(Room * room)
{
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		if ((std::abs(room->getX() - m_rooms[i]->getX()) * 2) < (room->getSize().x + m_rooms[i]->getSize().x) &&
			(std::abs(room->getY() - m_rooms[i]->getY()) * 2) < (room->getSize().y + m_rooms[i]->getSize().y))
			return true;
	}
	return false;
}

bool RoomCtrl::_intersect(DirectX::XMINT2 pos, DirectX::XMINT2 size)
{
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		/*if ((pos.x > m_rooms[i]->getX() && pos.y > m_rooms[i]->getY() && 
			pos.x < m_rooms[i]->getX() + m_rooms[i]->getSizeX() && 
			pos.y < m_rooms[i]->getY() + m_rooms[i]->getSizeY()) || 
			(pos.x + size.x > m_rooms[i]->getX() && pos.y + size.y > m_rooms[i]->getY() &&
			pos.x + size.x < m_rooms[i]->getX() + m_rooms[i]->getSizeX() &&
			pos.y + size.y < m_rooms[i]->getY() + m_rooms[i]->getSizeY()))*/

		/*if (!(pos.x > m_rooms[i]->getX() + m_rooms[i]->getSizeX()
			|| pos.x + size.x < m_rooms[i]->getX()
			|| pos.y + size.y > m_rooms[i]->getY()
			|| pos.y > m_rooms[i]->getY() + m_rooms[i]->getSizeY()))*/
		//if ((std::abs(pos.x - m_rooms[i]->getX()) * 2) < (size.x + m_rooms[i]->getSizeX()) &&
		//	(std::abs(pos.y - m_rooms[i]->getY()) * 2) < (size.y + m_rooms[i]->getSizeY()))
		if (pos.x < m_rooms[i]->getX() + m_rooms[i]->getSize().x &&
			pos.x + size.x > m_rooms[i]->getX() &&
			pos.y < m_rooms[i]->getY() + m_rooms[i]->getSize().y &&
			pos.y + size.y > m_rooms[i]->getY())
			return true;

	}
	return false;
}

bool RoomCtrl::isPlaceable(DirectX::XMINT2 pos, DirectX::XMINT2 size)
{
	Room* r;
	int x, mx;
	int y, my;

	bool isPlaceable = true;

	for (int i = 0; i < m_rooms.size(); i++)
	{
		r = m_rooms[i];
		x = r->getX();
		mx = r->getX() + r->getSize().x;
		y = r->getY();
		my = r->getY() + r->getSize().y;

		if (pos.x < m_rooms[i]->getX() + m_rooms[i]->getSize().x &&
			pos.x + size.x > m_rooms[i]->getX() &&
			pos.y < m_rooms[i]->getY() + m_rooms[i]->getSize().y &&
			pos.y + size.y > m_rooms[i]->getY())
			return false;
		/*
		if ((pos.y == my || pos.y + size.y == y))
		{	
			if ((pos.x < mx && pos.x + size.x > x))
				isPlaceable = true;			
		}
		if ((pos.x == mx || pos.x + size.x == x))
		{
			if ((pos.y < my && pos.y + size.y > y))
				isPlaceable = true;
		}
		*/
		
	}

	return isPlaceable;
}

bool RoomCtrl::_checkLegal(Room * room)
{

	return dynamic_cast<Bedroom*>(room) == nullptr;
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

	std::cout << "Room " << m_rooms.size() - 1 << "->" << 0 << ": ";
	for (int i = 0; i < m_tempPath.size(); i++)
		std::cout << m_tempPath[i] << " ";
	std::cout << std::endl;

}

RoomCtrl::RoomCtrl()
{
	m_entrance = nullptr;
	
	m_tileMesh[0] = new Mesh();
	m_tileMesh[0]->MakeRectangle();
	m_tileMesh[0]->setDiffuseTexture("trolls_inn/Resources/wood.png");
	
	m_wallMesh = new Mesh();
	m_wallMesh->LoadModel("trolls_inn/Resources/wall3.obj");
}


RoomCtrl::~RoomCtrl()
{
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		delete m_rooms[i];
	}

	delete m_wallMesh;
	delete m_tileMesh[0];
	m_rooms.clear();
}

void RoomCtrl::addRoomObject(DirectX::XMINT2 pos, Mesh * mesh)
{
	Object3D tempObj;
	tempObj.setMesh(mesh);
	tempObj.setPos(pos.x + 0.5f, 0, pos.y + 0.5f);
	
	//m_roomObjects.push_back(tempObj);
	
}


void RoomCtrl::AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<Tile*> tiles, bool force)
{
	Room * currentRoom = nullptr;
	if (!force)
	{		
		if (!isPlaceable(pos, size))
			return;
	}

	switch (roomType)
	{
	case kitchen:
		/*room = new Kitchen(pos.x, pos.y, size.x, size.y, tiles);

		if (m_tileMesh[0] != nullptr)
			room->setTile(m_tileMesh[0]);*/
		break;
	case bedroom:
		break;
	case reception:
		currentRoom = new Reception(pos.x, pos.y, size.x, size.y, tiles);
		currentRoom->setFloorMesh(m_tileMesh[0]);
		break;
	}

	CreateWalls(currentRoom);

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
	_printRoomConnections();

	if (!m_entrance) m_entrance = m_rooms.back();

	
}

void RoomCtrl::Update(Camera * cam)
{
	
	for (int i = 0; i < m_rooms.size(); i++)
	{
		m_rooms[i]->Update(cam);
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
			allowedWallsDown.push_back(true);
		for (int i = 0; i < currentRoomSizeY; i++)
			allowedWallsLeft.push_back(true);

		currentRoom->CreateWallSide(m_wallMesh, allowedWallsDown, down);
		currentRoom->CreateWallSide(m_wallMesh, allowedWallsDown, up);
		currentRoom->CreateWallSide(m_wallMesh, allowedWallsLeft, left);
		currentRoom->CreateWallSide(m_wallMesh, allowedWallsLeft, right);
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
						allowedWallsDown[i] = false;
				}

				for (auto& wall : room->getWalls(Direction::down))
				{
					XMFLOAT2 lol = { i + currentRoomPos.x, currentRoomPos.z + currentRoomSizeY };
					XMFLOAT2 upPos = { wall->getObject3D().getPosition().x, wall->getObject3D().getPosition().z + 0.5f };
					if (lol.x == upPos.x && lol.y == upPos.y)
						allowedWallsUp[i] = false;
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
						break;
					}
				}

				for (auto& wall : room->getWalls(Direction::right))
				{
					XMFLOAT2 lol = { currentRoomPos.x, currentRoomPos.z + i };
					XMFLOAT2 rightPos = { wall->getObject3D().getPosition().x + 0.5f, wall->getObject3D().getPosition().z };
					if (lol.x == rightPos.x && lol.y == rightPos.y)
						allowedWallsLeft[i] = false;
				}

			}
			currentRoom->CreateWallSide(m_wallMesh, allowedWallsDown, down);
			currentRoom->CreateWallSide(m_wallMesh, allowedWallsUp, up);
			currentRoom->CreateWallSide(m_wallMesh, allowedWallsRight, right);
			currentRoom->CreateWallSide(m_wallMesh, allowedWallsLeft, left);

		}
	}

	
}

void RoomCtrl::CreateDoors(Room * room)
{
	////get other room type 
	//for (auto& other : *room->getAdjasent()) {
	//	if (_checkLegal(other)) {
	//		/*Direction dir = getDirection(room, other);
	//		Direction otherDir = getDirection(other, room);*/
	//		std::vector<Wall*>* createdRoomWall = room->getAllWalls();
	//		std::vector<Wall*>* otherRoomWall = other->getAllWalls();

	//		std::vector<Wall*> intersectedWalls;

	//		
	//		for (int i = 0; i < createdRoomWall->size(); i++)
	//		{
	//			if(createdRoomWall->at(i)->getIsInner())
	//				for (int j = 0; j < otherRoomWall->size(); j++)
	//				{
	//					if (otherRoomWall->at(j)->getIsInner())
	//						if (createdRoomWall->at(i) == otherRoomWall->at(j))
	//						{
	//							intersectedWalls.push_back(createdRoomWall->at(i));
	//							break;
	//						}
	//				}
	//		}
	//		if (intersectedWalls.size() <= 0)
	//			return;
	//		Wall* door = intersectedWalls[(intersectedWalls.size()  ) / 2];
	//		Tile* t1 = door->getTile();
	//		Direction dir;
	//		for (int i = 0; i < 4; i++)
	//		{
	//			if (t1->getTileWalls(static_cast<Direction>(i)) == door)
	//			{
	//				dir = static_cast<Direction>(i);
	//				break;
	//			}
	//		}

	//		

	//		DirectX::XMINT2 pos = door->getPosition();
	//		pos.x -= static_cast<int>(room->getPosition().x);
	//		pos.y -= static_cast<int>(room->getPosition().z);
	//		
	//		
	//		Tile* t2 = t1->getAdjacent(dir);

	//		CreateDoor(t1, t2);

	//	}
	//}
}

// Not in use
DirectX::XMINT2 RoomCtrl::CreateMainDoor(Room * room)
{
	//for (auto& other : *room->getAdjasent()) {
	//	if (_checkLegal(other)) {
	//		/*Direction dir = getDirection(room, other);
	//		Direction otherDir = getDirection(other, room);*/
	//		std::vector<Wall*>* createdRoomWall = room->getAllWalls();
	//		std::vector<Wall*>* otherRoomWall = other->getAllWalls();

	//		std::vector<Wall*> intersectedWalls;


	//		for (int i = 0; i < createdRoomWall->size(); i++)
	//		{
	//			if (createdRoomWall->at(i)->getIsInner())
	//				for (int j = 0; j < otherRoomWall->size(); j++)
	//				{
	//					if (otherRoomWall->at(j)->getIsInner())
	//						if (createdRoomWall->at(i) == otherRoomWall->at(j))
	//						{
	//							intersectedWalls.push_back(createdRoomWall->at(i));
	//							break;
	//						}
	//				}
	//		}
	//		if (intersectedWalls.size() <= 0)
	//			return DirectX::XMINT2(-1337,-1337);
	//		Wall* door = intersectedWalls[(intersectedWalls.size()) / 2];
	//		Tile* t1 = door->getTile();
	//		Direction dir;
	//		for (int i = 0; i < 4; i++)
	//		{
	//			if (t1->getTileWalls(static_cast<Direction>(i)) == door)
	//			{
	//				dir = static_cast<Direction>(i);
	//				break;
	//			}
	//		}



	//		DirectX::XMINT2 pos = door->getPosition();
	//		pos.x -= static_cast<int>(room->getPosition().x);
	//		pos.y -= static_cast<int>(room->getPosition().z);

	//		/*if (pos.x >= 1)
	//		pos.x -= 1;*/

	//		//Tile* t1 = room->getTiles()[pos.x - 1][pos.y];
	//		Tile* t2 = t1->getAdjacent(dir);

	//		CreateDoor(t1, t2);
	//		return pos;
	//	}
	//}
	//std::cout << "CRASHH ROOM CTRL IN THE CREATE MAIN DOOR NO POS WAS RETURNED FUCKING SHIT" << std::endl;
	return DirectX::XMINT2(-1337, -1337);
}

void RoomCtrl::setDoorMesh(Mesh * mesh)
{
	this->m_doorMesh = mesh;
}

void RoomCtrl:: CreateDoor(Tile * tile1, Tile * tile2)
{
	//Direction dir = this->getDirection(tile1, tile2);

	//Wall* w = tile1->getTileWalls(dir);
	//if (!w)
	//	return;
	//w->setScale(1.0f,1.0f,1.0f);
	//w->setIsDoor(true);
	//int indexes[2] = { -1 };
	//Room* firstRoom = tile1->getRoom();
	//Room* secondRoom = tile2->getRoom();
	//for (int i = 0; i < m_rooms.size(); i++)
	//{
	//	if (*firstRoom == *m_rooms[i])
	//		indexes[0] = i;
	//	if (*secondRoom == *m_rooms[i])
	//		indexes[1] = i;
	//}
	//
	//_makeRoomConnection(indexes[0], indexes[1]);
	//XMINT2 tilePosition = { tile1->getPosX(), tile1->getPosY() };
	//XMINT2 tilePosition2 = { tile2->getPosX(), tile2->getPosY() };
	//XMINT2 doorDir = getDirection2i(tile1, tile2);
	//XMINT2 doorDirInv = { -doorDir.x, -doorDir.y };
	////std::cout << "Door position1 (" << tilePosition.x << "," << tilePosition.y << ")\n";
	////std::cout << "Door direction1 (" << doorDir.x << "," << doorDir.y << ")\n";

	////std::cout << "\nDoor position2 (" << tilePosition2.x << "," << tilePosition2.y << ")\n";
	////std::cout << "Door direction2 (" << doorDirInv.x << "," << doorDirInv.y << ")\n";
	//firstRoom->addAdjasentRoomDoor(secondRoom, tilePosition, doorDir);
	//secondRoom->addAdjasentRoomDoor(firstRoom, tilePosition2, doorDirInv);

	//w->setMesh(this->m_doorMesh);
	//_printRoomConnections();

}

void RoomCtrl::CreateMainDoor(Tile * tile1, Tile * tile2)
{
	//Direction dir = this->getDirection(tile1, tile2);

	//Wall* w = tile1->getTileWalls(dir);
	//if (!w)
	//	return;
	//w->setScale(1.0f, 1.0f, 1.0f);
	//w->setIsDoor(true);
	////tile1->setWallSpotPopulated(dir, true);
	//m_entrance = w->getTile()->getRoom();
	//
	//m_entrance->addAdjasentRoomDoor(nullptr, { tile2->getPosX(), tile2->getPosY() }, getDirection2i(tile2, tile1));
	//w->setMesh(this->m_doorMesh);
}

std::vector<int> RoomCtrl::roomTraversal(Tile * roomTile1, Tile * roomTile2)
{
	m_tempPath.clear();
	/*int indexes[2] = { -1 };
	for (int i = 0; i < m_rooms.size(); i++)
	{
		if (*m_rooms[i] == *roomTile1->getRoom())
			indexes[0] = i;
		if (*m_rooms[i] == *roomTile2->getRoom())
			indexes[1] = i;
	}

	_dijkstra(indexes[0], indexes[1]);
		*/
	return m_tempPath;
}

Room * RoomCtrl::getMainRoom() const
{
	return m_entrance;
}


//XMINT2 RoomCtrl::getMainDoorPosEnter() const
//{
//	return m_entrance->getConnectingRoomDoorPositionPartOne(nullptr);
//}
//
//XMINT2 RoomCtrl::getMainDoorPosLeave() const
//{
//	return m_entrance->getConnectingRoomDoorPositionPartTwo(nullptr);
//}
//
//XMINT2 RoomCtrl::getRoomEnterPos(Room * startRoom, int roomDstIndex)
//{
//	return startRoom->getConnectingRoomDoorPositionPartOne(m_rooms[roomDstIndex]);
//}
//
//XMINT2 RoomCtrl::getRoomLeavePos(Room * startRoom, int roomDstIndex)
//{
//	return startRoom->getConnectingRoomDoorPositionPartTwo(m_rooms[roomDstIndex]);
//}

Room * RoomCtrl::getRoomAt(int index)
{
	return m_rooms[index];
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



