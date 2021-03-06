#include "Room.h"
#include "../../Furniture/Table.h"
#include "../../Furniture/Bed.h"
#include "../../Mesh Manager/MeshLoaderPlus.h"

Mesh Room::s_AABB;
bool Room::s_isLoaded = false;
int Room::s_index = 1;

void Room::_loadStatic()
{
	Room::s_AABB.LoadModel("trolls_inn/Resources/box.obj");
	Room::s_isLoaded = true;
}

void Room::_initAABB(int x, int y, int sx, int sy, int level)
{
	m_AABB.setMesh(&s_AABB);
	m_AABB.setPos(x + ((float)sx / 2.0f) + 0.001f, (level * 2.0f) + 1.0f, y + ((float)sy / 2.0f) - 0.001f);
	m_AABB.setScale(static_cast<float>(sx) + 0.05f, 2.1f, static_cast<float>(sy) + 0.05f);
}

void Room::_createLight(int x, int y, int sx, int sy, int level)
{
	PointLight* l = new PointLight();
	l->setPosition(static_cast<float>(x) + ((float)sx / 2.0f), 2.0f, static_cast<float>(y) + ((float)sy / 2.0f));
	//l->setColor((rand() % 11) * 0.1f, (rand() % 11) * 0.1f, (rand() % 11) * 0.1f);
	l->setColor(0.75f, 0.75f, 0.75f);
	//float range = std::sqrt(std::pow(static_cast<float>(sx), 2) + std::pow(static_cast<float>(sy), 2));
	l->setSettingsForLight(5.0f, 0.8f);
	l->setIndex(m_index);
	m_lights.push_back(l);
}


bool Room::_findInVec(std::vector<std::shared_ptr<Node>>& list, std::shared_ptr<Node> node) const
{
	for (auto& cur : list)
		if (*cur == *node)
			return true;
	return false;
}

int Room::_index(int x, int y)
{
	int xCoord = (x - m_posX); if (xCoord < 0 || xCoord >= m_sizeX) return -1;
	int yCoord = (y - m_posY); if (yCoord < 0 || yCoord >= m_sizeY) return -1;
	return(xCoord + yCoord * m_sizeX);
}

Room::Room(int posX, int posY, int sizeX, int sizeY, std::vector<Tile*> tiles, RoomType roomType)
{
	if (!s_isLoaded)
		_loadStatic();
	m_index = s_index++;
	_initAABB(posX, posY, sizeX, sizeY);
	_createLight(posX, posY, sizeX, sizeY);
	m_selected = false;

	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;

	this->m_roomTiles = tiles;
	for (auto *t : m_roomTiles)
	{
		t->getQuad().setLightIndex(m_index);
	}
	m_wholeFloor.setLightIndex(m_index);
	m_wholeFloor.setPos(static_cast<float>(posX), -0.001f, static_cast<float>(posY));
	m_wholeFloor.setScale(sizeX * 2.0f, 1, sizeY*2.0f);
	m_wholeFloor.setRotation(90.0f, 0.0f, 0.0f);
	
	m_wholeFloor.setUVScaleX(m_sizeX);
	m_wholeFloor.setUVScaleY(m_sizeY);
	

	m_roomType = roomType;

}

Room::~Room()
{
	for (auto& light : m_lights) {
		delete light;
		light = nullptr;
	}
	for (auto& walls : m_allWalls)
		delete walls;
	for (auto& tile : m_roomTiles)
		delete tile;
	for (auto& element : m_roomObjects)
		delete element;
}

bool Room::Inside(int x, int y)
{
	return	x >= m_posX && x < m_posX + m_sizeX &&
			y >= m_posY && y < m_posY + m_sizeY;
}

bool Room::Inside(Tile * t)
{
	return	t->getPosX() >= m_posX && t->getPosY() < m_posX + m_sizeX &&
			t->getPosY() >= m_posY && t->getPosY() < m_posY + m_sizeY;
}

