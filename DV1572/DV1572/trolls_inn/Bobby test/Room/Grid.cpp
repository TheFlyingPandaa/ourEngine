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

Grid::Grid(int posX, int posY, int sizeX, int sizeY, Mesh * mesh)
{
	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;
	
	
	this->m_tileMesh = mesh;
	this->m_gridMesh = new Mesh();

	m_gridMesh->MakeRectangle();
	m_gridMesh->setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	m_gridMesh->setNormalTexture("trolls_inn/Resources/GrassNormal.png");

	m_wholeGrid.setMesh(m_gridMesh);
	m_wholeGrid.setScale(sizeX *2.0f);
	m_wholeGrid.setPos(posX, -0.01f, posX);
	m_wholeGrid.setRotation(90.0f, 0.0f, 0.0f);
	m_wholeGrid.setGridScale(sizeX);

	this->m_tiles = std::vector<std::vector<Tile*>>(this->m_sizeX);
	for (int i = 0; i < sizeX; i++)
	{
		this->m_tiles[i] = std::vector<Tile*>(this->m_sizeY);
		for (int j = 0; j < sizeY; j++)
		{
			Tile* t = new Tile(sizeX, sizeY, m_tileMesh);
			t->getQuad().setScale(2.0f);
			t->getQuad().setPos(static_cast<float>(i + posX), 0, static_cast<float>(j + posY));
			t->setPosX(i);
			t->setPosY(j);
			this->m_tiles[i][j] = t;
		}
	}
	int leftCounter = 0;

	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			int upright = 0;
			int upleft = 0;
			int downright = 0;
			int downleft = 0;

			for (int dirIndex = Direction::up; dirIndex != Direction::noneSpecial; dirIndex++)
			{
				
				Direction dir = static_cast<Direction>(dirIndex);

				if (dir == Direction::up) {
					if (j + 1 < sizeY) {
						m_tiles[i][j]->setAdjacent(m_tiles[i][j + 1], dir);
						upright++;
						upleft++;
					}					
				}
				else if (dir == Direction::down) {
					if (j - 1 >= 0) {
						m_tiles[i][j]->setAdjacent(m_tiles[i][j - 1], dir);
						downright++;
						downleft++;
					}
				}
				else if (dir == Direction::left) {
					if (i - 1 >= 0) {
						m_tiles[i][j]->setAdjacent(m_tiles[i - 1][j], dir);
						upleft++;
						downleft++;
					}
				}
				else if (dir == Direction::right) {
					if (i + 1 < sizeX) {
						m_tiles[i][j]->setAdjacent(m_tiles[i + 1][j], dir);
						upright++;
						downright++;
					}
				}

				if (upright == 2)
				{
					m_tiles[i][j]->setAdjacent(m_tiles[i + 1][j + 1], static_cast<Direction>(4));
					upright = 0;
				}
				if (upleft == 2)
				{
					m_tiles[i][j]->setAdjacent(m_tiles[i - 1][j + 1], static_cast<Direction>(5));
					upleft = 0;
				}
				if (downright == 2)
				{
					m_tiles[i][j]->setAdjacent(m_tiles[i + 1][j - 1], static_cast<Direction>(6));
					downright = 0;
				}
				if (downleft == 2)
				{
					m_tiles[i][j]->setAdjacent(m_tiles[i - 1][j - 1], static_cast<Direction>(7));
					downleft = 0;
				}
			}
		}
	}

}
Grid::~Grid()
{	
	for (int i = 0; i < this->m_tiles.size(); i++)
	{
		for (int j = 0; j < this->m_tiles[i].size(); j++)
		{
			delete this->m_tiles[i][j];
		}
	}
	delete m_gridMesh;
}


std::vector<std::vector<Tile*>> Grid::getGrid() const
{
	return m_tiles;	
}



