#include "Grid.h"
#include <math.h>
#include <memory>

bool Grid::_findInVec(std::vector<Node*>& list, Node * node) const
{
	for (auto& cur : list)
		if (*cur == *node)
			return true;
	return false;
}
bool Grid::_findInVec(std::vector<std::shared_ptr<Node>>& list, std::shared_ptr<Node> node) const
{
	for (auto& cur : list)
		if (*cur == *node)
			return true;
	return false;
}

int Grid::_index(int x, int y) const
{
	return x + (y * m_sizeY);
}


Grid::Grid(int posX, int posY, int sizeX, int sizeY, Mesh * mesh)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;
	
	this->m_gridMesh = new Mesh();
	m_tileMesh = new Mesh();

	m_gridMesh->MakeRectangle();
	m_gridMesh->setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	m_gridMesh->setNormalTexture("trolls_inn/Resources/GrassNormal.png");

	m_tileMesh->MakeRectangle();
	m_tileMesh->setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	m_tileMesh->setNormalTexture("trolls_inn/Resources/GrassNormal.png");
	

	m_wholeGrid.setMesh(m_gridMesh);
	m_wholeGrid.setScale(sizeX *2.0f);
	m_wholeGrid.setPos(static_cast<float>(posX), -0.01f, static_cast<float>(posY));
	m_wholeGrid.setRotation(90.0f, 0.0f, 0.0f);
	m_wholeGrid.setGridScale(sizeX);

	this->m_tiles.reserve(sizeX * sizeY);

	for (int i = 0; i < sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			Tile* t = new Tile(sizeX, sizeY, m_tileMesh);

			t->getQuad().setScale(2.0f);
			t->getQuad().setPos(static_cast<float>(j + posX), 0, static_cast<float>(i + posY));
			t->getQuad().setGridScale(1);

			this->m_tiles.push_back(t);
		}
	}
}
Grid::~Grid()
{
	for (auto& tile : m_tiles)
		delete tile;

	delete m_gridMesh;
	delete m_tileMesh;
}


std::vector<Tile*> Grid::getGrid() const
{
	return m_tiles;	
}

//bool Grid::CheckIfDoorCanBeBuilt(DirectX::XMINT2 pos, DirectX::XMINT2 pos2)
//{
//	// TODO
//
//	/*bool result = false;
//	DirectX::XMFLOAT3 c(5.0f, 0.5f, 0.5f);
//	Tile* t1 = m_tiles[pos.x][pos.y];
//	Tile* t2 = m_tiles[pos2.x][pos2.y];
//	if ((abs(pos.x - pos2.x) == 1 || abs(pos.y - pos2.y) == 1) &&
//		(pos.x == pos2.x || pos.y == pos2.y))
//	{
//		if (t1->getIsInside() && t2->getIsInside())
//		{
//			if (t1 != t2)
//			{
//				Direction dir = m_roomCtrl.getDirection(t1, t2);
//
//				Wall* w = t1->getTileWalls(dir);
//				if (w)
//				{
//					c = DirectX::XMFLOAT3(0.05f, 5.0f, 0.5f);
//				}
//			}
//		}
//	}
//	
//	t1->getQuad().setColor(c.x, c.y, c.z);
//	t2->getQuad().setColor(c.x, c.y, c.z);
//
//	return result;*/
//}


//void Grid::AddRoomObject(DirectX::XMINT2 pos, Mesh * mesh)
//{
//	// TODO
//	/*m_tiles[pos.x][pos.y]->setIsWalkeble(false);
//	m_tiles[pos.x][pos.y]->setHasObject(true);
//
//	m_roomCtrl.addRoomObject(pos, mesh);*/
//}