void Room::Update(Camera * cam)
{
	XMFLOAT3 test = { cam->getLookAt().x , 0.0f , cam->getLookAt().z };
	XMVECTOR xmCamDir = XMLoadFloat3(&test);
	xmCamDir = XMVector3Normalize(xmCamDir);
	XMVECTOR xmCamPos = XMLoadFloat3(&cam->getPosition());

	for (auto& wall : m_allWalls)
	{

		XMVECTOR wallPosition = XMLoadFloat3(&getPosition());
		
		float distance = XMVectorGetX(XMVector3Length(wallPosition - xmCamPos));
		if (wall->isShared() && false == wall->getIsDoor() && m_isBuildingDoor == false)
		{
			wall->setScale(1, 0.05f, 1.0f);
		}
		else
		{
			XMFLOAT3 wallDir = { wall->getDirection().x, 0, wall->getDirection().y };
			XMVECTOR xmWallDir = XMLoadFloat3(&wallDir);
			float angleDegrees = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(xmCamDir, xmWallDir)));
			
			if (angleDegrees < 90 && false == wall->getIsDoor() && m_isBuildingDoor == false)
			{
				float funcy = (2.0f * (angleDegrees / 90) ) - 1.0f;
				wall->setScale(1, funcy < 0.05f ? 0.05f : funcy, 1.0f);
			}
			else
				wall->setScale(1, 1.0f, 1.0f);
				
		}
	}
}

void Room::Draw()
{
	m_wholeFloor.Draw();
	
	
	for (auto& fur : m_roomObjects)
		fur->Draw();

	
	for (auto& tile : m_roomTiles)
	{
		if (tile->getQuad().getColor().x != 1.0f)
			tile->getQuad().Draw();
	}

	if (MLP::GetInstance().IsReady(MESH::WALL))
	{
		for (auto& wall : m_allWalls)
		{
			if (wall)
			{
				wall->Draw();
			}
		}
	}
	

	for (auto &l : m_lights)
	{
		l->Draw();
	}
	
}

std::string Room::toString() const
{
	return "meh"; // WHO DID THIS 
}

int Room::getRoomIndex() const
{
	return m_index;
}

void Room::ApplyIndexOnMesh()
{
	for (Wall* w : m_allWalls)
	{
		w->getObject3D().setLightIndex(m_index);
	}
	for(auto& tile : m_roomTiles)
			tile->getQuad().setLightIndex(m_index);
}

void Room::CastShadow()
{
	m_AABB.CastShadow();


	/*Move To Draw*/
	if (m_selected)
		m_AABB.TEMPTRANS();
}

void Room::setIsBuildingDoor(bool tje)
{
	m_isBuildingDoor = tje;
}

bool Room::CheckRoomComplete()
{
	if (m_roomType == RoomType::bedroom)
	{
		for (auto object : m_roomObjects)
		{
			if (object->WhatType() == "Bed")
			{
				return true;
			}
		}
	}
	else if(m_roomType == RoomType::kitchen)
	{
		for (auto object : m_roomObjects)
		{
			if(object->WhatType() == "Stove")
			{
				return true;
			}
			
		}
	}
	else if(m_roomType == RoomType::bar)
	{
		for (auto object : m_roomObjects)
		{
			if (object->WhatType() == "Bar")
			{
				return true;
			}

		}
	}
	else
	{
		return true;
	}

	return false;
}

float Room::getDistance(Tile * t1, Tile * t2)
{
	XMVECTOR xmTile = XMLoadFloat3(&t1->getQuad().getPosition());
	XMVECTOR xmGoal = XMLoadFloat3(&t2->getQuad().getPosition());
	return XMVectorGetX(XMVector2Length(xmTile - xmGoal));
}

