#include "GameState.h"
#include <iostream>

GameState::GameState() : State()
{
	std::cout << "Enterd GameState\n";
}

GameState::~GameState()
{
	std::cout << "Exit GameState\n";
}

void GameState::update(double deltaTime)
{
	int counter = 0;
	while (counter < 100000)
	{
		counter++;
	}
	std::cout << "GameState is complete\n";
	this->m_exitState = true;
	
}

void GameState::draw()
{
	std::cout << "GameState draw\n";
}