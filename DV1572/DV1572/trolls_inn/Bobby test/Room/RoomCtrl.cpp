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
	//this->m_hasWalls = true;
	bool createWall = true;
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		for (int x = 0; x < m_rooms[i]->getSizeX(); x++)
		{
			Tile * t = m_rooms[i]->getTiles(x,0);
			createWall = true;
			for (size_t j = 0; j < m_walls.size() && createWall; j++)
			{
				Tile * t2 = m_walls[j]->getTile();
				if (t == t2)
					if (t2->isWall(WallDirection::down))
					{
						createWall = false;
						m_walls[j]->setIsInner(true);
					}
			}
			if (createWall)
			{
				//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getX() + x, this->m_rooms[i]->getY());
				t->m_walls[WallDirection::down] = true;
				Wall * wall = new Wall(t, this->m_wall);
				
				wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + x) + WALLOFFSET, this->m_rooms[i]->getY()));
				this->m_walls.push_back(wall);
			}


			createWall = true;
			for (size_t j = 0; j < m_walls.size() && !createWall; j++)
			{
				if (m_walls[j]->getTile()->isWall(WallDirection::down))
				{
					createWall = false;
					m_walls[j]->setIsInner(true);
				}
			}
			if (createWall)
			{
				//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getX() + x, this->m_rooms[i]->getY() + this->m_rooms[i]->getSizeY() - 1);
				Tile * t = m_rooms[i]->getTiles(x, this->m_rooms[i]->getSizeY() - 1);
				t->m_walls[WallDirection::up] = true;
				Wall * wall = new Wall(t, this->m_wall);

				wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + x) + WALLOFFSET, this->m_rooms[i]->getY() + this->m_rooms[i]->getSizeY()));
				this->m_walls.push_back(wall);
			}
		}


	}

	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		for (int y = 0; y < m_rooms[i]->getSizeY(); y++)
		{
			createWall = true;
			for (size_t j = 0; j < m_walls.size() && !createWall; j++)
			{
				if (m_walls[j]->getTile()->isWall(WallDirection::left))
				{
					createWall = false;
					m_walls[j]->setIsInner(true);
				}
			}
			if (createWall)
			{
				//obj->setPos(this->m_posX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
				Tile * t = m_rooms[i]->getTiles(0, y);
				t->m_walls[WallDirection::left] = true;
				Wall * wall = new Wall(t, this->m_wall);
				wall->setRotation(DirectX::XMFLOAT3(0, 90, 0));

				wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX()), this->m_rooms[i]->getY() + y + WALLOFFSET));
				this->m_walls.push_back(wall);
			}			

			createWall = true;
			for (size_t j = 0; j < m_walls.size() && !createWall; j++)
			{
				if (m_walls[j]->getTile()->isWall(WallDirection::right))
				{
					createWall = false;
					m_walls[j]->setIsInner(true);
				}
			}
			if (createWall)
			{
				//obj->setPos(this->m_posX + this->m_sizeX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
				//Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getX() + this->m_rooms[i]->getSizeX() - 1, this->m_rooms[i]->getY() + y);
				Tile * t = m_rooms[i]->getTiles(this->m_rooms[i]->getSizeX() - 1, y);
				t->m_walls[WallDirection::right] = true;
				Wall * wall = new Wall(t, this->m_wall);
				wall->setRotation(DirectX::XMFLOAT3(0, 90, 0));

				wall->setPosition(DirectX::XMFLOAT2(static_cast<float>(this->m_rooms[i]->getX() + this->m_rooms[i]->getSizeX()), this->m_rooms[i]->getY() + y + WALLOFFSET));
				this->m_walls.push_back(wall);
			}
		}
	}

	/*
	for (size_t x = 0; x < m_sizeX; x++)
	{
		Object3D * obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setPos(static_cast<float>(this->m_posX + static_cast<int>(x)) + 0.5f, 0, this->m_posY);
		//obj->setScale(0.5f);
		down.push_back(obj);

		obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setPos(static_cast<float>(this->m_posX + static_cast<int>(x)) + 0.5f, 0, this->m_posY + m_sizeY);
		//obj->setScale(0.5f);
		up.push_back(obj);
	}

	for (size_t y = 0; y < m_sizeY; y++)
	{
		Object3D * obj = new Object3D();
		obj->setMesh(m_wall);

		obj->setRotation(0, 90, 0);
		obj->setPos(this->m_posX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
		//obj->setScale(0.5f);
		left.push_back(obj);

		obj = new Object3D();
		obj->setMesh(m_wall);


		obj->setRotation(0, 90, 0);
		obj->setPos(this->m_posX + this->m_sizeX, 0, static_cast<float>(this->m_posY + static_cast<int>(y)) + 0.5f);
		//obj->setScale(0.5f);
		right.push_back(obj);
	}
	*/
}