void Grid::AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, bool force)
{
	if (pos.x + size.x <= m_sizeX && pos.y + size.y <= m_sizeY && pos.x >= 0 && pos.y >= 0) {

		std::vector<std::vector<Tile*>> tiles(size.x);
		for (int x = pos.x; x < size.x + pos.x; x++)
		{
			tiles[x - pos.x] = std::vector<Tile*>(size.y);
			for (int y = pos.y; y < size.y + pos.y; y++)
			{
				tiles[x - pos.x][y - pos.y] = this->m_tiles[x][y];
			}
		}

		m_roomCtrl.AddRoom(pos, size, roomType, tiles, force);
	}
}
void Grid::Draw()
{
	for (int i = 0; i < m_sizeX; i++)
	{
		for (int j = 0; j < m_sizeY; j++)
		{
			
			if(m_tiles[i][j]->getQuad().getColor().x != 1.0f || m_tiles[i][j]->getIsInside())
				m_tiles[i][j]->getQuad().Draw();
		}
	}
	m_wholeGrid.Draw();

	m_roomCtrl.Draw();
}

void Grid::PickTiles(Shape* selectionTile)
{
	if (selectionTile)
	{
		int xPos = selectionTile->getPosition().x;
		int yPos = selectionTile->getPosition().z;
		for(int x = -5; x < 5; x++)
			for (int y = -5; y < 5; y++)
			{
				int indexX = xPos + x;
				int indexY = yPos + y;
				
				// Boundcheck
				if (indexX < 0 || indexX >= m_sizeX || indexY < 0 || indexY >= m_sizeY) continue;

				m_tiles[indexX][indexY]->getQuad().CheckPick();
			}
		for (int i = 0; i < m_tiles.size(); i++)
		{
			for (int j = 0; j < m_tiles[i].size(); j++)
			{
				m_tiles[i][j]->getQuad().setColor(1.0f, 1.0f, 1.0f);
			
			}
		}

		
	}
	else
	{
		for (int i = 0; i < m_tiles.size(); i++)
		{
			for (int j = 0; j < m_tiles[i].size(); j++)
			{
				m_tiles[i][j]->getQuad().setColor(1.0f, 1.0f, 1.0f);
				m_tiles[i][j]->getQuad().CheckPick();
			}
		}
	}
	
}
bool Grid::CheckAndMarkTiles(DirectX::XMINT2 start, DirectX::XMINT2 end)
{
	DirectX::XMFLOAT3 color(0.5f, 5.0f, 0.5f);
		
	if (end.x < start.x)
	{
		std::swap(end.x, start.x);
	}
	if (end.y < start.y)
	{
		std::swap(end.y, start.y);
	}
	
	DirectX::XMINT2 roomOffset = end;


	roomOffset.x -= start.x - 1;
	roomOffset.y -= start.y - 1;
	bool placeable = m_roomCtrl.isPlaceable(start, roomOffset);
	if (!placeable)
		color = XMFLOAT3(5.0f, 0.5f, 0.5f);

	

	for (int i = start.x; i < end.x + 1; i++)
	{
		for (int j = start.y; j < end.y + 1; j++)
		{
			m_tiles[i][j]->getQuad().setColor(color.x, color.y, color.z);
		}
	}
	return placeable;
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
			m_tiles[i][j]->getQuad().setColor(1.0f, 1.0f, 1.0f);
		}
	}
}

void Grid::DrawString()
{
	
}

RoomCtrl & Grid::getRoomCtrl()
{
	return m_roomCtrl;
}

void Grid::Update(Camera * cam) {
	m_roomCtrl.Update(cam);
}

