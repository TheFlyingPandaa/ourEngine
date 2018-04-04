#include "RoomCtrl.h"



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
			Tile * t2 = t->getAdjacent(WallDirection::down);

			createWall = true;
			if (t2 != nullptr)
				if (t2->isWall(WallDirection::up))
				{
					createWall = false;
					t2->m_w[WallDirection::up]->setIsInner(true);
				}

			if (createWall)
			{				
				t->m_walls[WallDirection::down] = true;
				Wall * wall = new Wall(t, this->m_wall);
				t->m_w[WallDirection::down] = wall;
				
				wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + x) + WALLOFFSET, this->m_rooms[i]->getY()));
				this->m_walls.push_back(wall);
			}

			t = m_rooms[i]->getTiles(x, m_rooms[i]->getSizeY() - 1);
			t2 = t->getAdjacent(WallDirection::up);
			createWall = true;
		
			if (t2 != nullptr)
				if (t2->isWall(WallDirection::down))
				{
					createWall = false;
					t2->m_w[WallDirection::down]->setIsInner(true);
				}
			if (createWall)
			{
				//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getX() + x, this->m_rooms[i]->getY() + this->m_rooms[i]->getSizeY() - 1);
				Tile * t = m_rooms[i]->getTiles(x, this->m_rooms[i]->getSizeY() - 1);
				t->m_walls[WallDirection::up] = true;
				Wall * wall = new Wall(t, this->m_wall);
				t->m_w[WallDirection::up] = wall;

				wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + x) + WALLOFFSET, this->m_rooms[i]->getY() + this->m_rooms[i]->getSizeY()));
				this->m_walls.push_back(wall);
			}
		}


	}

	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		for (int y = 0; y < m_rooms[i]->getSizeY(); y++)
		{
			Tile * t = m_rooms[i]->getTiles(0, y);
			Tile * t2 = t->getAdjacent(WallDirection::left);

			createWall = true;
			if (t2 != nullptr)
				if (t2->isWall(WallDirection::right))
				{
					createWall = false;
					t2->m_w[WallDirection::right]->setIsInner(true);
				}
			if (createWall)
			{
				//obj->setPos(this->m_posX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
				//Tile * t = m_rooms[i]->getTiles(0, y);
				t->m_walls[WallDirection::left] = true;
				Wall * wall = new Wall(t, this->m_wall);
				t->m_w[WallDirection::left] = wall;
				wall->setRotation(DirectX::XMFLOAT3(0, 90, 0));

				wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX()), this->m_rooms[i]->getY() + y + WALLOFFSET));
				this->m_walls.push_back(wall);
			}			

			t = m_rooms[i]->getTiles(m_rooms[i]->getSizeX() - 1, y);
			t2 = t->getAdjacent(WallDirection::right);

			createWall = true;
			if (t2 != nullptr)
				if (t2->isWall(WallDirection::left))
				{
					createWall = false;
					t2->m_w[WallDirection::left]->setIsInner(true);
				}
			if (createWall)
			{
				//obj->setPos(this->m_posX + this->m_sizeX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
				//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getX() + this->m_rooms[i]->getSizeX() - 1, this->m_rooms[i]->getY() + y);
				//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getSizeX() - 1, y);
				t->m_walls[WallDirection::right] = true;
				Wall * wall = new Wall(t, this->m_wall);
				t->m_w[WallDirection::right] = wall;
				wall->setRotation(DirectX::XMFLOAT3(0, 90, 0));

				wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + this->m_rooms[i]->getSizeX()), this->m_rooms[i]->getY() + y + WALLOFFSET));
				this->m_walls.push_back(wall);
			}
		}
	}	
}


