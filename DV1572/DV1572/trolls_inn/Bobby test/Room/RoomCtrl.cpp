#include "RoomCtrl.h"



void RoomCtrl::setTileMesh(Mesh * mesh, RoomType roomType)
{
	m_tileMesh[roomType] = mesh;
}

bool RoomCtrl::_intersect(Room * room)
{
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		if ((std::abs(room->getX() - m_rooms[i]->getX()) * 2) < (room->getSizeX() + m_rooms[i]->getSizeX()) &&
			(std::abs(room->getY() - m_rooms[i]->getY()) * 2) < (room->getSizeY() + m_rooms[i]->getSizeY()))
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
		if (pos.x < m_rooms[i]->getX() + m_rooms[i]->getSizeX() &&
			pos.x + size.x > m_rooms[i]->getX() &&
			pos.y < m_rooms[i]->getY() + m_rooms[i]->getSizeY() &&
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
		mx = r->getX() + r->getSizeX();
		y = r->getY();
		my = r->getY() + r->getSizeY();

		if (pos.x < m_rooms[i]->getX() + m_rooms[i]->getSizeX() &&
			pos.x + size.x > m_rooms[i]->getX() &&
			pos.y < m_rooms[i]->getY() + m_rooms[i]->getSizeY() &&
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

RoomCtrl::RoomCtrl()
{

}


RoomCtrl::~RoomCtrl()
{
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		delete m_rooms[i];
	}
	m_rooms.clear();
	for (size_t i = 0; i < m_walls.size(); i++)
	{
		delete m_walls[i];
	}
	m_walls.clear();
}

void RoomCtrl::setMesh(Mesh * mesh)
{
	this->m_wall = mesh;
}

void RoomCtrl::AddRoom(DirectX::XMINT2 pos, DirectX::XMINT2 size, RoomType roomType, std::vector<std::vector<Tile*>> tiles, bool force)
{
	Room * room = nullptr;
	if (!force)
	{		
		if (!isPlaceable(pos, size))
			return;
	}

	switch (roomType)
	{
	case kitchen:
		room = new Kitchen(pos.x, pos.y, size.x, size.y, tiles);

		if (m_tileMesh[0] != nullptr)
			room->setTile(m_tileMesh[0]);
		break;
	case bedroom:
		break;
	case reception:
		room = new Reception(pos.x, pos.y, size.x, size.y, tiles);

		if (m_tileMesh[2] != nullptr)
			room->setTile(m_tileMesh[2]);


		break;
	default:
		break;
	}
	if (room)
		m_rooms.push_back(room);
	CreateWalls();
	CreateDoors(room);
	
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
	for (int i = 0; i < m_walls.size(); i++)
	{	
		m_walls[i]->getObject3D().setInside();
		m_walls[i]->Draw();
	}
}

void RoomCtrl::CreateWalls()
{
	if (this->m_wall == nullptr)
	{
		std::cout << "no mesh boi" << std::endl;
		return;
	}	
	bool createWall = true;
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		for (int x = 0; x < m_rooms[i]->getSizeX(); x++)
		{
			Tile * t = m_rooms[i]->getTiles(x,0);
			Tile * t2;
			if (!t->isWall(Direction::down))
			{
				t2 = t->getAdjacent(Direction::down);

				createWall = true;
				if (t2 != nullptr)
					if (t2->isWall(Direction::up))
					{
						createWall = false;
						t2->getTileWalls(Direction::up)->setIsInner(true);		
						m_rooms[i]->addAdjasentRoom(t2->getRoom());
						t->setTileWalls(Direction::down, t2->getTileWalls(Direction::up));
						m_rooms[i]->addWall(t2->getTileWalls(Direction::up), Direction::down);
					}

				if (createWall) // where we aculy create the walls
				{
					t->setWallSpotPopulated(Direction::down, true);
					Wall * wall = new Wall(t, this->m_wall);
					t->setTileWalls(Direction::down, wall);

					wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + x) + WALLOFFSET, static_cast<float>(this->m_rooms[i]->getY())));
					m_rooms[i]->addWall(wall, Direction::down);
					this->m_walls.push_back(wall);
				}
			}
			else {
				m_rooms[i]->addWall(t->getTileWalls(Direction::down), Direction::down);
			}
			t = m_rooms[i]->getTiles(x, m_rooms[i]->getSizeY() - 1);
			if (!t->isWall(Direction::up))
			{
				t2 = t->getAdjacent(Direction::up);
				createWall = true;

				if (t2 != nullptr)
					if (t2->isWall(Direction::down))
					{
						createWall = false;
						t2->getTileWalls(Direction::down)->setIsInner(true);
						m_rooms[i]->addAdjasentRoom(t2->getRoom());
						t->setTileWalls(Direction::up, t2->getTileWalls(Direction::down));
						m_rooms[i]->addWall(t2->getTileWalls(Direction::down), Direction::up);
					}
				if (createWall)
				{
					Tile * t = m_rooms[i]->getTiles(x, this->m_rooms[i]->getSizeY() - 1);
					t->setWallSpotPopulated(Direction::up, true);
					Wall * wall = new Wall(t, this->m_wall);
					t->setTileWalls(Direction::up, wall);

					wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + x) + WALLOFFSET, static_cast<float>(this->m_rooms[i]->getY() + this->m_rooms[i]->getSizeY())));
					m_rooms[i]->addWall(wall, Direction::up);
					this->m_walls.push_back(wall);
				}
			}
			else {
				m_rooms[i]->addWall(t->getTileWalls(Direction::up), Direction::up);
			}
		}


	}

	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		for (int y = 0; y < m_rooms[i]->getSizeY(); y++)
		{
			
			Tile * t = m_rooms[i]->getTiles(0, y);
			Tile * t2;
			if (!t->isWall(Direction::left))
			{
				 t2 = t->getAdjacent(Direction::left);

				createWall = true;
				if (t2 != nullptr)
					if (t2->isWall(Direction::right))
					{
						createWall = false;
						t2->getTileWalls(Direction::right)->setIsInner(true);
						m_rooms[i]->addAdjasentRoom(t2->getRoom());
						t->setTileWalls(Direction::left, t2->getTileWalls(Direction::right));
						m_rooms[i]->addWall(t2->getTileWalls(Direction::right), Direction::left);
					}
				if (createWall)
				{
					//obj->setPos(this->m_posX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
					//Tile * t = m_rooms[i]->getTiles(0, y);
					t->setWallSpotPopulated(Direction::left, true);
					Wall * wall = new Wall(t, this->m_wall);
					t->setTileWalls(Direction::left, wall);
					wall->setRotation(DirectX::XMFLOAT3(0, 90, 0));

					wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX()), this->m_rooms[i]->getY() + y + WALLOFFSET));
					m_rooms[i]->addWall(wall, Direction::left);
					this->m_walls.push_back(wall);
				}
			}
			else
				m_rooms[i]->addWall(t->getTileWalls(Direction::left), Direction::left);

			t = m_rooms[i]->getTiles(m_rooms[i]->getSizeX() - 1, y);
			if (!t->isWall(Direction::right))
			{
				t2 = t->getAdjacent(Direction::right);

				createWall = true;
				if (t2 != nullptr)
					if (t2->isWall(Direction::left))
					{
						createWall = false;
						t2->getTileWalls(Direction::left)->setIsInner(true);
						m_rooms[i]->addAdjasentRoom(t2->getRoom());
						t->setTileWalls(Direction::right, t2->getTileWalls(Direction::left));
						m_rooms[i]->addWall(t2->getTileWalls(Direction::left), Direction::right);

					}
				if (createWall)
				{
					//obj->setPos(this->m_posX + this->m_sizeX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
					//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getX() + this->m_rooms[i]->getSizeX() - 1, this->m_rooms[i]->getY() + y);
					//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getSizeX() - 1, y);
					t->setWallSpotPopulated(Direction::right, true);
					Wall * wall = new Wall(t, this->m_wall);
					t->setTileWalls(Direction::right, wall);
					wall->setRotation(DirectX::XMFLOAT3(0, 90, 0));

					wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + this->m_rooms[i]->getSizeX()), this->m_rooms[i]->getY() + y + WALLOFFSET));
					m_rooms[i]->addWall(wall, Direction::right);
					this->m_walls.push_back(wall);
				}
			}
			else
				m_rooms[i]->addWall(t->getTileWalls(Direction::right), Direction::right);
		}
	}	
}