void Grid::CreateWalls(Mesh * mesh)
{
	if (mesh)
		m_roomCtrl.setMesh(mesh);
	//m_roomCtrl.CreateWalls();
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

	std::vector<std::shared_ptr<Node>> path;
	// Outside -> OutSide
	if (0 == startTile->getIsInside() && 0 == endTile->getIsInside())
	{
		path = findPath(startTile, endTile, true);
	} 
	// Outside -> inside
	else if (0 == startTile->getIsInside() && 1 == endTile->getIsInside())
	{
		if (0 == endTile->getRoom()->hasConnectedRooms()) 
			return std::vector<std::shared_ptr<Node>>();

		// Now do we wanna walk to the entrance
		path = findPath(startTile, m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], true);
		
		//Lets not talk about this one(This is so we walk straight through the door...)
		auto walkThroughDoor = findPath(m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], false);
		path.insert(path.end(), walkThroughDoor.begin(), walkThroughDoor.end());

		// Easy check, if its the main room we are wanting then we only need to perform pathfinding inside that room
		// and not start the higher level room path finding
		if (*endTile->getRoom() == *m_roomCtrl.getMainRoom())
		{
			auto doorToEndTile = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], endTile, false);
			path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
		}
		// We want to be advanced :P
		else
		{
			std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], endTile);

			Room* startRoom = m_roomCtrl.getMainRoom();
			roomIndexes.erase(roomIndexes.begin());
			XMINT2 DoorLeavePos;
			XMINT2 startPosition = m_roomCtrl.getMainDoorPosLeave();
			for (auto index : roomIndexes)
			{
				// Between this rooms leave door and other rooms enter door
				XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
				auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
				path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

				// Smooth Entering
				DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
				auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x ][DoorLeavePos.y], false);
				path.insert(path.end(), dontAsk.begin(), dontAsk.end());
				startRoom = m_roomCtrl.getRoomAt(index);
				startPosition = DoorLeavePos;

			}
			

			auto toTarget = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y],endTile , false);
			path.insert(path.end(), toTarget.begin(), toTarget.end());

		}

	}
	// Inside -> Inside
	else if (1 == startTile->getIsInside() && 1 == endTile->getIsInside())
	{
		if (0 == endTile->getRoom()->hasConnectedRooms())
			return std::vector<std::shared_ptr<Node>>();

		if (*startTile->getRoom() == *endTile->getRoom())
		{
			auto doorToEndTile = findPath(startTile, endTile, false);
			path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
		}
		else
		{
			std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(startTile, endTile);

			Room* startRoom = startTile->getRoom();
			roomIndexes.erase(roomIndexes.begin());
			XMINT2 DoorLeavePos;
			XMINT2 startPosition = { startTile->getPosX(), startTile->getPosY() };
			for (auto index : roomIndexes)
			{
				// Between this rooms leave door and other rooms enter door
				XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
				auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
				path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

				// Smooth Entering
				DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
				auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x][DoorLeavePos.y], false);
				path.insert(path.end(), dontAsk.begin(), dontAsk.end());
				startRoom = m_roomCtrl.getRoomAt(index);
				startPosition = DoorLeavePos;

			}
			auto toTarget = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y], endTile, false);
			path.insert(path.end(), toTarget.begin(), toTarget.end());
		}
		
	}
	//Inside to outside
	else
	{
		if (*startTile->getRoom() == *m_roomCtrl.getMainRoom())
		{
			// Path to door
			auto toDoor = findPath(startTile, m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], false);
			path.insert(path.end(), toDoor.begin(), toDoor.end());

			//Lets not talk about this one(This is so we walk straight through the door...)
			auto walkThroughDoor = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y],m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], false);
			path.insert(path.end(), walkThroughDoor.begin(), walkThroughDoor.end());

			auto walkToTarget = findPath(m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], endTile, true);
			path.insert(path.end(), walkToTarget.begin(), walkToTarget.end());
		}
		else
		{
			// We need to find the entrace from inside then we path to the destination
			std::vector<int> roomIndexes = m_roomCtrl.roomTraversal(startTile, m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y]);

			Room* startRoom = startTile->getRoom();
			roomIndexes.erase(roomIndexes.begin());
			XMINT2 DoorLeavePos;
			XMINT2 startPosition = { startTile->getPosX(), startTile->getPosY() };
			for (auto index : roomIndexes)
			{
				// Between this rooms leave door and other rooms enter door
				XMINT2 DoorEnterPos = m_roomCtrl.getRoomEnterPos(startRoom, index);
				auto toOtherRoom = findPath(m_tiles[startPosition.x][startPosition.y], m_tiles[DoorEnterPos.x][DoorEnterPos.y], false);
				path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

				// Smooth Entering
				DoorLeavePos = m_roomCtrl.getRoomLeavePos(startRoom, index);
				auto dontAsk = findPath(m_tiles[DoorEnterPos.x][DoorEnterPos.y], m_tiles[DoorLeavePos.x][DoorLeavePos.y], false);
				path.insert(path.end(), dontAsk.begin(), dontAsk.end());

				startRoom = m_roomCtrl.getRoomAt(index);
				startPosition = DoorLeavePos;

			}
			auto toMainLeave = findPath(m_tiles[DoorLeavePos.x][DoorLeavePos.y], m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], false);
			path.insert(path.end(), toMainLeave.begin(), toMainLeave.end());

			auto smooth = findPath(m_tiles[m_roomCtrl.getMainDoorPosLeave().x][m_roomCtrl.getMainDoorPosLeave().y], m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y], false);
			path.insert(path.end(), smooth.begin(), smooth.end());

			auto finallyTarget = findPath(m_tiles[m_roomCtrl.getMainDoorPosEnter().x][m_roomCtrl.getMainDoorPosEnter().y],endTile ,true);
			path.insert(path.end(), finallyTarget.begin(), finallyTarget.end());

		}

		
		
	}
	return path;
}

