#include <iostream>
#include <string>
#include <stack>

#include "Grid.h"
#include "StateManager\State.h"
#include "StateManager\GameState.h"
#include "StateManager\MainMenu.h"

int main() 
{
	Grid grid;

	grid.AddRoom(new Kitchen(2, 2, 4, 4), true);
	grid.AddRoom(new Bedroom(6, 6, 4, 4));
	while (true) 
	{		
		grid.DrawString();
		std::string s;
		std::cin >> s;
	}
	
	std::stack<State*> gameStates;

	

	gameStates.push(new MainMenu());
	gameStates.push(new GameState());

	while (!gameStates.empty())
	{

		gameStates.top()->Update(0.0f);
		
		if (gameStates.top()->Exit())
		{
			delete gameStates.top();
			gameStates.pop();
		}
		else
		{
			State * newState = gameStates.top()->NewState();
			if (newState != nullptr)
				gameStates.push(newState);
			else
				gameStates.top()->Draw();
		}

		int i;
		std::cin >> i;
	}

	std::cout << "opsi fuckity dooo we out of states\n";

	return 0;
}