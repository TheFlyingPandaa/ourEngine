#include "GameState.h"
#include <iostream>
#include <stdlib.h>

GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	m_firstPick = false;
	m_lastPick = false;
	m_firstPickedTile = nullptr;
	m_lastPickedTile = nullptr;

	this->m_cam = cam;
	this->_init();
	grid = new Grid(0, 0, 16, 16, &rect);	
	grid->AddRoom(DirectX::XMINT2(2, 2), DirectX::XMINT2(2, 2), RoomType::kitchen, true);
	posX = 1;
	posY = 1;
	grid->AddRoom(DirectX::XMINT2(4, 2), DirectX::XMINT2(2, 2), RoomType::kitchen);
	//grid->AddRoom(DirectX::XMINT2(3, 0), DirectX::XMINT2(2, 2), RoomType::kitchen);

	grid->CreateWalls(&m);

	//std::cout << sizeof(int) << std::endl;
	//std::cout << sizeof(short) << std::endl;
	//std::cout << sizeof(int64_t) << std::endl;
	
	previousKey = -1;
	//grid->CreateWalls(&m);
}

GameState::~GameState()
{
	delete grid;
}

void GameState::Update(double deltaTime)
{
	//system("cls");
	this->m_cam->update();
	this->grid->Update(this->m_cam);
	_checkCreationOfRoom();

	while (!p_keyEvents->empty() /*&& /*p_keyEvents->top() != 0*/)
	{
		//Do keypress events here
		//std::cout << p_keyEvents->top() << std::endl;
		/*
		if (p_keyEvents->top() == 'W' && p_keyEvents->top() != previousKey)
		{
			
			grid->AddRoom(DirectX::XMINT2(posX, posY++), DirectX::XMINT2(2, 2), RoomType::kitchen, true);
			grid->CreateWalls();
			
		}
		if (p_keyEvents->top() == 'S' && p_keyEvents->top() != previousKey)
		{

			grid->AddRoom(DirectX::XMINT2(posX, posY--), DirectX::XMINT2(2, 2), RoomType::kitchen, true);
			grid->CreateWalls();

		}
		if (p_keyEvents->top() == 'D' && p_keyEvents->top() != previousKey)
		{

			grid->AddRoom(DirectX::XMINT2(posX++, posY), DirectX::XMINT2(2, 2), RoomType::kitchen, true);
			grid->CreateWalls();

		}
		if (p_keyEvents->top() == 'A' && p_keyEvents->top() != previousKey)
		{

			grid->AddRoom(DirectX::XMINT2(posX--, posY), DirectX::XMINT2(2, 2), RoomType::kitchen, true);
			grid->CreateWalls();

		}
		*/
		previousKey = p_keyEvents->top();
		p_keyEvents->pop();

		
	}
	if (p_keyEvents->empty() || p_keyEvents->top() == 0)
		previousKey = -1;
	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		//do Picking events here
	}
}

void GameState::Draw()
{
	
	this->grid->Draw();
	//this->grid2->Draw();
}

void GameState::_init()
{
	rect.MakeRectangle();
	rect.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
	rect.setNormalTexture("trolls_inn/Resources/NormalMap.jpg");
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
		if (m_firstPickedTile && m_lastPickedTile)
		{
			//TODO : CREATE ROOM
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

				std::cout << "POS: " << roomPos.x << ":" << roomPos.y << std::endl;
				std::cout << "OFF: " << roomOffset.x << ":" << roomOffset.y << std::endl;

				grid->AddRoom(roomPos, roomOffset, RoomType::kitchen, false);
				grid->CreateWalls();
			}
		}
		m_firstPickedTile = m_lastPickedTile = nullptr;
		m_lastPick = m_firstPick = false;
	}
	else if (m_firstPick && m_lastPick && m_firstPickedTile && !m_lastPickedTile)
	{
		m_firstPickedTile = m_lastPickedTile = nullptr;
		m_lastPick = m_firstPick = false;
	}
	else if (m_firstPick && m_lastPick && !m_firstPickedTile && m_lastPickedTile)
	{
		m_firstPickedTile = m_lastPickedTile = nullptr;
		m_lastPick = m_firstPick = false;
	}
	else if (m_firstPick && m_lastPick && !m_firstPickedTile && !m_firstPickedTile)
	{
		m_firstPickedTile = m_lastPickedTile = nullptr;
		m_lastPick = m_firstPick = false;
	}
}
