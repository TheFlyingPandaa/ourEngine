#include "GameState.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <future>
#include "../../../ourEngine/interface/light/PointLight.h"
#include "../../../ourEngine/core/Dx.h"

GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	m_stage = GameStage::BuildRoom;
	// Building
	m_startTile = nullptr;
	m_selectedTile = nullptr;
	m_selectedRoomType = RoomType::kitchen;
	m_buildStage = BuildStage::None;
	m_roomPlaceable = false;
	m_hasClicked = false;
	m_colorButton = false;
	m_hudPickStage = HudPickingStage::Miss;
	// Building END
	m_lastPickedIndex = -1;
	m_Rpressed = false;

	_setHud();

	box.LoadModel("trolls_inn/Resources/box.obj");

	c.setModel(&box);
	c.setPosition( 10+0.5, 2+0.5);
	c.setFloor(0);

	int startSize = 32;
	int firstRoomSizeX = 4;
	int firstRoomSizeY = 3;

	int secondRoomSizeX = 4;
	int secondRoomSizeY = 2;

	this->m_cam = cam;
	this->_init();
	grid = new Grid(0, 0, startSize, startSize, &rect);	
	grid->CreateWalls(&m);	
	grid->getRoomCtrl().setTileMesh(&kitchenTile, RoomType::kitchen);
	grid->getRoomCtrl().setDoorMesh(&door);
	grid->AddRoom(DirectX::XMINT2((startSize / 2) - firstRoomSizeX / 2, 4), DirectX::XMINT2(firstRoomSizeX, firstRoomSizeY), RoomType::kitchen, true);
	//grid->getRoomCtrl().CreateDoor(grid->getGrid()[(startSize / 2)][4], grid->getGrid()[(startSize / 2)][3]);
	grid->getRoomCtrl().CreateMainDoor(grid->getGrid()[(startSize / 2)][4], grid->getGrid()[(startSize / 2)][3]);	//This will create the main door and place the pos in in m_mainDoorPos 
	
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
	//auto currentTime = std::chrono::high_resolution_clock::now();
	if (Input::isKeyPressed('N')) {
		m_newState = new MainMenu(p_pickingEvent, p_keyEvents, m_cam);
	}
	auto currentTime = std::chrono::high_resolution_clock::now();
	
	this->m_cam->update();
	this->grid->Update(this->m_cam);
	m_colorButton = false;
	gameTime.updateCurrentTime(deltaTime); 
	auto time = std::chrono::high_resolution_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(time - currentTime).count();
	//std::cout << " TIME: " << dt << std::endl;
	

	//<TEMP>
	c.Update();
	if (c.walkQueueDone())
	{
		if ((int)((c.getPosition().x - 0.5) / 1) == m_mainDoorPos.x && (int)(round_n(c.getPosition().y, 1)) == m_mainDoorPos.y && m_justMoved == false)
		{
			c.Move(Character::UP);
			std::cout << " " << c.getPosition().x << " " << c.getPosition().y << std::endl;
			m_justMoved = true;
		}
		else if ((int)((c.getPosition().x - 0.5) / 1) == m_mainDoorPos.x && (int)(round_n(c.getPosition().y, 1)) == m_mainDoorPos.y + 1 && m_justMoved == false)
		{
			c.Move(Character::DOWN);
			std::cout << " " << c.getPosition().x << " " << c.getPosition().y << std::endl;
			m_justMoved = true;
		}
	}

	//if (Input::isKeyPressed('A'))
	//{
	//	c.Move(Character::LEFT);
	//}
	//if (Input::isKeyPressed('W'))
	//{
	//	c.Move(Character::UP);
	//}
	//if (Input::isKeyPressed('S'))
	//{
	//	c.Move(Character::DOWN);
	//}
	//if (Input::isKeyPressed('D'))
	//{
	//	c.Move(Character::RIGHT);
	//}
	//</TEMP>

	 // Get result.

	_handlePicking();	// It's important this is before handleInput();
	_handleInput();		// It's important this is after handlePicking();
	
}

void GameState::Draw()
{
	gameTime.m_cpyLightToGPU();
	this->grid->Draw();

	//TEST
	c.Draw();
	//this->grid2->Draw();
	test.setScale(5);
	test.setPos(2.5f, 2.5f, 2.5f);
	test.setMesh(&box);
	test.CastShadow();
	test.Draw();

}

