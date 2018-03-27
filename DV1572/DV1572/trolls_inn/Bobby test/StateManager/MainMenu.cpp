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

void MainMenu::Update(double deltaTime)
{
	int counter = 0;
	while (counter > 100000)
	{
		counter++;
	}
	this->m_exitState = true;
}

void MainMenu::Draw()
{
	std::cout << "MainMenu draw\n";
}
