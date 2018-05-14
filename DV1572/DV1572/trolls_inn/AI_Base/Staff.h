#pragma once
#include "LevelTracker.h"
#include "../Bobby test/Character.h"
#include <DirectXMath.h>

enum Professions
{
	Maid,
	Assassin,
	Thief,
	Cook,
	Bartender,
	Guard
};

class Staff : public Character
{
private:
	LevelTracker m_level;
	Professions m_profession;
	DirectX::XMFLOAT2 m_position; 
	Mesh m_trollMesh; 

public:
	Staff();
	Staff(Professions profession);
	Staff(Professions profession, int level);
	~Staff();

	Professions GetProfession() const;
	LevelTracker& GetLevelTracker();
};