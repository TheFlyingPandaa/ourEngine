#pragma once
#include "LevelTracker.h"

enum Professions
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
	Professions profession;
	int posX, posY;

public:
	Staff();
	Staff(Professions profession);
	Staff(Professions profession, int level);
	~Staff();
	
	Professions getProfession() const;
	int getPosX() const;
	int getPosY() const;
	LevelTracker& getLevelTracker();

	// move by offset
	void move(int x, int y);
};