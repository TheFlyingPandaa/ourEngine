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

LevelTracker& Staff::getLevelTracker()
{
	return this->level;
}