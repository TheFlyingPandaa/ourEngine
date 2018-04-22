#include "Room.h"

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
	m_AABB.setPos(x + ((float)sx / 2.0f) + 0.001, (level * 2.0f) + 1.0f, y + ((float)sy / 2.0f) - 0.001);
	m_AABB.setScale(static_cast<float>(sx) + 0.05, 2.1f, static_cast<float>(sy) + 0.05);
}

void Room::_createLight(int x, int y, int sx, int sy, int level)
{
	PointLight l;
	l.setPosition(static_cast<float>(x) + ((float)sx / 2), static_cast<float>(level * 2 + 2), static_cast<float>(y) + ((float)sy / 2));
	l.setColor((rand() % 11) * 0.1f, (rand() % 11) * 0.1f, (rand() % 11) * 0.1f);
	l.setSettingsForLight(1, 0.8f);
	l.setIndex(m_index);
	m_lights.push_back(l);
	m_lights[0].addToLightQueue();
}

Room::Room(int posX, int posY, int sizeX, int sizeY, Mesh * m)
{
	//if (!s_isLoaded)
	//	_loadStatic();
	//m_index = s_index++;
	//_initAABB(posX, posY, sizeX, sizeY);
	//_createLight(posX, posY, sizeX, sizeY);

	//this->m_posX = posX;
	//this->m_posY = posY;
	//this->m_sizeX = sizeX;
	//this->m_sizeY = sizeY;
	//
	////this->m_hasDoor = false;
	//m_roomTiles.push_back()
	//for (unsigned short x = 0; x < m_tiles.size(); x++)
	//{
	//	for (unsigned short y = 0; y < m_tiles[x].size(); y++)
	//	{
	//		
	//		m_tiles[x][y]->setMesh(m);
	//	}
	//}
	//TODO: Dont forget to send the tiles you lil cunt :D
}

Room::Room(int posX, int posY, int sizeX, int sizeY, std::vector<Tile*> tiles)
{
	if (!s_isLoaded)
		_loadStatic();
	m_index = s_index++;
	_initAABB(posX, posY, sizeX, sizeY);
	_createLight(posX, posY, sizeX, sizeY);

	this->m_posX = posX;
	this->m_posY = posY;
	this->m_sizeX = sizeX;
	this->m_sizeY = sizeY;

	this->m_roomTiles = tiles;
}

Room::~Room()
{
	for (auto& walls : m_allWalls)
		delete walls;
	for (auto& tile : m_roomTiles)
		delete tile;
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
	XMVECTOR xmCamDir = XMLoadFloat3(&cam->getLookAt());
	for (auto& wall : m_allWalls)
	{
		XMFLOAT3 wallDir = { wall->getDirection().x, 0, wall->getDirection().y };
		XMVECTOR xmWallDir = XMLoadFloat3(&wallDir);
		float angleDegrees = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(xmCamDir, xmWallDir)));
		if (angleDegrees < 90)
			wall->setScale(1, 0.05f, 1.0f);
		else 
			wall->setScale(1, 1.0f, 1.0f);
	}


	
	/*if (cam)
	{
		bool cullWalls[4] = { false, false, false, false };
		DirectX::XMFLOAT3 camPos3D = cam->getPosition();
		DirectX::XMFLOAT2 camPosition = { camPos3D.x, camPos3D.z };
		DirectX::XMFLOAT2 roomCenter(m_AABB.getPosition().x, m_AABB.getPosition().z);
		float distanceToCamera = DirectX::XMVectorGetX(DirectX::XMVector2Length(DirectX::XMLoadFloat2(&camPosition) - DirectX::XMLoadFloat2(&roomCenter)));
		int cullDist = 15;

		if (distanceToCamera < cullDist)
		{
			if (camPosition.x < m_posX)
			{
				cullWalls[Direction::left] = true;
			}
			else if (camPosition.x > m_posX + m_sizeX)
			{
				cullWalls[Direction::right] = true;
			}
			if (camPosition.y < m_posY)
			{
				cullWalls[Direction::down] = true;
			}
			else if (camPosition.y > m_posY + m_sizeY)
			{
				cullWalls[Direction::up] = true;
			}
		}

		bool changeUp = false;
		bool changeDown = false;
		bool changeRight = false;
		bool changeLeft = false;

		if (m_culledWalls[Direction::up] != cullWalls[Direction::up])
		{
			m_culledWalls[Direction::up] = cullWalls[Direction::up];
			changeUp = true;
		}
		if (m_culledWalls[Direction::down] != cullWalls[Direction::down])
		{
			m_culledWalls[Direction::down] = cullWalls[Direction::down];
			changeDown = true;
		}
		if (m_culledWalls[Direction::right] != cullWalls[Direction::right])
		{
			m_culledWalls[Direction::right] = cullWalls[Direction::right];
			changeRight = true;
		}
		if (m_culledWalls[Direction::left] != cullWalls[Direction::left])
		{
			m_culledWalls[Direction::left] = cullWalls[Direction::left];
			changeLeft = true;
		}


		if (changeUp || changeDown)
		{
			for (size_t x = 0; x < down.size(); x++)
			{
				if (changeDown)
				{
					if (m_culledWalls[Direction::down] && !down[x]->getIsDoor())
						down[x]->setScale(1.0f, 0.05f, 1.0f);
					else
						down[x]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
			for (size_t x = 0; x < up.size(); x++)
			{
				if (changeUp)
				{
					if (m_culledWalls[Direction::up] && !up[x]->getIsDoor())
						up[x]->setScale(1.0f, 0.05f, 1.0f);
					else
						up[x]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
		}

		if (changeRight || changeLeft)
		{
			for (size_t y = 0; y < left.size(); y++)
			{
				if (changeLeft)
				{
					if (m_culledWalls[Direction::left] && !left[y]->getIsDoor())
						left[y]->setScale(1.0f, 0.05f, 1.0f);
					else
						left[y]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
			for (size_t y = 0; y < right.size(); y++)
			{
				if (changeRight)
				{
					if (m_culledWalls[Direction::right] && !right[y]->getIsDoor())
						right[y]->setScale(1.0f, 0.05f, 1.0f);
					else
						right[y]->setScale(1.0f, 1.0f, 1.0f);
				}
			}
		}

	}
	for (int i = 0; i < down.size(); i++)
	{
		if (down[i]->getIsInner() && !down[i]->getIsDoor())
			down[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < up.size(); i++)
	{
		if (up[i]->getIsInner() && !up[i]->getIsDoor())
			up[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < left.size(); i++)
	{
		if (left[i]->getIsInner() && !left[i]->getIsDoor())
			left[i]->setScale(1.0f, 0.05f, 1.0f);
	}
	for (int i = 0; i < right.size(); i++)
	{
		if (right[i]->getIsInner() && !right[i]->getIsDoor())
			right[i]->setScale(1.0f, 0.05f, 1.0f);
	}*/
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
	//for (int i = 0; i < m_sizeY; i++)
	//{
	//	for (int j = 0; j < m_sizeX; j++)
	//	{
	//		m_tiles[j][i]->getQuad().setLightIndex(m_index);
	//	}
	//}
}