std::vector<Tile*> Grid::extractTiles(DirectX::XMINT2 pos, DirectX::XMINT2 size)
{
	std::vector<Tile*> extractedTiles;
	extractedTiles.reserve(size.x * size.y);

	for (int _y = 0; _y < size.y; _y++)
	{
		for (int _x = 0; _x < size.x; _x++)
		{
			Tile* tile = m_tiles[_index(_x + pos.x, _y + pos.y)];

			if (tile != nullptr)
			{
				extractedTiles.push_back(tile);
				m_tiles[_index(_x + pos.x, _y + pos.y)] = nullptr;
			}

		}
	}

	return extractedTiles;
}

void Grid::insertTiles(DirectX::XMINT2 pos, DirectX::XMINT2 size, std::vector<Tile*>& tiles)
{
	int counter = 0;
	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			m_tiles[_index(x + pos.x, y + pos.y)] = tiles[counter++];
			m_tiles[_index(x + pos.x, y + pos.y)]->setMesh(m_tileMesh);
			m_tiles[_index(x + pos.x, y + pos.y)]->getQuad().setColor(1.0f, 1.0f, 1.0f);
		}
	}
}

void Grid::Draw()
{
	for (auto& tile : m_tiles)
	{
		if (tile != nullptr)
		{
			if (tile->getQuad().getColor().x != 1.0f)
				tile->getQuad().Draw();

		}
	}
	
	m_wholeGrid.Draw();

}

void Grid::PickTiles(Shape* selectionTile)
{
	bool notFound = false;
	if (selectionTile)
	{
		int xPos = static_cast<int>(selectionTile->getPosition().x);
		int yPos = static_cast<int>(selectionTile->getPosition().z);
		for (int x = -5; x < 5; x++)
		{

			for (int y = -5; y < 5; y++)
			{
				int indexX = xPos + x;
				int indexY = yPos + y;

				// Boundcheck
				if (indexX < 0 || indexX >= m_sizeX || indexY < 0 || indexY >= m_sizeY) continue;
				Tile* t = m_tiles[_index(indexX, indexY)];
				if(t)t->getQuad().CheckPick();
			}
			
		}

		for (auto& t : m_tiles)
			if (t)
				t->getQuad().setColor(1.0f, 1.0f, 1.0f);

	}
	else
	{
		for (int i = 0; i < m_tiles.size(); i++)
		{
			if (m_tiles[i] != nullptr)
			{
				m_tiles[i]->getQuad().setColor(1.0f, 1.0f, 1.0f);
				m_tiles[i]->getQuad().CheckPick();
			}
		}
	}
	
	
}
bool Grid::CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end)
{
	DirectX::XMFLOAT3 color(0.5f, 500.0f, 0.5f);

	if (end.x < start.x)
	{
		std::swap(end.x, start.x);
	}
	if (end.y < start.y)
	{
		std::swap(end.y, start.y);
	}
	

	

	bool result = true;
	for (int i = start.x; i < end.x + 1; i++)
	{
		for (int j = start.y; j < end.y + 1; j++)
		{
			Tile* t = m_tiles[_index(i, j)];
			if (!t)
			{
				color = XMFLOAT3(5.0f, 0.5f, 0.5f);
				result = false;
			}
		}
	}
	for (int i = start.x; i < end.x + 1; i++)
	{
		for (int j = start.y; j < end.y + 1; j++)
		{
			Tile* t = m_tiles[_index(i, j)];
			if (t)
				t->getQuad().setColor(color.x, color.y, color.z);
		}
	}

	return result;
}