std::vector<std::shared_ptr<Node>> Room::findPath(Tile * startTile, Tile * endTile)
{
	auto getAdjacentTile = [&](std::shared_ptr<Node> current, float dx, float dy) -> Tile*
	{
		int index = _index(static_cast<int>(current->tile->getQuad().getPosition().x + dx), static_cast<int>(current->tile->getQuad().getPosition().z + dy));
		if (index < 0 || index >= m_roomTiles.size()) return nullptr;;
		return m_roomTiles[index];
	};
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

		for (int dirIndex = Direction::up; dirIndex != Direction::upright; dirIndex++)
		{

			Direction dir = static_cast<Direction>(dirIndex);
			float addedCost = (dirIndex > 3) ? 1.414f : 1.0f;
			XMFLOAT2 dirFloat;
			switch (dir)
			{
			case up:
				dirFloat = XMFLOAT2(0, 1);
				break;
			case down:
				dirFloat = XMFLOAT2(0, -1);
				break;
			case left:
				dirFloat = XMFLOAT2(-1, 0);
				break;
			case right:
				dirFloat = XMFLOAT2(1, 0);
				break;
			case upright:
				dirFloat = XMFLOAT2(1, 1);
				break;
			case upleft:
				dirFloat = XMFLOAT2(-1, 1);
				break;
			case downright:
				dirFloat = XMFLOAT2(1, -1);
				break;
			case downleft:
				dirFloat = XMFLOAT2(-1, -1);
				break;
			}

			int index = _index(static_cast<int>(current->tile->getQuad().getPosition().x + dirFloat.x), static_cast<int>(current->tile->getQuad().getPosition().z + dirFloat.y));
			if (index < 0 || index >= m_roomTiles.size()) 
				continue;
			Tile* currentTile = m_roomTiles[index];

			// Rules here

			if (currentTile == nullptr)
				continue;
			bool shouldContinue = false;
			/*for (auto& object : m_roomObjects)
			{
				if (currentTile->getPosition().x == object->getPosition().x
					&& currentTile->getPosition().y == object->getPosition().z)
					shouldContinue = true;

			}*/

			if (shouldContinue) continue;

			if (dir == Direction::downleft)
			{
				Tile* leftTile = getAdjacentTile(current, -1, 0);
				if (leftTile == nullptr) continue;

				Tile* downTile = getAdjacentTile(current,0, -1);
				if (downTile == nullptr) continue;
			}
			else if (dir == Direction::downright)
			{
				Tile* rightTile = getAdjacentTile(current,1,0);
				if (rightTile == nullptr) continue;

				Tile* downTile = getAdjacentTile(current,0,-1);
				if (downTile == nullptr) continue;
			}
			else if (dir == Direction::upright)
			{
				
				Tile* upTile = getAdjacentTile(current,0,1);
				if (upTile == nullptr) continue;

				Tile* rightTile = getAdjacentTile(current,1,0);
				if (rightTile == nullptr) continue;
			}
			else if (dir == Direction::upleft)
			{
				
				Tile* upTile = getAdjacentTile(current, 0, 1);
				if (upTile == nullptr) continue;

				Tile* leftTile = getAdjacentTile(current, -1, 0);
				if (leftTile == nullptr) continue;
			}

			//--Rules End Here--

			float gCost = current->gCost + addedCost;

			float hCost = getDistance(currentTile, endTile);
			std::shared_ptr<Node> newNode(new Node(currentTile, current, gCost, hCost));
			//pointerBank.push_back(newNode);

			if (_findInVec(closedList, newNode) && gCost >= newNode->gCost)
				continue;
			if (!_findInVec(openList, newNode) || gCost < newNode->gCost)
			{
				openList.push_back(newNode);

			}


		}


	}

	return std::vector<std::shared_ptr<Node>>();
}

bool Room::operator==(const Room & other) const
{
	return m_posX == other.m_posX && m_posY == other.m_posY;
}

Tile * Room::getTile(int x, int y)
{
	return m_roomTiles[_index(x,y)];
}

void Room::setFloorMesh(Mesh * mesh)
{
	m_wholeFloor.setMesh(mesh);
	for (auto& tile : m_roomTiles)
		tile->getQuad().setMesh(mesh);
}

