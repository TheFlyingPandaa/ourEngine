#include "RoomCtrl.h"



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

	bool isPlaceable = false;

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
		
	}

	return isPlaceable;
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
		break;
	default:
		break;
	}

	if (room)
		m_rooms.push_back(room);
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
						t2->m_w[Direction::up]->setIsInner(true);
					}

				if (createWall) // where we aculy create the walls
				{
					t->m_walls[Direction::down] = true;
					Wall * wall = new Wall(t, this->m_wall);
					t->m_w[Direction::down] = wall;

					wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + x) + WALLOFFSET, static_cast<float>(this->m_rooms[i]->getY())));
					m_rooms[i]->addWall(wall, Direction::down);
					this->m_walls.push_back(wall);
				}
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
						t2->m_w[Direction::down]->setIsInner(true);
					}
				if (createWall)
				{
					Tile * t = m_rooms[i]->getTiles(x, this->m_rooms[i]->getSizeY() - 1);
					t->m_walls[Direction::up] = true;
					Wall * wall = new Wall(t, this->m_wall);
					t->m_w[Direction::up] = wall;

					wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + x) + WALLOFFSET, static_cast<float>(this->m_rooms[i]->getY() + this->m_rooms[i]->getSizeY())));
					m_rooms[i]->addWall(wall, Direction::up);
					this->m_walls.push_back(wall);
				}
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
						t2->m_w[Direction::right]->setIsInner(true);
					}
				if (createWall)
				{
					//obj->setPos(this->m_posX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
					//Tile * t = m_rooms[i]->getTiles(0, y);
					t->m_walls[Direction::left] = true;
					Wall * wall = new Wall(t, this->m_wall);
					t->m_w[Direction::left] = wall;
					wall->setRotation(DirectX::XMFLOAT3(0, 90, 0));

					wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX()), this->m_rooms[i]->getY() + y + WALLOFFSET));
					m_rooms[i]->addWall(wall, Direction::left);
					this->m_walls.push_back(wall);
				}
			}
			t = m_rooms[i]->getTiles(m_rooms[i]->getSizeX() - 1, y);
			if (!t->isWall(Direction::right))
			{
				t2 = t->getAdjacent(Direction::right);

				createWall = true;
				if (t2 != nullptr)
					if (t2->isWall(Direction::left))
					{
						createWall = false;
						t2->m_w[Direction::left]->setIsInner(true);
					}
				if (createWall)
				{
					//obj->setPos(this->m_posX + this->m_sizeX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
					//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getX() + this->m_rooms[i]->getSizeX() - 1, this->m_rooms[i]->getY() + y);
					//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getSizeX() - 1, y);
					t->m_walls[Direction::right] = true;
					Wall * wall = new Wall(t, this->m_wall);
					t->m_w[Direction::right] = wall;
					wall->setRotation(DirectX::XMFLOAT3(0, 90, 0));

					wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + this->m_rooms[i]->getSizeX()), this->m_rooms[i]->getY() + y + WALLOFFSET));
					m_rooms[i]->addWall(wall, Direction::right);
					this->m_walls.push_back(wall);
				}
			}
		}
	}	
}