void GameState::DrawHUD()
{
	m_stateHUD.Draw();
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
	door.setNormalTexture("trolls_inn/Resources/door/SickDoorNormal.png");
	this->m.LoadModel("trolls_inn/Resources/Wall2.obj");
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
	m_stateHUD.LoadHud("trolls_inn/Resources/HUD/HUDDesc.txt");
}

void GameState::_handlePicking()
{
	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		if (m_hudPickStage != HudPickingStage::Miss)
			_handleHUDPicking(dynamic_cast<RectangleShape*>(obj));
		else if (m_buildStage != BuildStage::None)
			_handleBuildRoom(obj);


		using namespace std::chrono_literals;

		// Create a promise and get its future.
		if (m_i == 0)
		{
			m_i++;
			future = std::async(std::launch::async, &GameState::_handlePickingAi, this, obj);
		}


		// Use wait_for() with zero milliseconds to check thread status.
		auto status = future.wait_for(0ms);

		// Print status. And start a new thread if the other thread was finnished
		if (status == std::future_status::ready) {
			//std::cout << "Thread finished" << std::endl;
			future.get();
			future = std::async(std::launch::async, &GameState::_handlePickingAi, this, obj);
			
		}
		else {
			//std::cout << "Thread still running" << std::endl;
		}

		//_handlePickingAi(obj);
		
	}
}

void GameState::_handlePickingAi(Shape * obj)
{

	if (m_stage == GameStage::Play)
	{
		if (c.walkQueueDone() && m_move)
		{
			//Shape * obj = this->p_pickingEvent->top();
			XMFLOAT2 charPos = c.getPosition(); // (x,y) == (x,z,0)

			int xTile = (int)(round_n(charPos.x, 1) - 0.5f);
			int yTile = (int)(round_n(charPos.y, 1) - 0.5f);

			std::vector<std::shared_ptr<Node>> path = grid->findPathHighLevel(grid->getTile(xTile, yTile), grid->getTile((int)obj->getPosition().x, (int)obj->getPosition().z));

			XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };

			if (path.size() != 0)
			{
				m_justMoved = false;

				c.Move(c.getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

				for (int i = 0; i < path.size() - 1; i++)
					c.Move(c.getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
			}

		}
	}
}


void GameState::_handleHUDPicking(RectangleShape* r)
{
	if (r)
	{
		switch (m_hudPickStage)
		{
		case HudPickingStage::Hover:
			m_colorButton = true;
			if (r->getIndex() != m_lastPickedIndex)
			{
				m_lastPickedIndex = r->getIndex();
				m_stateHUD.ResetColorsOnPickable();
			}
			switch (r->getIndex())
			{
			case 0:
				r->setColor(0.2f, 0.2f, 2.0f);
				break;
			case 1:
				r->setColor(2.0f, 0.2f, 0.2f);
				break;
			case 2:
				r->setColor(1.5f, 0.2f, 1.5f);
				break;
			case 3:
				r->setColor(0.2f, 1.5f, 1.5f);
				break;
			}
			break;
		case HudPickingStage::Click:
			m_colorButton = true;
			//do something
			std::cout << "Something Picked";
			break;
		}
	}
	else
	{
		m_hudPickStage = HudPickingStage::Miss;
	}
}

void GameState::_handleInput()
{
	if (m_stateHUD.isMouseInsidePotentialArea(Input::getMousePositionLH()))
	{
		m_stateHUD.CheckIfPicked();
		m_hudPickStage = HudPickingStage::Hover;
		if (Input::isMouseLeftPressed() && !m_hasClicked)
		{
			m_hudPickStage = HudPickingStage::Click;
			m_hasClicked = true;
		}
		else if (!Input::isMouseLeftPressed() && m_hasClicked)
		{
			m_hasClicked = false;
		}
		if (!m_colorButton)
			m_stateHUD.ResetColorsOnPickable();
	}
	else
	{
		if (m_hudPickStage != HudPickingStage::Miss)
		{
			m_hudPickStage = HudPickingStage::Miss;
			m_stateHUD.ResetColorsOnPickable();
		}


		if (m_stage == GameStage::BuildRoom)
		{
			_buildInput();
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

void GameState::_buildInput()
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
			this->grid->PickTiles(m_selectedTile);
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