void Room::CreateWallSide(Mesh* mesh, std::vector<bool> allowed, Direction side)
{
	std::vector<Wall*> currentWalls(m_allWalls);
	if (side == up)
	{
		for (int i = 0; i < m_sizeX; ++i)
		{
			if (allowed[i])
			{
				XMFLOAT2 newPos(static_cast<float>(m_posX) + 0.5f + i, static_cast<float>(m_posY + m_sizeY));
				bool canBuild = true;
				for (auto& cw : currentWalls)
				{
					XMFLOAT2 currentWallPos(cw->getObject3D().getPosition().x, cw->getObject3D().getPosition().z);
					if (newPos.x == currentWallPos.x && currentWallPos.y == newPos.y)
					{
						canBuild = false;
						break;
					}
				}
				if (canBuild)
				{
					Wall* newWallUp = new Wall(mesh, { 0, -1 });
					newWallUp->setPosition(static_cast<float>(m_posX) + 0.5f + i, static_cast<float>(m_posY + m_sizeY));
					newWallUp->getObject3D().setLightIndex(m_index);
					m_allWalls.push_back(newWallUp);
					m_upWalls.push_back(newWallUp);
				}
			}
				
		}
	}
	else if (side == down)
	{
		for (int i = 0; i < m_sizeX; ++i)
		{
			if (allowed[i])
			{
				XMFLOAT2 newPos(static_cast<float>(m_posX) + 0.5f + i, static_cast<float>(m_posY));
				bool canBuild = true;
				for (auto& cw : currentWalls)
				{
					XMFLOAT2 currentWallPos(cw->getObject3D().getPosition().x, cw->getObject3D().getPosition().z);
					if (newPos.x == currentWallPos.x && currentWallPos.y == newPos.y)
					{
						canBuild = false;
					}
				}
				if (canBuild)
				{
					Wall* newWallLow = new Wall(mesh, { 0,1 });
					newWallLow->setPosition(newPos);
					newWallLow->getObject3D().setLightIndex(m_index);
					m_allWalls.push_back(newWallLow);
					m_downWalls.push_back(newWallLow);
				}
				
			}
		}
	}
	else if (side == left)
	{
		for (int i = 0; i < m_sizeY; ++i)
		{
			if (allowed[i])
			{
				XMFLOAT2 newPos(static_cast<float>(m_posX), static_cast<float>(m_posY + i) + 0.5f);
				bool canBuild = true;
				for (auto& cw : currentWalls)
				{
					XMFLOAT2 currentWallPos(cw->getObject3D().getPosition().x, cw->getObject3D().getPosition().z);
					if (newPos.x == currentWallPos.x && currentWallPos.y == newPos.y)
					{
						canBuild = false;
					}
				}
				if (canBuild)
				{
					Wall* newWallLeft = new Wall(mesh, { 1, 0 });
					newWallLeft->setRotation(0, 90, 0);
					newWallLeft->setPosition(static_cast<float>(m_posX), static_cast<float>(m_posY + i) + 0.5f);
					newWallLeft->getObject3D().setLightIndex(m_index);
					m_allWalls.push_back(newWallLeft);
					m_leftWalls.push_back(newWallLeft);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < m_sizeY; ++i)
		{

			if (allowed[i])
			{
				XMFLOAT2 newPos(static_cast<float>(m_posX + m_sizeX), static_cast<float>(m_posY + i) + 0.5f);
				bool canBuild = true;
				for (auto& cw : currentWalls)
				{
					XMFLOAT2 currentWallPos(cw->getObject3D().getPosition().x, cw->getObject3D().getPosition().z);
					if (newPos.x == currentWallPos.x && currentWallPos.y == newPos.y)
					{
						canBuild = false;
					}
				}
				if (canBuild)
				{
					Wall* newWallRight = new Wall(mesh, { -1, 0 });
					newWallRight->setRotation(0, 90, 0);
					newWallRight->setPosition(static_cast<float>(m_posX + m_sizeX), static_cast<float>(m_posY + i) + 0.5f);
					newWallRight->getObject3D().setLightIndex(m_index);
					m_allWalls.push_back(newWallRight);
					m_rightWalls.push_back(newWallRight);
				}
			}

		}
	}
}

void Room::AddRoomObject(Furniture * fut)
{
	Furniture * temp = fut->MakeCopy();
	temp->setLightIndex(this->m_index);
	m_roomObjects.push_back(temp);
	m_roomObjects[m_roomObjects.size() - 1]->setIndex(static_cast<int>(m_roomObjects.size()));
}

void Room::PickTiles()
{
	for (auto& rTile : m_roomTiles)
	{
		rTile->getQuad().setColor(1.0f, 1.0f, 1.0f);
		rTile->getQuad().CheckPick();
	}
}

void Room::PickWalls()
{
	for (auto& wall : m_allWalls)
		wall->getObject3D().CheckPick();
}

void Room::PickFurnitures()
{
	for (auto& obj : m_roomObjects)
		obj->getObject3D().CheckPick();
}

void Room::DeleteWalls()
{
	auto it = m_allWalls.begin();
	for (; it != m_allWalls.end(); )
	{
		if (false == (*it)->getIsDoor())
		{
			int counter = 0;
			for (auto& up : m_upWalls)
			{
				if (*(*it) == *up)
				{
					m_upWalls[counter] = nullptr;
					m_upWalls.erase(m_upWalls.begin() + counter);
					break;
				}
				counter++;
			}
			counter = 0;
			for (auto& up : m_downWalls)
			{
				if (*(*it) == *up)
				{
					m_downWalls[counter] = nullptr;
					m_downWalls.erase(m_downWalls.begin() + counter);
					break;
				}
				counter++;
			}
			counter = 0;
			for (auto& up : m_rightWalls)
			{
				if (*(*it) == *up)
				{
					m_rightWalls[counter] = nullptr;
					m_rightWalls.erase(m_rightWalls.begin() + counter);
					break;
				}
				counter++;
			}
			counter = 0;
			for (auto& up : m_leftWalls)
			{
				if (*(*it) == *up)
				{
					m_leftWalls[counter] = nullptr;
					m_leftWalls.erase(m_leftWalls.begin() + counter);
					break;
				}
				counter++;
			}
			delete *it;
			*it = nullptr;
			it = m_allWalls.erase(it);
		}
		else
		{
			it++;
			std::cout << "Was a door \n";
		}
	}
}

void Room::Select()
{
	m_selected = !m_selected;
}

std::vector<Tile*> Room::ReturnTiles()
{
	std::vector<Tile*> tiles;
	for (auto& t : m_roomTiles)
	{
		tiles.push_back(t);
		t = nullptr;
	}
	return tiles;
}

std::vector<Tile*> Room::getTiles()
{
	return m_roomTiles;
}

int Room::getX() const
{
	return m_posX;
}

int Room::getY() const
{
	return m_posY;
}

XMINT2 Room::getSize() const
{
	return XMINT2(m_sizeX, m_sizeY);
}

DirectX::XMFLOAT3 Room::getPosition() const
{
	return DirectX::XMFLOAT3(static_cast<float>(getX()), 0.0f, static_cast<float>(getY()));
}
//
//void Room::addAdjasentRoomDoor(Room * room, XMINT2 doorPos, XMINT2 direction)
//{
//	if (std::find(adjasentRoomDoors.begin(), adjasentRoomDoors.end(), room) == adjasentRoomDoors.end())
//	{
//		RoomConnection newConnection;
//		newConnection.otherRoom = room;
//		newConnection.connectingDoor = doorPos;
//		newConnection.direction = direction;
//		adjasentRoomDoors.push_back(newConnection);
//	}
//}

//void Room::addAdjasentRoom(Room * room)
//{
//	if (std::find(adjasentRooms.begin(), adjasentRooms.end(), room) == adjasentRooms.end())
//	{
//		adjasentRooms.push_back(room);
//	}
//}

//XMINT2 Room::getConnectingRoomDoorPositionPartOne(Room * otherroom)
//{
//	std::vector<RoomConnection>::iterator it =  std::find(adjasentRoomDoors.begin(), adjasentRoomDoors.end(), otherroom);
//
//	if (it != adjasentRoomDoors.end())
//		return it->connectingDoor;
//	else
//		return XMINT2(-1, -1);
//}

//XMINT2 Room::getConnectingRoomDoorPositionPartTwo(Room * otherroom)
//{
//	std::vector<RoomConnection>::iterator it = std::find(adjasentRoomDoors.begin(), adjasentRoomDoors.end(), otherroom);
//
//	if (it != adjasentRoomDoors.end())
//	{
//		XMINT2 doorPos = it->connectingDoor;
//		return { doorPos.x + it->direction.x, doorPos.y + it->direction.y };
//	}
//	else
//		return XMINT2(-1, -1);
//}


std::vector<Wall*>* Room::getAllWalls()
{
	return &m_allWalls;
}

std::vector<Wall*> Room::getWalls(Direction dir)
{
	switch (dir)
	{
	case Direction::up:
		return m_upWalls;
		break;
	case Direction::down:
		return m_downWalls;
		break;
	case Direction::left:
		return m_leftWalls;
		break;
	case Direction::right:
		return m_rightWalls;
		break;
	}
	return std::vector<Wall*>();
}

const RoomType & Room::getRoomType() const
{
	return m_roomType;
}

int Room::getAmountOfObjects()
{
	return static_cast<int>(m_roomObjects.size());
}

int Room::getAmountOfSpecificObjects(Furniture * compare)
{
	int amount = 0;
	if (m_roomObjects.size() <= 0)
	{
		return -1;
	}
	else
	{
		for (size_t i = 0; i < m_roomObjects.size(); i++)
		{
			if (m_roomObjects.at(i)->WhatType() == compare->WhatType())
			{
				amount++;
			}

		}
	}
	return amount;
}

int Room::getAmountOfSpecificObjectsNotBusy(Furniture * compare)
{
	int amount = 0;
	if (m_roomObjects.size() <= 0)
	{
		return -1;
	}
	else
	{
		for (size_t i = 0; i < m_roomObjects.size(); i++)
		{
			if (m_roomObjects.at(i)->WhatType() == compare->WhatType())
			{
				if (false == m_roomObjects.at(i)->getIsBusy())
				{
					amount++;
				}
			}

		}
	}
	return amount;
}

int Room::getPriceOfAllObjects()
{
	int m = 0;
	for (size_t i = 0; i < m_roomObjects.size(); i++)
	{
		m += m_roomObjects.at(i)->getPrice();
	}
	return m;
}

std::vector<Furniture*> Room::getAllRoomFurnitures()
{
	return m_roomObjects;
}

bool Room::RemoveThisFurniture(Furniture * fur)
{
	for (int r = 0; r < m_roomObjects.size(); ++r)
	{
		if (m_roomObjects.at(r) == fur)
		{
			std::vector<int> tileIndices;
			int sizeOf = fur->getGridSize();
			float x = 0;
			float y = 0;
			tileIndices.push_back(_index(fur->getPosition().x, fur->getPosition().z));
			for (int i = 0; i < sizeOf -1; i++)
			{
				switch (fur->getRotation())
				{
				case 0:
					y++;
					break;
				case 90:
					x++;
					break;
				case 180:
					y--;
					break;
				case 270:
					x--;
					break;
				}
				tileIndices.push_back(_index(fur->getPosition().x + static_cast<int>(x), fur->getPosition().z + static_cast<int>(y)));
			}
			for (size_t i = 0; i < tileIndices.size(); i++)
			{
				m_roomTiles.at(tileIndices[i])->setHasObject(false);
			}
			int t = _index(fur->getPosition().x, fur->getPosition().z);
			delete fur;
			m_roomObjects.erase(m_roomObjects.begin() + r);
			
			return true;
		}
	}
	return false;
}

Furniture * Room::getFurnitureAtPos(DirectX::XMINT2 pos)
{
	for (auto& element : m_roomObjects)
	{
		if (element->getPosition().x == pos.x && element->getPosition().z == pos.y)
		{
			return element;
		}
	}
	return nullptr;
}

RoomType Room::getRoomType()
{
	return m_roomType;
}

std::vector<Furniture*> Room::getNoneBusyFurnitures()
{
	std::vector<Furniture*> tempFurni;
	for (int i = 0; i < m_roomObjects.size(); ++i)
	{
		if (false == m_roomObjects.at(i)->getIsBusy())
		{
			tempFurni.push_back(m_roomObjects.at(i));
		}
	}
	return tempFurni;
}

void Room::move(int x, int y)
{
	this->m_posX += x;
	this->m_posY += y;
}
