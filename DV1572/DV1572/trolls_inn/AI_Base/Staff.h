#pragma once
#include "LevelTracker.h"

enum Type
{
	Maid,
	Assassin,
	Thief,
	Cook,
	Bartender,
	Guard
};

class Staff
{
private:
	LevelTracker level;
	Type profession;
	int posX, posY;

public:
	Type getProfession() const;
	int getPosX() const;
	int getPosY() const;
};