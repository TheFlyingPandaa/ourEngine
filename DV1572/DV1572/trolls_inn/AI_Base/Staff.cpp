#include "Staff.h"

Staff::Staff()
{
}

Staff::Staff(Professions profession)
{
	this->profession = profession;
}

Staff::Staff(Professions profession, int level)
{
	this->profession = profession;
	this->level.setLevel(level);
}

Staff::~Staff()
{
}

Professions Staff::getProfession() const
{
	return this->profession;
}

int Staff::getPosX() const
{
	return this->posX;
}

int Staff::getPosY() const
{
	return this->posY;
}

LevelTracker& Staff::getLevelTracker()
{
	return this->level;
}

void Staff::move(int x, int y)
{
	this->posX += x;
	this->posY += y;
}