bool Grid::CheckAndMarkTilesObject(DirectX::XMINT2 start, int size, int angle)
{
	/*bool isFalse = false;

	if (angle == 0 || angle == 180)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (angle == 0)
			{
				if (m_tiles[start.x][start.y + i]->getHasObject() == false)
				{
					m_tiles[start.x][start.y + i]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else
				{
					m_tiles[start.x][start.y + i]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
			else
			{
				if (m_tiles[start.x][start.y - i]->getHasObject() == false)
				{
					m_tiles[start.x][start.y - i]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else
				{
					m_tiles[start.x][start.y - i]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
		}
	}
	if (angle == 90 || angle == 270)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (angle == 90)
			{
				if (m_tiles[start.x + i][start.y]->getHasObject() == false)
				{
					m_tiles[start.x + i][start.y]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else
				{
					m_tiles[start.x + i][start.y]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
			else
			{
				if (m_tiles[start.x - i][start.y]->getHasObject() == false)
				{
					m_tiles[start.x - i][start.y]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
				}
				else
				{
					m_tiles[start.x - i][start.y]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
					isFalse = true;
				}
			}
		}
	}

	if (m_tiles[start.x][start.y]->getHasObject() == true)
	{
		m_tiles[start.x][start.y]->getQuad().setColor(XMFLOAT3(5.5f, 0.5f, 0.5f));
		return false;
	}
	else
	{
		m_tiles[start.x][start.y]->getQuad().setColor(XMFLOAT3(0.5f, 5.0f, 0.5f));
		if (isFalse)
		{
			return false;
		}
		else {
			return true;
		}
	}*/
	return true;
}

void Grid::ResetTileColor(DirectX::XMINT2 pos, DirectX::XMINT2 end)
{
	if (end.x < pos.x)
	{
		std::swap(end.x, pos.x);
	}
	if (end.y < pos.y)
	{
		std::swap(end.y, pos.y);
	}
	for (int i = pos.x; i < end.x + 1; i++)
	{
		for (int j = pos.y; j < end.y + 1; j++)
		{
			Tile* t = m_tiles[_index(i, j)];
			if(t)
				t->getQuad().setColor(1.0f, 1.0f, 1.0f);
		}
	}
}

void Grid::DrawString()
{
	
}

float Grid::getDistance(Tile* t1, Tile* t2)
{
	XMVECTOR xmTile = XMLoadFloat3(&t1->getQuad().getPosition());
	XMVECTOR xmGoal = XMLoadFloat3(&t2->getQuad().getPosition());
	return XMVectorGetX(XMVector2Length(xmTile - xmGoal));
}

