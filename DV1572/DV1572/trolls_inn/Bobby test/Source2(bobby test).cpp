#include <iostream>
#include <string>
#include "Grid.h"


int main() 
{
	Grid grid;

	grid.addRoom(new Kitchen(2, 2, 4, 4));
	grid.addRoom(new Bedroom(4, 4, 4, 4));
	while (true) 
	{		
		grid.drawString();
		std::string s;
		std::cin >> s;
	}
	return 0;
}