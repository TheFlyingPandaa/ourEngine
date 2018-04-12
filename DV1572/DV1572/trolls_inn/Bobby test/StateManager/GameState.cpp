#include "GameState.h"
#include <iostream>
#include <stdlib.h>

GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	m_firstPick = false;
	m_lastPick = false;
	m_firstPickedTile = nullptr;
	m_middlePickedTile = nullptr;
	m_lastPickedTile = nullptr;
	m_isPlaceable = false;

	_setHud();

	box.LoadModel("trolls_inn/Resources/box.obj");

	box.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	box.setNormalTexture("trolls_inn/Resources/NormalMap.png");

	c.setModel(&box);
	c.setPosition(0.5, 0.5);
	c.setFloor(0);

	int startSize = 32;
	int firstRoomSizeX = 4;
	int firstRoomSizeY = 3;

	int secondRoomSizeX = 4;
	int secondRoomSizeY = 2;


	m_prevStart	  = XMINT2(0,0);
	m_prevEnd	  = XMINT2(0,0);

	this->m_cam = cam;
	this->_init();
	grid = new Grid(0, 0, startSize, startSize, &rect);	
	grid->CreateWalls(&m);	
	grid->getRoomCtrl().setTileMesh(&kitchenTile, RoomType::kitchen);
	grid->getRoomCtrl().setDoorMesh(&door);
	grid->AddRoom(DirectX::XMINT2((startSize / 2) - firstRoomSizeX / 2, (startSize / 2) - firstRoomSizeY / 2), DirectX::XMINT2(firstRoomSizeX, firstRoomSizeY), RoomType::kitchen, true);
	grid->AddRoom(DirectX::XMINT2(((startSize / 2) - firstRoomSizeX / 2) + firstRoomSizeX, ((startSize / 2) - firstRoomSizeY / 2) + firstRoomSizeY / 2), DirectX::XMINT2(secondRoomSizeX, secondRoomSizeY), RoomType::kitchen, false);
	grid->getRoomCtrl().CreateDoor(grid->getGrid()[(startSize / 2)][((startSize / 2) - ((firstRoomSizeY / 2)))], grid->getGrid()[(startSize / 2)][(startSize / 2) - ((firstRoomSizeY + 1) / 2)]);

	posX = 1;
	posY = 1;
	//grid->getRoomCtrl().CreateDoors();
	previousKey = -1;
	
}

GameState::~GameState()
{
	delete grid;
}

// round float to n decimals precision
float round_n(float num, int dec)
{
	float m = (num < 0.0f) ? -1.0f : 1.0f;   // check if input is negative
	float pwr = pow(10.0f, dec);
	return float((float)floor((double)num * m * pwr + 0.5) / pwr) * m;
}
void GameState::Update(double deltaTime)
{

	this->m_cam->update();
	this->grid->Update(this->m_cam);

	gameTime.updateCurrentTime(deltaTime); 
	_checkCreationOfRoom();

	c.Update();
	

	//std::cout << "Position: " << c.getPosition().x << " " << c.getPosition().y << std::endl;
	

	if (Input::isKeyPressed('W') && !move)
		c.Move(Character::WalkDirection::UP);
	else if (Input::isKeyPressed('S') && !move)
		c.Move(Character::WalkDirection::DOWN);
	else if (Input::isKeyPressed('D') && !move)
		c.Move(Character::WalkDirection::RIGHT);
	else if (Input::isKeyPressed('A') && !move)
		c.Move(Character::WalkDirection::LEFT);
	else if (Input::isKeyPressed('R'))
		_setHud();

	if (Input::isKeyPressed('W') || Input::isKeyPressed('A') ||
		Input::isKeyPressed('S') || Input::isKeyPressed('D') ||
		Input::isKeyPressed('R'))
	{
		move = true;
	}
	else
	{
		move = false;
	}

	while (!p_keyEvents->empty())
	{
		previousKey = p_keyEvents->top();
		p_keyEvents->pop();		
	}
	if (p_keyEvents->empty() || p_keyEvents->top() == 0)
		previousKey = -1;
	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		static Shape* firstHolder = nullptr;
		static Shape* secondHolder = nullptr;
		if (Input::isKeyPressed('G'))
			firstHolder = obj;
		if (Input::isKeyPressed('H'))
			secondHolder = obj;

		if (firstHolder && secondHolder)
		{
			grid->getRoomCtrl().CreateDoor(grid->getGrid()[static_cast<int>(firstHolder->getPosition().x + 0.5f)][static_cast<int>(firstHolder->getPosition().z + 0.5f)],
				grid->getGrid()[static_cast<int>(secondHolder->getPosition().x + 0.5f)][static_cast<int>(secondHolder->getPosition().z + 0.5f)]);	
			firstHolder = nullptr;
			secondHolder = nullptr;
					
		}

		//do Picking events here
		if (c.walkQueueDone() && Input::isMouseLeftPressed())
		{
			XMFLOAT2 charPos = c.getPosition(); // (x,y) == (x,z,0)


			int xTile = (int)(round_n(charPos.x, 1) - 0.5f);
			int yTile = (int)(round_n(charPos.y, 1) - 0.5f);

			std::vector<Node*> path = grid->findPath(grid->getTile(xTile, yTile), grid->getTile((int)obj->getPosition().x, (int)obj->getPosition().z));

			XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };
			
			c.Move(c.getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

			for (int i = 0; i < path.size() - 1; i++)
				c.Move(c.getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
			
			for (auto& p : path)
				delete p;

		}
		
	}
}

void GameState::Draw()
{
	
	this->grid->Draw();
	m_stateHUD.Draw();

	//TEST
	c.Draw();
	//this->grid2->Draw();
}