std::vector<std::shared_ptr<Node>> Grid::findPathHighLevel(Tile * startTile, Tile * endTile)
{
	/*
		- If start and end tile is both outside then its easy just call find path

		--- ROOM TO ROOM ---
		room by room traversal is dividing into a two part path finding. First we find the quickest
		room traversal then we perform A* star inside the rooms from door to door.

		- If start is inside and end is outside then we need to find the path to the main door
		by performing djiksta from the current room to the entrance and then A* Outside

		- If both start and end is inside then we perform djikstra first with the rooms and A* between 
		the room doors. 
		
	*/

	//std::vector<std::shared_ptr<Node>> path;
	//// Outside -> OutSide
	//if (0 == startTile->getIsInside() && 0 == endTile->getIsInside())
	//{
	//	path = findPath(startTile, endTile, true);
	//} 
	//// Outside -> inside
	//else if (0 == startTile->getIsInside() && 1 == endTile->getIsInside())
	//{
	//	if (0 == endTile->getRoom()->hasConnectedRooms()) 
	//		return std::vector<std::shared_ptr<Node>>();

	//	// Now do we wanna walk to the entrance
	//	path = findPath(startTile, m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], true);
	//	
	//	//Lets not talk about this one(This is so we walk straight through the door...)
	//	auto walkThroughDoor = findPath(m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], false);
	//	path.insert(path.end(), walkThroughDoor.begin(), walkThroughDoor.end());

	//	// Easy check, if its the main room we are wanting then we only need to perform pathfinding inside that room
	//	// and not start the higher level room path finding
	//	if (*endTile->getRoom() == *m_roomCtrl.getMainRoom())
	//	{
	//		auto doorToEndTile = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], endTile, false);
	//		path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
	//	}
	//	// We want to be advanced :P
	//	else
	//	{
	//		std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], endTile);

	//		Room* startRoom = m_roomCtrl.getMainRoom();
	//		roomIndexes.erase(roomIndexes.begin());
	//		XMINT2 DoorLeavePos;
	//		XMINT2 startPosition = m_roomCtrl.getMainDoorPosLeave();
	//		for (auto index : roomIndexes)
	//		{
	//			// Between this rooms leave door and other rooms enter door
	//			XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
	//			auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
	//			path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

	//			// Smooth Entering
	//			DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
	//			auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x ][DoorLeavePos.y], false);
	//			path.insert(path.end(), dontAsk.begin(), dontAsk.end());
	//			startRoom = m_roomCtrl.getRoomAt(index);
	//			startPosition = DoorLeavePos;

	//		}
	//		

	//		auto toTarget = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y],endTile , false);
	//		path.insert(path.end(), toTarget.begin(), toTarget.end());

	//	}

	//}
	//// Inside -> Inside
	//else if (1 == startTile->getIsInside() && 1 == endTile->getIsInside())
	//{
	//	if (0 == endTile->getRoom()->hasConnectedRooms())
	//		return std::vector<std::shared_ptr<Node>>();

	//	if (*startTile->getRoom() == *endTile->getRoom())
	//	{
	//		auto doorToEndTile = findPath(startTile, endTile, false);
	//		path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
	//	}
	//	else
	//	{
	//		std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(startTile, endTile);

	//		Room* startRoom = startTile->getRoom();
	//		roomIndexes.erase(roomIndexes.begin());
	//		XMINT2 DoorLeavePos;
	//		XMINT2 startPosition = { startTile->getPosX(), startTile->getPosY() };
	//		for (auto index : roomIndexes)
	//		{
	//			// Between this rooms leave door and other rooms enter door
	//			XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
	//			auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
	//			path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

	//			// Smooth Entering
	//			DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
	//			auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x][DoorLeavePos.y], false);
	//			path.insert(path.end(), dontAsk.begin(), dontAsk.end());
	//			startRoom = m_roomCtrl.getRoomAt(index);
	//			startPosition = DoorLeavePos;

	//		}
	//		auto toTarget = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y], endTile, false);
	//		path.insert(path.end(), toTarget.begin(), toTarget.end());
	//	}
	//	
	//}
	////Inside to outside
	//else
	//{
	//	if (*startTile->getRoom() == *m_roomCtrl.getMainRoom())
	//	{
	//		// Path to door
	//		auto toDoor = findPath(startTile, m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], false);
	//		path.insert(path.end(), toDoor.begin(), toDoor.end());

	//		//Lets not talk about this one(This is so we walk straight through the door...)
	//		auto walkThroughDoor = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y],m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], false);
	//		path.insert(path.end(), walkThroughDoor.begin(), walkThroughDoor.end());

	//		auto walkToTarget = findPath(m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], endTile, true);
	//		path.insert(path.end(), walkToTarget.begin(), walkToTarget.end());
	//	}
	//	else
	//	{
	//		// We need to find the entrace from inside then we path to the destination
	//		std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(startTile, m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y]);

	//		Room* startRoom = startTile->getRoom();
	//		roomIndexes.erase(roomIndexes.begin());
	//		XMINT2 DoorLeavePos;
	//		XMINT2 startPosition = { startTile->getPosX(), startTile->getPosY() };
	//		for (auto index : roomIndexes)
	//		{
	//			// Between this rooms leave door and other rooms enter door
	//			XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
	//			auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
	//			path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

	//			// Smooth Entering
	//			DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
	//			auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x][DoorLeavePos.y], false);
	//			path.insert(path.end(), dontAsk.begin(), dontAsk.end());

	//			startRoom = m_roomCtrl.getRoomAt(index);
	//			startPosition = DoorLeavePos;

	//		}
	//		auto toMainLeave = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y], m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], false);
	//		path.insert(path.end(), toMainLeave.begin(), toMainLeave.end());

	//		auto smooth = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], false);
	//		path.insert(path.end(), smooth.begin(), smooth.end());

	//		auto finallyTarget = findPath(m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y],endTile ,true);
	//		path.insert(path.end(), finallyTarget.begin(), finallyTarget.end());

	//	}

	//	
	//	
	//}

	return std::vector<std::shared_ptr<Node>>();
}

