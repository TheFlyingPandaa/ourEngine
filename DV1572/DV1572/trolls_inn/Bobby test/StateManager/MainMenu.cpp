#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	this->m_cam = cam;
	this->_init();
}

MainMenu::~MainMenu()
{

}

void MainMenu::Update(double deltaTime)
{

}

void MainMenu::Draw()
{

}

void MainMenu::_init()
{
}
