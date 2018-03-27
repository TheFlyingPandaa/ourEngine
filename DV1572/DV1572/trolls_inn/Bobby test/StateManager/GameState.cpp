#include "GameState.h"
#include <iostream>


GameState::GameState(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	this->m_cam = cam;
	this->_init();
}

GameState::~GameState()
{
	
}

void GameState::Update(double deltaTime)
{
	this->obj.CheckPick();
}

void GameState::Draw()
{
	this->obj.Draw();
}

void GameState::_init()
{
	this->m.LoadModel("trolls_inn/Resources/wall.obj");
	this->m.setDiffuseTexture("trolls_inn/Resources/wood.jpg");
	this->obj.setMesh(&m);
	
	this->obj.setPos(-1.1f, -1.0f, 0);
	
}
