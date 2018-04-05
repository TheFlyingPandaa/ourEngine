#pragma once
#include "LevelTracker.h"

class Staff
{
public:
	enum Type
	{
		Maid,
		Assassin,
		Thief,
		Cook,
		Bartender,
		Guard
	};

private:
	LevelTracker level;
	Type profession;
	int posX, posY;

public:
	Type getProfession() const;
	int getPosX() const;
	int getPosY() const;
};