#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu()
{
	std::cout << "Enter MainMenu\n";
}

MainMenu::~MainMenu()
{
	std::cout << "Exit MainMenu\n";
}

void MainMenu::update(double deltaTime)
{
	int counter = 0;
	while (counter > 100000)
	{
		counter++;
	}
	this->m_exitState = true;
}

void MainMenu::draw()
{
	std::cout << "MainMenu draw\n";
}