void Room::CastShadow()
{
	m_AABB.CastShadow();
	//m_AABB.Draw();
}

bool Room::operator==(const Room & other) const
{
	return m_posX == other.m_posX && m_posY == other.m_posY;
}

void Room::setFloorMesh(Mesh * mesh)
{
	for (auto& tile : m_roomTiles)
		tile->getQuad().setMesh(mesh);
}

void Room::CreateWallSide(Mesh* mesh, std::vector<bool> allowed, Direction side)
{
	// X led only
	if (side == up)
	{
		for (int i = 0; i < m_sizeX; ++i)
		{
			if (allowed[1])
			{
				Wall* newWallUp = new Wall(mesh, { 0, -1 });
				newWallUp->setPosition(m_posX + 0.5f + i, m_posY + m_sizeY);
				m_allWalls.push_back(newWallUp);
			}
		}
	}
	for (int i = 0; i < m_sizeX; ++i)
	{
		if (sides[0])
		{
			Wall* newWallLow = new Wall(mesh, { 0,1 });
			newWallLow->setPosition(m_posX + 0.5f + i, m_posY);
			m_allWalls.push_back(newWallLow);
		}
		if (sides[1])
		{
			Wall* newWallUp = new Wall(mesh, { 0, -1 });
			newWallUp->setPosition(m_posX + 0.5f + i, m_posY + m_sizeY);
			m_allWalls.push_back(newWallUp);
		}
		
	}

	for (int i = 0; i < m_sizeY; ++i)
	{
		if (sides[2])
		{
			Wall* newWallLeft = new Wall(mesh, { 1, 0 });
			newWallLeft->setRotation(0, 90, 0);
			newWallLeft->setPosition(m_posX, m_posY + 0.5f + i);
			m_allWalls.push_back(newWallLeft);
		}
		if (sides[3])
		{
			Wall* newWallRight = new Wall(mesh, { -1, 0 } );
			newWallRight->setRotation(0, 90, 0);
			newWallRight->setPosition(m_posX + m_sizeX, m_posY + 0.5f + i);
			m_allWalls.push_back(newWallRight);
		}
		
	}
}

int Room::getX() const
{
	return m_posX;
}

int Room::getY() const
{
	return m_posY;
}

int Room::getSizeX() const
{
	return m_sizeX;
}

int Room::getSizeY() const
{
	return m_sizeY;
}

void Room::setWalls(std::vector<Wall*> walls, Direction dir)
{
	for (int i = 0; i < walls.size(); i++)
	{
		m_allWalls.push_back(walls[i]);
	}

	//switch (dir)
	//{
	//case 0:
	//	up = walls;
	//	break;
	//case 1:
	//	down = walls;
	//	break;
	//case 2:
	//	left = walls;
	//	break;
	//case 3:
	//	right = walls;
	//	break;
	//default:
	//	break;
	//}
}

void Room::addWall(Wall * wall, Direction dir)
{
	/*m_allWalls.push_back(wall);
	switch (dir)
	{
	case 0:		
		if (std::find(up.begin(), up.end(), wall) == up.end())
			up.push_back(wall);
		break;
	case 1:
		if (std::find(down.begin(), down.end(), wall) == down.end())
			down.push_back(wall);
		break;
	case 2:
		if (std::find(left.begin(), left.end(), wall) == left.end())
			left.push_back(wall);
		break;
	case 3:
		if (std::find(right.begin(), right.end(), wall) == right.end())
			right.push_back(wall);
		break;
	default:
		break;
	}*/
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

std::vector<Wall*>* Room::getWall(Direction dir)
{
	/*switch (dir)
	{
	case Direction::up:
		return &up;
		break;
	case Direction::down:
		return &down;
		break;
	case Direction::left:
		return &left;
		break;
	case Direction::right:
		return &right;
		break;
	default:
		return nullptr;
		break;
	}*/
	return new std::vector<Wall*>();
}

void Room::move(int x, int y)
{
	this->m_posX += x;
	this->m_posY += y;
}
