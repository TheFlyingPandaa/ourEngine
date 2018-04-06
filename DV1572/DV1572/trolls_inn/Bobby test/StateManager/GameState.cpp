#include "GameState.h"
#include <iostream>
#include <stdlib.h>

GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	this->m_cam = cam;
	this->_init();
	grid = new Grid(0, 0, 64, 64, &rect);	
	grid->AddRoom(DirectX::XMINT2(0, 0), DirectX::XMINT2(2, 2), RoomType::kitchen, true);
	//grid->AddRoom(DirectX::XMINT2(2, 0), DirectX::XMINT2(2, 2), RoomType::kitchen, true);
	//grid->AddRoom(DirectX::XMINT2(0, 2), DirectX::XMINT2(4, 2), RoomType::kitchen, true);
	posX = 1;
	posY = 1;
	//grid->AddRoom(DirectX::XMINT2(2 * pos++, 0), DirectX::XMINT2(2, 2), RoomType::kitchen, true);

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
	std::cout << posX << " " << posY << std::endl;

	while (!p_keyEvents->empty() /*&& /*p_keyEvents->top() != 0*/)
	{
		//Do keypress events here
		//std::cout << p_keyEvents->top() << std::endl;

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