std::vector<std::shared_ptr<Node>> Grid::findPath(Tile* startTile, Tile* endTile, bool outside)
{
	std::vector<std::shared_ptr<Node>> openList;
	std::vector<std::shared_ptr<Node>> closedList;

	std::shared_ptr<Node> current(new Node(startTile, nullptr, 0, getDistance(startTile, endTile)));
	
	openList.push_back(current);

	while (openList.size() > 0)
	{
		std::sort(openList.begin(), openList.end(), [](std::shared_ptr<Node> a1, std::shared_ptr<Node> a2) {return a1->fCost < a2->fCost; });
		current = openList.at(0);

		if (*current == *endTile)
		{
			std::vector<std::shared_ptr<Node>> path;
			while (current->parent != nullptr)
			{
				path.push_back(current);
				std::shared_ptr<Node> t(current->parent);
				current = t;
				
			}

			std::reverse(path.begin(), path.end());
			return path;
		}
		
		closedList.push_back(current);		// add the entry to the closed list
		openList.erase(openList.begin());   // Remove the entry

		for (int dirIndex = Direction::up; dirIndex != Direction::noneSpecial; dirIndex++)
		{
			
			Direction dir = static_cast<Direction>(dirIndex);

			Tile* currentTile = current->tile->getAdjacent(dir);
			
			// Rules here

			if (currentTile == nullptr)
				continue;
			if (current->tile->getIsInside() && outside)
				continue;
			if (!currentTile->getIsWalkeble())
				continue;
			
			if (outside)
			{
				if (dir == Direction::downleft)
				{
					if (current->tile->getAdjacent(left)->getRoom() != nullptr)
						continue;
					if (current->tile->getAdjacent(down)->getRoom() != nullptr)
						continue;
				}
				else if (dir == Direction::downright)
				{
					if (current->tile->getAdjacent(right)->getRoom() != nullptr)
						continue;
					if (current->tile->getAdjacent(down)->getRoom() != nullptr)
						continue;
				}
				else if (dir == Direction::upright)
				{
					if (current->tile->getAdjacent(up)->getRoom() != nullptr)
						continue;
					if (current->tile->getAdjacent(right)->getRoom() != nullptr)
						continue;
				}
				else if (dir == Direction::upleft)
				{
					if (current->tile->getAdjacent(up)->getRoom() != nullptr)
						continue;
					if (current->tile->getAdjacent(left)->getRoom() != nullptr)
						continue;
				}
			}
			


			//--Rules End Here--

			float gCost = current->gCost + (getDistance(current->tile, currentTile) == 1 ? 1 : 0.95f);

			float hCost = getDistance(currentTile, endTile);
			std::shared_ptr<Node> newNode (new Node(currentTile, current, gCost, hCost));
			//pointerBank.push_back(newNode);

			if (_findInVec(closedList,newNode) && gCost >= newNode->gCost)
				continue;
			if (!_findInVec(openList, newNode) || gCost < newNode->gCost)
			{
				openList.push_back(newNode);
				
			}
			
			
		}
		
		
	}
	
	return std::vector<std::shared_ptr<Node>>();
}

Tile* Grid::getTile(int x, int y) const
{
	if (x < 0 || x > m_sizeX || y < 0 || y >m_sizeY)
		return nullptr;
	return m_tiles[x][y];
}

