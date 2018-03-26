#include <iostream>
#include <string>
#include "Grid.h"


int main() 
{
	Grid grid;

	grid.addRoom(new Kitchen(2, 2, 4, 4), true);
	grid.addRoom(new Bedroom(6, 6, 4, 4));
	while (true) 
	{		
		grid.drawString();
		std::string s;
		std::cin >> s;
	}
	return 0;
}