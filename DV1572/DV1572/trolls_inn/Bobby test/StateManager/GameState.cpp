#include "GameState.h"
#include <iostream>


GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	this->m_cam = cam;
	this->_init();
	grid = new Grid(0, 0, 8, 8);	
	grid->AddRoom(new Kitchen(0, 0, 8, 3));


	grid->CreateWalls(&m);
}

GameState::~GameState()
{
	delete grid;
}

void GameState::Update(double deltaTime)
{
	//this->m_cam->update();

	while (!p_keyEvents->empty())
	{
		//Do keypress events here
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
	this->m.LoadModel("trolls_inn/Resources/Wall.obj");
	this->m.setDiffuseTexture("trolls_inn/Resources/Untitled.bmp");
}
