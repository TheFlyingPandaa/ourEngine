#include "GameState.h"
#include <iostream>


GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	this->m_cam = cam;
	this->_init();
	grid = new Grid(0, 0, 8, 8);	
	grid->AddRoom(DirectX::XMINT2(0, 0), DirectX::XMINT2(1, 1), RoomType::kitchen, true);
	grid->AddRoom(DirectX::XMINT2(0,1), DirectX::XMINT2(2,2), RoomType::kitchen, true);

	grid->CreateWalls(&m);

	//grid->CreateWalls(&m);
}

GameState::~GameState()
{
	delete grid;
}

void GameState::Update(double deltaTime)
{
	this->m_cam->update(Input::getMousePosition());

	while (!p_keyEvents->empty() && p_keyEvents->top() != 0)
	{
		//Do keypress events here
		std::cout << p_keyEvents->top() << std::endl;
		p_keyEvents->pop();
	}
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
	this->m.LoadModel("trolls_inn/Resources/Wall2.obj");
	this->m.setDiffuseTexture("trolls_inn/Resources/wood.jpg");
	this->m.setNormalTexture("trolls_inn/Resources/woodNormalMap.jpg");
}
