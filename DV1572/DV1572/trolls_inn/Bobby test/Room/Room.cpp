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

	m_wholeFloor.setPos(posX, -0.001f, posY);
	m_wholeFloor.setScale(sizeX * 2.0f, 1, sizeY*2.0f);
	m_wholeFloor.setRotation(90.0f, 0.0f, 0.0f);
	//TODO //
	m_wholeFloor.setUVScale(sizeX);

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
#include <iostream>
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
		if (wall->isShared() && false == wall->getIsDoor())
		{
			wall->setScale(1, 0.05f, 1.0f);
		}
		else
		{
			XMFLOAT3 wallDir = { wall->getDirection().x, 0, wall->getDirection().y };
			XMVECTOR xmWallDir = XMLoadFloat3(&wallDir);
			float angleDegrees = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(xmCamDir, xmWallDir)));
			
			if (angleDegrees < 90 && false == wall->getIsDoor())
			{
				float funcy = (2.0f * (angleDegrees / 90) ) - 1.0f;
				wall->setScale(1, funcy < 0.05f ? 0.05f : funcy, 1.0f);
			}
			else
				wall->setScale(1, 1.0f, 1.0f);
				//std::cout << (angleDegrees/90.0f) << std::endl
		}
	}
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
	//m_AABB.Draw();
}

bool Room::operator==(const Room & other) const
{
	return m_posX == other.m_posX && m_posY == other.m_posY;
}

void Room::setFloorMesh(Mesh * mesh)
{
	m_wholeFloor.setMesh(mesh);
	for (auto& tile : m_roomTiles)
		tile->getQuad().setMesh(mesh);
}

void Room::CreateWallSide(Mesh* mesh, std::vector<bool> allowed, Direction side)
{
	if (side == up)
	{
		for (int i = 0; i < m_sizeX; ++i)
		{
			if (allowed[i])
			{
				Wall* newWallUp = new Wall(mesh, { 0, -1 });
				newWallUp->setPosition(m_posX + 0.5f + i, m_posY + m_sizeY);
				m_allWalls.push_back(newWallUp);
				m_upWalls.push_back(newWallUp);
			}
		}
	}
	else if (side == down)
	{
		for (int i = 0; i < m_sizeX; ++i)
		{
			if (allowed[i])
			{
				Wall* newWallLow = new Wall(mesh, { 0,1 });
				newWallLow->setPosition(m_posX + 0.5f + i, m_posY);
				m_allWalls.push_back(newWallLow);
				m_downWalls.push_back(newWallLow);
			}
		}
	}
	else if (side == left)
	{
		for (int i = 0; i < m_sizeY; ++i)
		{
			if (allowed[i])
			{
				Wall* newWallLeft = new Wall(mesh, { 1, 0 });
				newWallLeft->setRotation(0, 90, 0);
				newWallLeft->setPosition(m_posX, m_posY + 0.5f + i);
				m_allWalls.push_back(newWallLeft);
				m_leftWalls.push_back(newWallLeft);
			}
		}
	}
	else
	{
		for (int i = 0; i < m_sizeY; ++i)
		{

			if (allowed[i])
			{
				Wall* newWallRight = new Wall(mesh, { -1, 0 });
				newWallRight->setRotation(0, 90, 0);
				newWallRight->setPosition(m_posX + m_sizeX, m_posY + 0.5f + i);
				m_allWalls.push_back(newWallRight);
				m_rightWalls.push_back(newWallRight);
			}

		}
	}
}

void Room::AddRoomObject(Furniture fut)
{
	m_roomObjects.push_back(fut);
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

void Room::move(int x, int y)
{
	this->m_posX += x;
	this->m_posY += y;
}