void GameState::_init()
{
	kitchenTile.MakeRectangle();
	kitchenTile.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	kitchenTile.setNormalTexture("trolls_inn/Resources/BatmanNormal.png");
	rect.MakeRectangle();
	rect.setDiffuseTexture("trolls_inn/Resources/Grass.jpg");
	rect.setNormalTexture("trolls_inn/Resources/GrassNormal.png");
	door.LoadModel("trolls_inn/Resources/door/Door.obj");
	door.setDiffuseTexture("trolls_inn/Resources/door/Texture.bmp");
	door.setNormalTexture("trolls_inn/Resources/door/SickDoorNormal.png");
	this->m.LoadModel("trolls_inn/Resources/Wall2.obj");
	this->m.setDiffuseTexture("trolls_inn/Resources/wood.jpg");
	this->m.setNormalTexture("trolls_inn/Resources/woodNormalMap.jpg");
}

void GameState::_checkCreationOfRoom()
{
	
	if (m_firstPick && !m_firstPickedTile)
	{
		if (!p_pickingEvent->empty())
		{
			m_firstPickedTile = p_pickingEvent->top();
			p_pickingEvent->pop();
		}
		else
		{
			m_firstPick = false;
		}
	}
	else if (m_firstPick && m_firstPickedTile && Input::isMouseMiddlePressed())
	{
		this->grid->PickTiles();
		
		if (!p_pickingEvent->empty())
		{
			m_middlePickedTile = p_pickingEvent->top();
			p_pickingEvent->pop();

			DirectX::XMINT2 start(
				static_cast<int>(m_firstPickedTile->getPosition().x + 0.5f),
				static_cast<int>(m_firstPickedTile->getPosition().z + 0.5f)
			);

			DirectX::XMINT2 end(
				static_cast<int>(m_middlePickedTile->getPosition().x + 0.5f),
				static_cast<int>(m_middlePickedTile->getPosition().z + 0.5f)
			);
			
			//this->grid->ResetTileColor(XMINT2(0,0), m_prevEnd);
			m_prevEnd = end;
			m_prevStart = start;
			m_isPlaceable = this->grid->CheckAndMarkTiles(start, end);
			
		}
	}

	if (m_lastPick && !m_lastPickedTile)
	{
		if (!p_pickingEvent->empty())
		{
			m_lastPickedTile = p_pickingEvent->top();
			p_pickingEvent->pop();
		}
		else
		{
			m_lastPick = false;
		}
	}

	if (Input::isMouseMiddlePressed() && !m_firstPick)
	{
		m_firstPick = true;
	
		this->grid->PickTiles();
	}
	else if (!Input::isMouseMiddlePressed() && m_firstPick && !m_lastPick && m_firstPickedTile)
	{
		m_lastPick = true;

		this->grid->PickTiles();
	}
	else if (m_firstPick && m_lastPick)
	{
		if (m_firstPickedTile && m_lastPickedTile  && m_isPlaceable)
		{

			DirectX::XMFLOAT3 posF = m_firstPickedTile->getPosition();
			DirectX::XMFLOAT3 offsetF = m_lastPickedTile->getPosition();
			DirectX::XMINT2 roomPos(static_cast<int>(posF.x + 0.5f), static_cast<int>(posF.z + 0.5f));
			DirectX::XMINT2 roomOffset(static_cast<int>(offsetF.x + 0.5f), static_cast<int>(offsetF.z + 0.5f));
			if (roomPos.x >= 0 && roomPos.y >= 0 && roomOffset.x >= 0 && roomOffset.y >= 0)
			{
				if (roomOffset.x < roomPos.x)
				{
					std::swap(roomPos.x, roomOffset.x);
				}
				if (roomOffset.y < roomPos.y)
				{
					std::swap(roomPos.y, roomOffset.y);
				}

				roomOffset.x -= roomPos.x - 1;
				roomOffset.y -= roomPos.y - 1;

				
				grid->AddRoom(roomPos, roomOffset, RoomType::kitchen, true);
				//grid->CreateWalls();
				//grid->getRoomCtrl().CreateDoors();
			}
		}
		m_firstPickedTile->setColor(1, 1, 1);
		m_lastPickedTile->setColor(1, 1, 1);
		m_firstPickedTile = m_lastPickedTile = nullptr;

		m_lastPick = m_firstPick = false;
	}
	else if (m_firstPick && m_lastPick && m_firstPickedTile && !m_lastPickedTile)
	{
		m_firstPickedTile->setColor(1, 1, 1);
		m_lastPickedTile->setColor(1, 1, 1);
		m_firstPickedTile = m_lastPickedTile = nullptr;
		m_lastPick = m_firstPick = false;
	}
	else if (m_firstPick && m_lastPick && !m_firstPickedTile && m_lastPickedTile)
	{
		m_firstPickedTile->setColor(1, 1, 1);
		m_lastPickedTile->setColor(1, 1, 1);
		m_firstPickedTile = m_lastPickedTile = nullptr;
		m_lastPick = m_firstPick = false;
	}
	else if (m_firstPick && m_lastPick && !m_firstPickedTile && !m_firstPickedTile)
	{
		m_firstPickedTile->setColor(1, 1, 1);
		m_lastPickedTile->setColor(1, 1, 1);
		m_firstPickedTile = m_lastPickedTile = nullptr;
		m_lastPick = m_firstPick = false;
	}
}

void GameState::_setHud()
{
	m_stateHUD.setWindowSize(1280, 720);
	m_stateHUD.LoadHud("trolls_inn/Resources/HUD/HUDDesc.txt");
}
