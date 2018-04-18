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
	LevelTracker m_level;
	Professions m_profession;

public:
	Staff();
	Staff(Professions profession);
	Staff(Professions profession, int level);
	~Staff();
	
	Professions GetProfession() const;
	LevelTracker& GetLevelTracker();
};