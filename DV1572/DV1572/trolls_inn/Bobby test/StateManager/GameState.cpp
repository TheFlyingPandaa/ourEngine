#include "GameState.h"
#include <iostream>


GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	this->m_cam = cam;
	this->_init();
	grid = new Grid(10, 10, 8, 8);
	grid->AddRoom(new Kitchen(0, 0, 8, 3, &m));

	grid->AddRoom(new Kitchen(8, 3, 4, 2, &m));
}

GameState::~GameState()
{
	delete grid;
}

void GameState::Update(double deltaTime)
{
	this->m_cam->update();



	this->obj.CheckPick();
	while (!p_keyEvents->empty())
	{
		//DO STUFF
	}
	while (!p_pickingEvent->empty())
	{
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		obj->Move(0.1f, 0, 0);
	}
}

void GameState::Draw()
{
	//this->obj.Draw();
	this->grid->Draw();
}

void GameState::_init()
{
	this->m.LoadModel("trolls_inn/Resources/wall.obj");
	this->m.setDiffuseTexture("trolls_inn/Resources/wood.jpg");
	this->obj.setMesh(&m);
	
	this->obj.setPos(0.5f, 0.0f, 0);
	this->obj.setScale(0.5f);
	
}