void RoomCtrl::CreateDoors(Room * room)
{
	//get other room type 
	for (Room * other : *room->getAdjasent()) {
		if (_checkLegal(other)) {
			/*Direction dir = getDirection(room, other);
			Direction otherDir = getDirection(other, room);*/
			std::vector<Wall*>* createdRoomWall = room->getAllWalls();
			std::vector<Wall*>* otherRoomWall = other->getAllWalls();

			std::vector<Wall*> intersectedWalls;

			
			for (int i = 0; i < createdRoomWall->size(); i++)
			{
				if(createdRoomWall->at(i)->getIsInner())
					for (int j = 0; j < otherRoomWall->size(); j++)
					{
						if (otherRoomWall->at(j)->getIsInner())
							if (createdRoomWall->at(i) == otherRoomWall->at(j))
							{
								intersectedWalls.push_back(createdRoomWall->at(i));
								break;
							}
					}
			}
			if (intersectedWalls.size() <= 0)
				return;
			Wall* door = intersectedWalls[(intersectedWalls.size()  ) / 2];
			Tile* t1 = door->getTile();
			Direction dir;
			for (int i = 0; i < 4; i++)
			{
				if (t1->getTileWalls(static_cast<Direction>(i)) == door)
				{
					dir = static_cast<Direction>(i);
					break;
				}
			}

			

			DirectX::XMINT2 pos = door->getPosition();
			pos.x -= static_cast<int>(room->getPosition().x);
			pos.y -= static_cast<int>(room->getPosition().z);
			
			/*if (pos.x >= 1)
				pos.x -= 1;*/

			//Tile* t1 = room->getTiles()[pos.x - 1][pos.y];
			Tile* t2 = t1->getAdjacent(dir);

			CreateDoor(t1, t2);

		}
	}
}

void RoomCtrl::setDoorMesh(Mesh * mesh)
{
	this->m_doorMesh = mesh;
}

void RoomCtrl::CreateDoor(Tile * tile1, Tile * tile2)
{
	Direction dir = this->getDirection(tile1, tile2);

	Wall* w = tile1->getTileWalls(dir);
	if (!w)
		return;
	w->setScale(1.0f,1.0f,1.0f);
	w->setIsDoor(true);
	//tile1->setWallSpotPopulated(dir, true);
	w->setMesh(this->m_doorMesh);
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

Direction RoomCtrl::getDirection(Room * r1, Room * r2)
{
	XMVECTOR r1Pos = XMLoadFloat3(&r1->getPosition());

	XMFLOAT3 r2PosLOL = r2->getPosition();
	int r2SizeX = r2->getSizeX();
	int r2SizeY = r2->getSizeY();

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



