#include "GameState.h"
#include <iostream>
#include <stdlib.h>

GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	m_stage = GameStage::Play;
	// Building
	m_startTile = nullptr;
	m_selectedTile = nullptr;
	m_selectedRoomType = RoomType::kitchen;
	m_buildStage = BuildStage::None;
	m_roomPlaceable = false;
	// Building END
	
	m_Rpressed = false;

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

	//<TEMP>
	c.Update();
	//</TEMP>

	

	_handlePicking();	// It's important this is before handleInput();
	_handleInput();		// It's important this is after handlePicking();
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

void GameState::_handleBuildRoom(Shape* pickedShape)
{
	switch (m_buildStage)
	{
	case BuildStage::Start:
		if (pickedShape)
		{
			m_startTile = pickedShape;
			m_buildStage = BuildStage::Selection;
		}
		else
			m_buildStage = BuildStage::None;
		break;
	case BuildStage::Selection:
		if (pickedShape)
			m_selectedTile = pickedShape;
		break;
	}



}

void GameState::_setHud()
{
	m_stateHUD.setWindowSize(1280, 720);
	m_stateHUD.LoadHud("trolls_inn/Resources/HUD/HUDDesc.txt");
}

void GameState::_handlePicking()
{
	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		if (m_buildStage != BuildStage::None)
			_handleBuildRoom(obj);

		if (m_stage == GameStage::Play)
		{
			if (c.walkQueueDone() && m_move)
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
}

void GameState::_handleInput()
{
	if (m_stage == GameStage::BuildRoom)
	{
		if (Input::isMouseLeftPressed())
		{
			if (m_buildStage == BuildStage::None)
			{
				m_buildStage = BuildStage::Start;
				this->grid->PickTiles();
			}
			else if (m_buildStage == BuildStage::Selection)
			{
				this->grid->PickTiles();
				if (m_startTile && m_selectedTile)
				{
					XMFLOAT3 s = m_startTile->getPosition();
					XMFLOAT3 e = m_selectedTile->getPosition();

					XMINT2 start;
					start.x = static_cast<int>(s.x + 0.5f);
					start.y = static_cast<int>(s.z + 0.5f);
					XMINT2 end;
					end.x = static_cast<int>(e.x + 0.5f);
					end.y = static_cast<int>(e.z + 0.5f);

					m_roomPlaceable = this->grid->CheckAndMarkTiles(start, end);
				}
			}
		}
		else if (m_buildStage == BuildStage::Selection && !Input::isMouseLeftPressed())
		{
			m_buildStage = BuildStage::End;
		}
		else if (m_buildStage == BuildStage::End)
		{
			XMFLOAT3 s = m_startTile->getPosition();
			XMFLOAT3 e = m_selectedTile->getPosition();

			XMINT2 start;
			start.x = static_cast<int>(s.x + 0.5f);
			start.y = static_cast<int>(s.z + 0.5f);
			XMINT2 end;
			end.x = static_cast<int>(e.x + 0.5f);
			end.y = static_cast<int>(e.z + 0.5f);

			if (start.x > end.x)
				std::swap(start.x, end.x);
			if (start.y > end.y)
				std::swap(start.y, end.y);
			this->grid->ResetTileColor(start, end);

			end.x -= start.x - 1;
			end.y -= start.y - 1;

			m_buildStage = BuildStage::None;
			m_startTile = nullptr;
			m_selectedTile = nullptr;
			m_roomPlaceable = false;
			this->grid->AddRoom(start, end, m_selectedRoomType);
		}
	}
	else if (m_stage == GameStage::Play)
	{
		if (Input::isMouseLeftPressed())
		{
			this->grid->PickTiles();
			m_move = true;
		}
		else
			m_move = false;
	}


	if (Input::isKeyPressed('B'))
		m_stage = GameStage::BuildRoom;
	else if (Input::isKeyPressed('P'))
		m_stage = GameStage::Play;


	if (Input::isKeyPressed('R') && !m_Rpressed)
		_setHud();
	else if (!Input::isKeyPressed('R'))
		m_Rpressed = false;
}