std::vector<std::shared_ptr<Node>> Grid::findPath(Tile* startTile, Tile* endTile, bool outside)
{
	//std::vector<std::shared_ptr<Node>> openList;
	//std::vector<std::shared_ptr<Node>> closedList;

	//std::shared_ptr<Node> current(new Node(startTile, nullptr, 0, getDistance(startTile, endTile)));
	//
	//openList.push_back(current);

	//while (openList.size() > 0)
	//{
	//	std::sort(openList.begin(), openList.end(), [](std::shared_ptr<Node> a1, std::shared_ptr<Node> a2) {return a1->fCost < a2->fCost; });
	//	current = openList.at(0);

	//	if (*current == *endTile)
	//	{
	//		std::vector<std::shared_ptr<Node>> path;
	//		while (current->parent != nullptr)
	//		{
	//			path.push_back(current);
	//			std::shared_ptr<Node> t(current->parent);
	//			current = t;
	//			
	//		}

	//		std::reverse(path.begin(), path.end());
	//		return path;
	//	}
	//	
	//	closedList.push_back(current);		// add the entry to the closed list
	//	openList.erase(openList.begin());   // Remove the entry

	//	for (int dirIndex = Direction::up; dirIndex != Direction::noneSpecial; dirIndex++)
	//	{
	//		
	//		Direction dir = static_cast<Direction>(dirIndex);

	//		Tile* currentTile = current->tile->getAdjacent(dir);
	//		
	//		// Rules here

	//		if (currentTile == nullptr)
	//			continue;
	//		if (current->tile->getIsInside() && outside)
	//			continue;
	//		if (!currentTile->getIsWalkeble())
	//			continue;
	//		
	//		if (outside)
	//		{
	//			if (dir == Direction::downleft)
	//			{
	//				if (current->tile->getAdjacent(left)->getRoom() != nullptr)
	//					continue;
	//				if (current->tile->getAdjacent(down)->getRoom() != nullptr)
	//					continue;
	//			}
	//			else if (dir == Direction::downright)
	//			{
	//				if (current->tile->getAdjacent(right)->getRoom() != nullptr)
	//					continue;
	//				if (current->tile->getAdjacent(down)->getRoom() != nullptr)
	//					continue;
	//			}
	//			else if (dir == Direction::upright)
	//			{
	//				if (current->tile->getAdjacent(up)->getRoom() != nullptr)
	//					continue;
	//				if (current->tile->getAdjacent(right)->getRoom() != nullptr)
	//					continue;
	//			}
	//			else if (dir == Direction::upleft)
	//			{
	//				if (current->tile->getAdjacent(up)->getRoom() != nullptr)
	//					continue;
	//				if (current->tile->getAdjacent(left)->getRoom() != nullptr)
	//					continue;
	//			}
	//		}
	//		


	//		//--Rules End Here--

	//		float gCost = current->gCost + (getDistance(current->tile, currentTile) == 1 ? 1 : 0.95f);

	//		float hCost = getDistance(currentTile, endTile);
	//		std::shared_ptr<Node> newNode (new Node(currentTile, current, gCost, hCost));
	//		//pointerBank.push_back(newNode);

	//		if (_findInVec(closedList,newNode) && gCost >= newNode->gCost)
	//			continue;
	//		if (!_findInVec(openList, newNode) || gCost < newNode->gCost)
	//		{
	//			openList.push_back(newNode);
	//			
	//		}
	//		
	//		
	//	}
	//	
	//	
	//}
	
	return std::vector<std::shared_ptr<Node>>();
}
